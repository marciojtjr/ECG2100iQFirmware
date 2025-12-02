#define _DEFAULT_SOURCE
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>

// Define CRTSCTS if not available (hardware flow control)
//#ifndef CRTSCTS
//#define CRTSCTS 020000000000
//#endif

#include "bgapi_parser.h"
#include "bgapi_commands.h"

#define SERIAL_PORT "/dev/ttyUSB1"
#define BAUD_RATE B115200
#define BUFFER_SIZE 256

static volatile int running = 1;
static bgapi_parser_state_t parser_state;
static int serial_fd = -1;
static volatile int waiting_for_response = 0;
static volatile int response_received = 0;

void signal_handler(int signum) {
    (void)signum;
    running = 0;
    printf("\nShutting down...\n");
}

int configure_serial_port(int fd) {
    struct termios tty;

    // Get current port settings
    if (tcgetattr(fd, &tty) != 0) {
        perror("Error getting terminal attributes");
        return -1;
    }

    // Set baud rate
    cfsetospeed(&tty, BAUD_RATE);
    cfsetispeed(&tty, BAUD_RATE);

    // 8 data bits, 1 stop bit, no parity
    tty.c_cflag &= ~PARENB;        // No parity
    tty.c_cflag &= ~CSTOPB;        // 1 stop bit
    tty.c_cflag &= ~CSIZE;         // Clear data size bits
    tty.c_cflag |= CS8;            // 8 data bits

    // Enable hardware flow control (RTS/CTS)
    tty.c_cflag |= CRTSCTS;

    // Enable receiver, ignore modem control lines
    tty.c_cflag |= CREAD | CLOCAL;

    // Raw mode (no input processing)
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Raw output mode
    tty.c_oflag &= ~OPOST;

    // No input processing
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);           // No software flow control
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    // Set read timeout (blocking read with timeout)
    tty.c_cc[VMIN] = 0;     // Minimum number of characters to read
    tty.c_cc[VTIME] = 1;    // Timeout in deciseconds (0.1 second)

    // Apply settings
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("Error setting terminal attributes");
        return -1;
    }

    // Flush any existing data
    tcflush(fd, TCIOFLUSH);

    return 0;
}

void print_hex_and_ascii(const unsigned char *buffer, int length) {
    printf("Received %d bytes:\n", length);

    // Print hex dump
    printf("Hex: ");
    for (int i = 0; i < length; i++) {
        printf("%02X ", buffer[i]);
        if ((i + 1) % 16 == 0 && (i + 1) < length) {
            printf("\n     ");
        }
    }
    printf("\n");

    // Print ASCII (printable characters only)
    printf("ASCII: ");
    for (int i = 0; i < length; i++) {
        if (buffer[i] >= 32 && buffer[i] <= 126) {
            printf("%c", buffer[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");
}

int wait_for_bgapi_response(int timeout_ms) {
    int elapsed = 0;
    response_received = 0;
    waiting_for_response = 1;

    while (elapsed < timeout_ms && !response_received) {
        usleep(10000);  // 10ms
        elapsed += 10;
    }

    waiting_for_response = 0;

    if (!response_received) {
        printf("⚠️  TIMEOUT waiting for response!\n");
        return -1;
    }

    return 0;
}

int send_bgapi_command_and_wait(bgapi_command_t *cmd, int timeout_ms) {
    bgapi_print_command(cmd);
    bgapi_send_command(serial_fd, cmd);
    return wait_for_bgapi_response(timeout_ms);
}

void bt122_set_device_name(const char *name) {
    bgapi_command_t cmd;
    uint8_t name_data[20];
    int name_len = strlen(name);
    if (name_len > 20) name_len = 20;
    memcpy(name_data, name, name_len);

    printf(">>> Setting Device Name: %s\n", name);
    // Write device name to PS store (key 0x8000 for local name)
    bgapi_cmd_ps_save(&cmd, 0x8000, name_data, name_len);
    send_bgapi_command_and_wait(&cmd, 1000);
}

void handle_bgapi_event(const bgapi_packet_t *packet) {
    bgapi_command_t cmd;

    // Check if this is a response to our command
    if (packet->message_type == BGAPI_MSG_TYPE_RESPONSE) {
        response_received = 1;
        if (packet->payload_length > 0 && packet->payload[0] != 0) {
            printf("⚠️  Command returned error code: 0x%04X\n", packet->payload[0]);
        } else {
            printf("✅ Command successful\n");
        }
        return;
    }

    // Handle System Boot Event
    if (packet->class_id == BGAPI_CLASS_SYSTEM && packet->command_id == BGAPI_EVENT_SYSTEM_BOOT) {
        printf("\n");
        printf("╔══════════════════════════════════════════════════════════════╗\n");
        printf("║ *** BT122 Module Booted! ***                                 ║\n");
        printf("╚══════════════════════════════════════════════════════════════╝\n");
        printf("\n");
        // Module just booted, initialization will happen after "ready" event
    }

    // Handle System Ready Event
    else if (packet->class_id == BGAPI_CLASS_SYSTEM && packet->command_id == 0x01) {
        printf("\n");
        printf("╔══════════════════════════════════════════════════════════════╗\n");
        printf("║ *** BT122 Module Ready! Starting Initialization... ***       ║\n");
        printf("║                                                              ║\n");
        printf("║ BT122 = DUAL-MODE Bluetooth (BR/EDR + BLE)                   ║\n");
        printf("║                                                              ║\n");
        printf("║ Supported:                                                   ║\n");
        printf("║  ✅ Bluetooth Classic (BR/EDR)                               ║\n");
        printf("║  ✅ Bluetooth Low Energy (BLE)                               ║\n");
        printf("║  ✅ SPP (Serial Port Profile)                                ║\n");
        printf("║  ✅ Apple iAP/iAP2                                           ║\n");
        printf("║  ✅ GATT Services                                            ║\n");
        printf("╚══════════════════════════════════════════════════════════════╝\n");
        printf("\n");

        // BT122 (BLE) initialization sequence with proper command-response handling

        // 1. Get system info
        printf(">>> Step 1: Get System Info\n");
        bgapi_cmd_system_get_info(&cmd);
        if (send_bgapi_command_and_wait(&cmd, 1000) < 0) {
            printf("❌ Failed to get system info\n");
            return;
        }
        usleep(100000);

        // 2. Set device name (equivalent to "SET BT NAME SpauCliQ...")
        printf("\n>>> Step 2: Set Device Name\n");
        bt122_set_device_name("SpauCliQ_BLE");
        usleep(100000);

        // 3. Configure GAP parameters
        printf("\n>>> Step 3: Configure GAP (discoverable & connectable)\n");
        bgapi_cmd_gap_set_mode(&cmd, 0x02, 0x02);
        if (send_bgapi_command_and_wait(&cmd, 1000) < 0) {
            printf("❌ Failed to set GAP mode\n");
            return;
        }

        printf("\n");
        printf("╔══════════════════════════════════════════════════════════════╗\n");
        printf("║ ✅ BT122 Initialization Complete                            ║\n");
        printf("║                                                              ║\n");
        printf("║ Module is now:                                               ║\n");
        printf("║  • Advertising as 'SpauCliQ_BLE'                            ║\n");
        printf("║  • BLE: Connectable via GATT                                 ║\n");
        printf("║  • Classic BT: SPP/iAP profiles ready                        ║\n");
        printf("║                                                              ║\n");
        printf("║ 💡 Dual-mode: Supports both BLE and Classic BT!             ║\n");
        printf("╚══════════════════════════════════════════════════════════════╝\n");
        printf("\n");
    }

    // Handle Connection Status
    else if (packet->class_id == BGAPI_CLASS_CONNECTION && packet->command_id == 0x00) {
        printf("\n*** Connection Established! ***\n\n");
    }

    // Handle Disconnection
    else if (packet->class_id == BGAPI_CLASS_CONNECTION && packet->command_id == 0x04) {
        printf("\n*** Connection Disconnected! ***\n\n");

        // Re-enable advertising
        printf(">>> Re-enabling GAP advertising...\n");
        bgapi_cmd_gap_set_mode(&cmd, 0x02, 0x02);
        bgapi_print_command(&cmd);
        bgapi_send_command(serial_fd, &cmd);
    }
}

void process_bgapi_data(const unsigned char *buffer, int length) {
    static int remaining_offset = 0;
    bgapi_packet_t packet;

    // Feed data to parser
    int offset = 0;
    while (offset < length) {
        int result = bgapi_parser_feed(&parser_state, buffer + offset, length - offset, &packet);

        if (result > 0) {
            // Found a complete packet
            bgapi_print_packet(&packet);

            // Handle events and send responses
            handle_bgapi_event(&packet);

            // Move offset past the parsed packet
            offset += packet.total_length - remaining_offset;
            remaining_offset = 0;
        } else {
            // No complete packet yet, need more data
            remaining_offset = parser_state.buffer_pos;
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    unsigned char buffer[BUFFER_SIZE];
    int bytes_read;
    const char *port = SERIAL_PORT;

    // Allow custom port as command line argument
    if (argc > 1) {
        port = argv[1];
    }

    printf("Linux BT122 DevKit Protocol Monitor\n");
    printf("====================================\n");
    printf("Opening serial port: %s\n", port);
    printf("Settings: 115200 baud, 8N1, Hardware Flow Control (RTS/CTS)\n");
    printf("Press Ctrl+C to exit\n\n");

    // Set up signal handler for graceful shutdown
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // Open serial port
    serial_fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
    if (serial_fd < 0) {
        perror("Error opening serial port");
        fprintf(stderr, "Make sure %s exists and you have permission to access it.\n", port);
        fprintf(stderr, "You may need to run: sudo usermod -a -G dialout $USER\n");
        return EXIT_FAILURE;
    }

    // Configure serial port
    if (configure_serial_port(serial_fd) != 0) {
        close(serial_fd);
        return EXIT_FAILURE;
    }

    printf("Serial port opened and configured successfully.\n");
    printf("Waiting for data...\n\n");

    // Initialize BGAPI parser
    bgapi_parser_init(&parser_state);

    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║ Waiting for BT122 Boot/Ready events...                      ║\n");
    printf("║ (Module should auto-boot, or reset it manually)             ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");

    // Don't send hello immediately - wait for module to boot
    // We'll send commands when we receive the "ready" event

    // Main read loop
    while (running) {
        bytes_read = read(serial_fd, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0) {
            // Display raw data in hex and ASCII
            print_hex_and_ascii(buffer, bytes_read);

            // Parse and display BGAPI packets
            process_bgapi_data(buffer, bytes_read);

            // Also write raw data to stdout for piping if needed
            // write(STDOUT_FILENO, buffer, bytes_read);
        } else if (bytes_read < 0) {
            if (errno != EAGAIN && errno != EINTR) {
                perror("Error reading from serial port");
                break;
            }
        }
        // bytes_read == 0 means timeout, continue waiting
    }

    // Cleanup
    printf("Closing serial port...\n");
    close(serial_fd);

    return EXIT_SUCCESS;
}
