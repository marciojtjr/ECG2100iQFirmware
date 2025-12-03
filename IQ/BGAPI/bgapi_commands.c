#include "bgapi_commands.h"
#include "bgapi_parser.h"
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
    #define usleep(x) Sleep((x)/1000)
#else
    #include <unistd.h>
    #include <errno.h>
#endif

// Response tracking variables
static volatile int waiting_for_response = 0;
static volatile int response_received = 0;
static volatile int last_response_error = 0;

// Forward declarations
void process_bgapi_data(const unsigned char *buffer, int length);
void handle_bgapi_event(const bgapi_packet_t *packet);

// Helper to build BGAPI command header
static void build_command_header(bgapi_command_t *cmd, uint8_t class_id, uint8_t command_id, uint8_t payload_len) {
    cmd->buffer[0] = 0x20;                    // Command message type (0x20)
    cmd->buffer[1] = payload_len;             // Payload length
    cmd->buffer[2] = class_id;                // Class ID
    cmd->buffer[3] = command_id;              // Command ID
    cmd->length = 4 + payload_len;            // Total length
}

// Send command
#ifdef _WIN32
int bgapi_send_command(HANDLE handle, const bgapi_command_t *cmd) {
    DWORD bytes_written;
    if (!WriteFile(handle, cmd->buffer, cmd->length, &bytes_written, NULL)) {
        return -1;
    }
    if (bytes_written != cmd->length) {
        return -1;
    }
    return 0;
}
#else
int bgapi_send_command(int fd, const bgapi_command_t *cmd) {
    int bytes_written = write(fd, cmd->buffer, cmd->length);
    if (bytes_written != cmd->length) {
        return -1;
    }
    return 0;
}
#endif

// System Commands

void bgapi_cmd_system_hello(bgapi_command_t *cmd) {
    build_command_header(cmd, 0x01, 0x00, 0);
}

void bgapi_cmd_system_reset(bgapi_command_t *cmd, uint8_t boot_in_dfu) {
    build_command_header(cmd, 0x01, 0x01, 1);
    cmd->buffer[4] = boot_in_dfu;
}

void bgapi_cmd_system_set_local_name(bgapi_command_t *cmd, const uint8_t *name, uint8_t name_len) {
    if (name_len > 247) name_len = 247;  // Max payload - header - length byte
    build_command_header(cmd, 0x01, 0x08, 1 + name_len);  // +1 for length byte
    cmd->buffer[4] = name_len;  // First byte is name length
    if (name && name_len > 0) {
        memcpy(&cmd->buffer[5], name, name_len);  // Name starts at offset 5
    }
}

void bgapi_cmd_system_set_class_of_device(bgapi_command_t *cmd, uint32_t cod) {
    build_command_header(cmd, 0x01, 0x06, 4);
    cmd->buffer[4] = cod & 0xFF;
    cmd->buffer[5] = (cod >> 8) & 0xFF;
    cmd->buffer[6] = (cod >> 16) & 0xFF;
    cmd->buffer[7] = (cod >> 24) & 0xFF;
}

// BT GAP Commands (Bluetooth Classic GAP for BT122)

void bgapi_cmd_bt_gap_set_mode(bgapi_command_t *cmd, uint8_t connectable, uint8_t discoverable, uint8_t limited) {
    build_command_header(cmd, 0x02, 0x03, 3);
    cmd->buffer[4] = connectable;
    cmd->buffer[5] = discoverable;
    cmd->buffer[6] = limited;
}

// BT RFCOMM Commands

void bgapi_cmd_bt_rfcomm_start_server(bgapi_command_t *cmd, uint8_t sdp_id, uint8_t streaming_destination) {
    build_command_header(cmd, 0x04, 0x01, 2);
    cmd->buffer[4] = sdp_id;
    cmd->buffer[5] = streaming_destination;
}

void bgapi_cmd_bt_rfcomm_stop_server(bgapi_command_t *cmd, uint8_t sdp_id) {
    build_command_header(cmd, 0x04, 0x03, 1);
    cmd->buffer[4] = sdp_id;
}

// Security Manager Commands

void bgapi_cmd_sm_set_bondable_mode(bgapi_command_t *cmd, uint8_t bondable) {
    build_command_header(cmd, 0x0f, 0x00, 1);
    cmd->buffer[4] = bondable;
}

// Endpoint Commands

void bgapi_cmd_endpoint_send(bgapi_command_t *cmd, uint8_t endpoint, const uint8_t *data, uint8_t data_len) {
    if (data_len > 251) data_len = 251;  // Max payload - header - endpoint
    build_command_header(cmd, 0x0b, 0x00, 1 + data_len);
    cmd->buffer[4] = endpoint;
    if (data && data_len > 0) {
        memcpy(&cmd->buffer[5], data, data_len);
    }
}

// Legacy BLE GAP Commands (for compatibility)

void bgapi_cmd_gap_set_mode(bgapi_command_t *cmd, uint8_t discover, uint8_t connect) {
    build_command_header(cmd, 0x06, 0x01, 2);
    cmd->buffer[4] = discover;
    cmd->buffer[5] = connect;
}

void bgapi_cmd_gap_discover(bgapi_command_t *cmd, uint8_t mode) {
    build_command_header(cmd, 0x06, 0x02, 1);
    cmd->buffer[4] = mode;
}

void bgapi_cmd_gap_end_procedure(bgapi_command_t *cmd) {
    build_command_header(cmd, 0x06, 0x04, 0);
}

void bgapi_cmd_gap_connect_direct(bgapi_command_t *cmd, const uint8_t *address, uint8_t addr_type,
                                  uint16_t conn_interval_min, uint16_t conn_interval_max,
                                  uint16_t timeout, uint16_t latency) {
    build_command_header(cmd, 0x06, 0x03, 15);
    if (address) {
        memcpy(&cmd->buffer[4], address, 6);
    }
    cmd->buffer[10] = addr_type;
    cmd->buffer[11] = conn_interval_min & 0xFF;
    cmd->buffer[12] = (conn_interval_min >> 8) & 0xFF;
    cmd->buffer[13] = conn_interval_max & 0xFF;
    cmd->buffer[14] = (conn_interval_max >> 8) & 0xFF;
    cmd->buffer[15] = timeout & 0xFF;
    cmd->buffer[16] = (timeout >> 8) & 0xFF;
    cmd->buffer[17] = latency & 0xFF;
    cmd->buffer[18] = (latency >> 8) & 0xFF;
}

// Connection Commands

void bgapi_cmd_connection_disconnect(bgapi_command_t *cmd, uint8_t connection) {
    build_command_header(cmd, 0x03, 0x00, 1);
    cmd->buffer[4] = connection;
}

void bgapi_cmd_connection_get_status(bgapi_command_t *cmd, uint8_t connection) {
    build_command_header(cmd, 0x03, 0x07, 1);
    cmd->buffer[4] = connection;
}

// Persistent Store Commands

void bgapi_cmd_ps_save(bgapi_command_t *cmd, uint16_t key, const uint8_t *value, uint8_t value_len) {
    if (value_len > 249) value_len = 249;  // Max payload - header - key - length
    build_command_header(cmd, 0x0d, 0x01, 3 + value_len);
    cmd->buffer[4] = key & 0xFF;
    cmd->buffer[5] = (key >> 8) & 0xFF;
    cmd->buffer[6] = value_len;
    if (value && value_len > 0) {
        memcpy(&cmd->buffer[7], value, value_len);
    }
}

void bgapi_cmd_ps_load(bgapi_command_t *cmd, uint16_t key) {
    build_command_header(cmd, 0x0d, 0x00, 2);
    cmd->buffer[4] = key & 0xFF;
    cmd->buffer[5] = (key >> 8) & 0xFF;
}

void bgapi_cmd_ps_erase(bgapi_command_t *cmd, uint16_t key) {
    build_command_header(cmd, 0x0d, 0x02, 2);
    cmd->buffer[4] = key & 0xFF;
    cmd->buffer[5] = (key >> 8) & 0xFF;
}

// Print command for debugging
void bgapi_print_command(const bgapi_command_t *cmd) {
    printf("\n>>> Sending BGAPI Command (%d bytes):\n", cmd->length);
    printf("    ");
    for (int i = 0; i < cmd->length; i++) {
        printf("%02X ", cmd->buffer[i]);
        if ((i + 1) % 16 == 0 && (i + 1) < cmd->length) {
            printf("\n    ");
        }
    }
    printf("\n");
}

// Response tracking functions
void bgapi_reset_response_tracking(void) {
    response_received = 0;
    waiting_for_response = 0;
    last_response_error = 0;
}

int bgapi_is_response_received(void) {
    return response_received;
}

int bgapi_get_last_response_error(void) {
    return last_response_error;
}

// Response handling - called by the main event handler
void bgapi_handle_response(const bgapi_packet_t *packet) {
    if (packet->message_type == BGAPI_MSG_TYPE_RESPONSE) {
        response_received = 1;
        
        // Most BGAPI responses start with a result/error code
        // The format depends on the command:
        // - Some have 2-byte result code (little-endian)
        // - Some have 1-byte result code
        // - Some have no result code (just success by receiving response)
        
        if (packet->payload_length >= 2) {
            // Try 2-byte little-endian error code first
            uint16_t error_code = packet->payload[0] | (packet->payload[1] << 8);
            last_response_error = error_code;
            
            // Print the response details for debugging
            printf(">>> Response: Class=0x%02X Cmd=0x%02X PayloadLen=%d\n", 
                   packet->class_id, packet->command_id, packet->payload_length);
            printf(">>> Raw payload: ");
            for (int i = 0; i < packet->payload_length && i < 8; i++) {
                printf("%02X ", packet->payload[i]);
            }
            printf("\n");
            
            if (error_code != 0) {
                printf("⚠️  Command returned error code: 0x%04X\n", error_code);
            } else {
                printf("✅ Command successful (result: 0x%04X)\n", error_code);
            }
        } else if (packet->payload_length == 1) {
            last_response_error = packet->payload[0];
            printf(">>> Response: Class=0x%02X Cmd=0x%02X PayloadLen=%d\n", 
                   packet->class_id, packet->command_id, packet->payload_length);
            printf(">>> Raw payload: %02X\n", packet->payload[0]);
            
            if (packet->payload[0] != 0) {
                printf("⚠️  Command returned error code: 0x%02X\n", packet->payload[0]);
            } else {
                printf("✅ Command successful (result: 0x%02X)\n", packet->payload[0]);
            }
        } else {
            last_response_error = 0;
            printf(">>> Response: Class=0x%02X Cmd=0x%02X PayloadLen=%d (no payload)\n", 
                   packet->class_id, packet->command_id, packet->payload_length);
            printf("✅ Command successful (no error code)\n");
        }
    }
}

#ifdef _WIN32
int bgapi_read_and_process_until_response(HANDLE handle, int timeout_ms) {
    unsigned char buffer[256];
    int bytes_read;
    int elapsed = 0;
    extern bgapi_parser_state_t parser_state; // Defined in main.c
    
    while (elapsed < timeout_ms && !response_received) {
        DWORD dwBytesRead;
        if (ReadFile(handle, buffer, sizeof(buffer) - 1, &dwBytesRead, NULL)) {
            bytes_read = (int)dwBytesRead;
        } else {
            bytes_read = -1;
        }

        if (bytes_read > 0) {
            // Process BGAPI packets - we need to call the parser from main
            process_bgapi_data(buffer, bytes_read);
            
            if (response_received) {
                return last_response_error;
            }
        } else if (bytes_read < 0) {
            DWORD error = GetLastError();
            if (error != ERROR_TIMEOUT) {
                printf("Error reading from serial port: %lu\n", error);
                return -1;
            }
        }
        
        usleep(10000); // 10ms
        elapsed += 10;
    }
    
    if (!response_received) {
        printf("⚠️  TIMEOUT waiting for response!\n");
        return -1;
    }
    
    return last_response_error;
}

int bgapi_send_command_and_wait(HANDLE handle, bgapi_command_t *cmd, int timeout_ms) {
    bgapi_print_command(cmd);
    
    // Reset response tracking
    response_received = 0;
    waiting_for_response = 1;
    last_response_error = 0;
    
    // Send the command
    bgapi_send_command(handle, cmd);

    // Read and process data until we get the response
    int result = bgapi_read_and_process_until_response(handle, timeout_ms);
    
    waiting_for_response = 0;
    return result;
}
#else
int bgapi_read_and_process_until_response(int fd, int timeout_ms) {
    unsigned char buffer[256];
    int bytes_read;
    int elapsed = 0;
    
    while (elapsed < timeout_ms && !response_received) {
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0) {
            // Process BGAPI packets
            process_bgapi_data(buffer, bytes_read);
            
            if (response_received) {
                return last_response_error;
            }
        } else if (bytes_read < 0) {
            if (errno != EAGAIN && errno != EINTR) {
                perror("Error reading from serial port");
                return -1;
            }
        }
        
        usleep(10000); // 10ms
        elapsed += 10;
    }
    
    if (!response_received) {
        printf("⚠️  TIMEOUT waiting for response!\n");
        return -1;
    }
    
    return last_response_error;
}

int bgapi_send_command_and_wait(int fd, bgapi_command_t *cmd, int timeout_ms) {
    bgapi_print_command(cmd);
    
    // Reset response tracking
    response_received = 0;
    waiting_for_response = 1;
    last_response_error = 0;
    
    // Send the command
    bgapi_send_command(fd, cmd);

    // Read and process data until we get the response
    int result = bgapi_read_and_process_until_response(fd, timeout_ms);
    
    waiting_for_response = 0;
    return result;
}
#endif
