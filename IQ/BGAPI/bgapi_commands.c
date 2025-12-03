#include "bgapi_commands.h"
#include "bgapi_parser.h"
#include <string.h>
#include <stdio.h>
#include "../taskUSB.h"
#include "../usart_driver.h"
#include "../rtos.h"

// Response tracking variables
static volatile int waiting_for_response = 0;
static volatile int response_received = 0;
static volatile int last_response_error = 0;

// BGAPI parser state
static bgapi_parser_state_t parser_state;

// Helper to build BGAPI command header
static void build_command_header(bgapi_command_t *cmd, uint8_t class_id, uint8_t command_id, uint8_t payload_len) {
    cmd->buffer[0] = 0x20;                    // Command message type (0x20)
    cmd->buffer[1] = payload_len;             // Payload length
    cmd->buffer[2] = class_id;                // Class ID
    cmd->buffer[3] = command_id;              // Command ID
    cmd->length = 4 + payload_len;            // Total length
}

// Send command via bt FILE stream
int bgapi_send_command(const bgapi_command_t *cmd) {
    extern FILE *bt;  // Defined in TaskBT
    fwrite(cmd->buffer, 1, cmd->length, bt);
    return 0;
}

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
    print_usb("\n>>> Sending BGAPI Command (%d bytes):\n", cmd->length);
    print_usb("    ");
    for (int i = 0; i < cmd->length; i++) {
        print_usb("%02X ", cmd->buffer[i]);
        if ((i + 1) % 16 == 0 && (i + 1) < cmd->length) {
            print_usb("\n    ");
        }
    }
    print_usb("\n");
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
            print_usb(">>> Response: Class=0x%02X Cmd=0x%02X PayloadLen=%d\n", 
                   packet->class_id, packet->command_id, packet->payload_length);
            print_usb(">>> Raw payload: ");
            for (int i = 0; i < packet->payload_length && i < 8; i++) {
                print_usb("%02X ", packet->payload[i]);
            }
            print_usb("\n");
            
            if (error_code != 0) {
                print_usb("Command returned error code: 0x%04X\n", error_code);
            } else {
                print_usb("Command successful (result: 0x%04X)\n", error_code);
            }
        } else if (packet->payload_length == 1) {
            last_response_error = packet->payload[0];
            print_usb(">>> Response: Class=0x%02X Cmd=0x%02X PayloadLen=%d\n", 
                   packet->class_id, packet->command_id, packet->payload_length);
            print_usb(">>> Raw payload: %02X\n", packet->payload[0]);
            
            if (packet->payload[0] != 0) {
                print_usb("⚠️  Command returned error code: 0x%02X\n", packet->payload[0]);
            } else {
                print_usb("✅ Command successful (result: 0x%02X)\n", packet->payload[0]);
            }
        } else {
            last_response_error = 0;
            print_usb(">>> Response: Class=0x%02X Cmd=0x%02X PayloadLen=%d (no payload)\n", 
                   packet->class_id, packet->command_id, packet->payload_length);
            print_usb("Command successful (no error code)\n");
        }
    }
}

// Initialize BGAPI subsystem
void bgapi_init(void) {
    bgapi_parser_init(&parser_state);
    bgapi_reset_response_tracking();
}

// Process incoming BGAPI data through parser
// Returns 1 if packet found, 0 if need more data, -1 on error
int process_bgapi_data(const unsigned char *buffer, int length, bgapi_packet_t *out_packet) {
    static int remaining_offset = 0;

    // Feed data to parser
    int offset = 0;
    while (offset < length) {
        int result = bgapi_parser_feed(&parser_state, buffer + offset, length - offset, out_packet);

        if (result > 0) {
            // Found a complete packet
            bgapi_print_packet(out_packet);

            // Handle response packets (sets response_received flag)
            if (out_packet->message_type == BGAPI_MSG_TYPE_RESPONSE) {
                bgapi_handle_response(out_packet);
            }

            // Handle events and send responses
            //handle_bgapi_event(out_packet);

            // Move offset past the parsed packet
            offset += out_packet->total_length - remaining_offset;
            remaining_offset = 0;

            return 1;  // Packet found and returned
        } else {
            // No complete packet yet, need more data
            remaining_offset = parser_state.buffer_pos;
            break;
        }
    }

    return 0;  // Need more data
}

// Send command and wait for response (AVR firmware version)
// Sends the command and waits for response, actively processing incoming data
int bgapi_send_command_and_wait(bgapi_command_t *cmd, int timeout_ms) {
    // Circular buffer for interrupt-safe BT data access
    extern volatile uint8_t btDebugBuf[];
    extern volatile uint8_t btDebugHead;
    extern volatile uint8_t btDebugTail;

    bgapi_print_command(cmd);

    // Reset response tracking
    response_received = 0;
    waiting_for_response = 1;
    last_response_error = 0;

    // Send the command
    bgapi_send_command(cmd);

    // Wait for response with timeout, actively processing incoming data
    int elapsed = 0;
    bgapi_packet_t packet;

    while (!response_received && elapsed < timeout_ms) {
        // Process data from interrupt-safe circular buffer
        while(btDebugTail != btDebugHead) {
            unsigned char data = btDebugBuf[btDebugTail];
            btDebugTail = (btDebugTail + 1) & (BT_DEBUG_BUF_SIZE - 1);

            int result = process_bgapi_data(&data, 1, &packet);

            if (result > 0 && response_received) {
                waiting_for_response = 0;
                return last_response_error;
            }
        }

        osWAIT(10);  // Yield to other RTOS tasks for 10ms
        elapsed += 10;
    }

    waiting_for_response = 0;

    if (!response_received) {
        print_usb("BGAPI timeout waiting for response\n");
        return -1;  // Timeout
    }

    return last_response_error;
}
