#ifdef _WIN32
    #define _CRT_SECURE_NO_WARNINGS
    #include <windows.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <signal.h>
    #include <conio.h>
    #include <io.h>
    #include <process.h>
    #define usleep(x) Sleep((x)/1000)
    #define SIGTERM SIGBREAK
#else
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
    #include <pthread.h>
#endif

// Define CRTSCTS if not available (hardware flow control)
//#ifndef CRTSCTS
//#define CRTSCTS 020000000000
//#endif

#include "bgapi_parser.h"
#include "bgapi_commands.h"

#ifdef _WIN32
    #define SERIAL_PORT "COM6"
    #define BUFFER_SIZE 256
    static volatile int running = 1;
    static bgapi_parser_state_t parser_state;
    static HANDLE serial_handle = INVALID_HANDLE_VALUE;
#else
    #define SERIAL_PORT "/dev/ttyUSB1"
    #define BAUD_RATE B115200
    #define BUFFER_SIZE 256
    static volatile int running = 1;
    static bgapi_parser_state_t parser_state;
    static int serial_fd = -1;
#endif

// Forward declarations
void process_bgapi_data(const unsigned char *buffer, int length);
void handle_bgapi_event(const bgapi_packet_t *packet);

void signal_handler(int signum) {
    (void)signum;
    running = 0;
    printf("\nShutting down...\n");
}

#ifdef _WIN32
int configure_serial_port(HANDLE handle) {
    DCB dcb;
    COMMTIMEOUTS timeouts;

    // Get current DCB settings
    if (!GetCommState(handle, &dcb)) {
        printf("Error getting DCB settings\n");
        return -1;
    }

    // Configure DCB
    dcb.BaudRate = CBR_115200;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;  // Hardware flow control
    dcb.fOutxCtsFlow = TRUE;                  // Hardware flow control
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fOutX = FALSE;                        // No software flow control
    dcb.fInX = FALSE;                         // No software flow control

    // Set DCB
    if (!SetCommState(handle, &dcb)) {
        printf("Error setting DCB settings\n");
        return -1;
    }

    // Configure timeouts
    timeouts.ReadIntervalTimeout = 1;
    timeouts.ReadTotalTimeoutConstant = 100;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    timeouts.WriteTotalTimeoutConstant = 100;
    timeouts.WriteTotalTimeoutMultiplier = 1;

    if (!SetCommTimeouts(handle, &timeouts)) {
        printf("Error setting timeouts\n");
        return -1;
    }

    // Purge any existing data
    PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR);

    return 0;
}
#else
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
#endif

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



void handle_bgapi_event(const bgapi_packet_t *packet) {
    bgapi_command_t cmd;

    // Check if this is a response to our command
    if (packet->message_type == BGAPI_MSG_TYPE_RESPONSE) {
        bgapi_handle_response(packet);
        return;
    }

    // Handle System Boot Event
    if (packet->class_id == BGAPI_CLASS_SYSTEM && packet->command_id == BGAPI_EVENT_SYSTEM_BOOT) {
        printf("\n");
        printf("╔══════════════════════════════════════════════════════════════╗\n");
        printf("║ *** BT122 Module Booted! ***                                 ║\n");
        printf("╚══════════════════════════════════════════════════════════════╝\n");
        printf("\n");
        // Module just booted, wait for initialized event
    }

    // Handle System Initialized Event (BT122)  
    else if (packet->class_id == BGAPI_CLASS_SYSTEM && packet->command_id == BGAPI_EVENT_SYSTEM_INITIALIZED) {
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

        // BT122 initialization sequence as per commands.txt

        // 1. Set local name  
        printf(">>> Step 1: Set Local Name to 'Spaulding'\n");
        bgapi_cmd_system_set_local_name(&cmd, (const uint8_t*)"Spaulding", 9);
#ifdef _WIN32
        int result = bgapi_send_command_and_wait(serial_handle, &cmd, 2000);
#else
        int result = bgapi_send_command_and_wait(serial_fd, &cmd, 2000);
#endif
        if (result != 0) {
            printf("❌ Failed to set local name (error: 0x%04X)\n", result);
            // Continue anyway, this might not be critical
        }

        // 2. Set class of device (0x202 = Audio/Video device)
        printf("\n>>> Step 2: Set Class of Device\n");
        bgapi_cmd_system_set_class_of_device(&cmd, 0x202);
#ifdef _WIN32
        result = bgapi_send_command_and_wait(serial_handle, &cmd, 2000);
#else
        result = bgapi_send_command_and_wait(serial_fd, &cmd, 2000);
#endif
        if (result != 0) {
            printf("❌ Failed to set class of device (error: 0x%04X)\n", result);
            // Continue anyway
        }

        // 3. Configure BT GAP mode (connectable=1, discoverable=1, limited=0)
        printf("\n>>> Step 3: Configure BT GAP Mode\n");
        bgapi_cmd_bt_gap_set_mode(&cmd, 1, 1, 0);
#ifdef _WIN32
        result = bgapi_send_command_and_wait(serial_handle, &cmd, 2000);
#else
        result = bgapi_send_command_and_wait(serial_fd, &cmd, 2000);
#endif
        if (result != 0) {
            printf("❌ Failed to set BT GAP mode (error: 0x%04X)\n", result);
            // Continue anyway
        }

        // 4. Set bondable mode
        printf("\n>>> Step 4: Set Bondable Mode\n");
        bgapi_cmd_sm_set_bondable_mode(&cmd, 1);
#ifdef _WIN32
        result = bgapi_send_command_and_wait(serial_handle, &cmd, 2000);
#else
        result = bgapi_send_command_and_wait(serial_fd, &cmd, 2000);
#endif
        if (result != 0) {
            printf("❌ Failed to set bondable mode (error: 0x%04X)\n", result);
            // Continue anyway
        }

        // 5. Start RFCOMM server
        printf("\n>>> Step 5: Start RFCOMM Server\n");
        bgapi_cmd_bt_rfcomm_start_server(&cmd, 2, 0);  // sdp_id=2, streaming_destination=0
#ifdef _WIN32
        result = bgapi_send_command_and_wait(serial_handle, &cmd, 2000);
#else
        result = bgapi_send_command_and_wait(serial_fd, &cmd, 2000);
#endif
        if (result != 0) {
            printf("❌ Failed to start RFCOMM server (error: 0x%04X)\n", result);
            // Continue anyway
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

    // Handle BT Connection Opened (BT122)
    else if (packet->class_id == BGAPI_CLASS_BT_CONNECTION && packet->command_id == BGAPI_EVENT_BT_CONNECTION_OPENED) {
        printf("\n*** BT Classic Connection Opened! ***\n");
        if (packet->payload_length >= 9) {
            printf("Device Address: ");
            for (int i = 0; i < 6; i++) {
                printf("%02X", packet->payload[i]);
                if (i < 5) printf(":");
            }
            printf("\n");
            printf("Central: %d\n", packet->payload[6]);
            printf("Connection: %d\n", packet->payload[7]); 
            printf("Bonding: %d\n", packet->payload[8]);
        }
        printf("\n");
    }

    // Handle Endpoint Status
    else if (packet->class_id == BGAPI_CLASS_ENDPOINT && packet->command_id == BGAPI_EVENT_ENDPOINT_STATUS) {
        printf("\n*** Endpoint Status Update ***\n");
        if (packet->payload_length >= 7) {
            printf("Endpoint: %d\n", packet->payload[0]);
            printf("Type: %d\n", packet->payload[1]);
            printf("Flags: 0x%02X\n", packet->payload[6]);
        }
        printf("\n");
    }

    // Handle Legacy Connection Events (for compatibility)
    else if (packet->class_id == BGAPI_CLASS_CONNECTION && packet->command_id == 0x00) {
        printf("\n*** Connection Established! ***\n\n");
    }

    // Handle Legacy Disconnection
    else if (packet->class_id == BGAPI_CLASS_CONNECTION && packet->command_id == 0x04) {
        printf("\n*** Connection Disconnected! ***\n\n");

        // Re-enable BT GAP advertising for BT122
        printf(">>> Re-enabling BT GAP mode...\n");
        bgapi_cmd_bt_gap_set_mode(&cmd, 1, 1, 0);
#ifdef _WIN32
        bgapi_send_command_and_wait(serial_handle, &cmd, 1000);
#else
        bgapi_send_command_and_wait(serial_fd, &cmd, 1000);
#endif
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
#ifdef _WIN32
    serial_handle = CreateFile(port,
                              GENERIC_READ | GENERIC_WRITE,
                              0,
                              NULL,
                              OPEN_EXISTING,
                              0,
                              NULL);
    
    if (serial_handle == INVALID_HANDLE_VALUE) {
        printf("Error opening serial port %s\n", port);
        printf("Make sure the port exists and is not being used by another application.\n");
        return EXIT_FAILURE;
    }

    // Configure serial port
    if (configure_serial_port(serial_handle) != 0) {
        CloseHandle(serial_handle);
        return EXIT_FAILURE;
    }
#else
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
#endif

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

    // Main read loop - now just handles events after initialization
    printf(">>> Listening for events and connections...\n");
    while (running) {
#ifdef _WIN32
        DWORD dwBytesRead;
        if (ReadFile(serial_handle, buffer, sizeof(buffer) - 1, &dwBytesRead, NULL)) {
            bytes_read = (int)dwBytesRead;
        } else {
            bytes_read = -1;
        }
#else
        bytes_read = read(serial_fd, buffer, sizeof(buffer) - 1);
#endif

        if (bytes_read > 0) {
            // Display raw data in hex and ASCII
            print_hex_and_ascii(buffer, bytes_read);

            // Parse and display BGAPI packets
            process_bgapi_data(buffer, bytes_read);
        } else if (bytes_read < 0) {
#ifdef _WIN32
            DWORD error = GetLastError();
            if (error != ERROR_TIMEOUT) {
                printf("Error reading from serial port: %lu\n", error);
                break;
            }
#else
            if (errno != EAGAIN && errno != EINTR) {
                perror("Error reading from serial port");
                break;
            }
#endif
        }
        // bytes_read == 0 means timeout, continue waiting
    }

    // Cleanup
    printf("Closing serial port...\n");
#ifdef _WIN32
    CloseHandle(serial_handle);
#else
    close(serial_fd);
#endif

    return EXIT_SUCCESS;
}
