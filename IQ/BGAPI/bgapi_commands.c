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
    debug_message("[BGAPI_SEND] Sending %d bytes to BT122\n", cmd->length);
    fwrite(cmd->buffer, 1, cmd->length, bt);
    debug_message("[BGAPI_SEND] Command sent successfully\n");
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

// BT SDP Commands

void bgapi_cmd_bt_sdp_load_service(bgapi_command_t *cmd, uint16_t ps_key) {
    build_command_header(cmd, 0x05, 0x00, 2);
    cmd->buffer[4] = ps_key & 0xFF;
    cmd->buffer[5] = (ps_key >> 8) & 0xFF;
}

// Security Manager Commands

void bgapi_cmd_sm_set_bondable_mode(bgapi_command_t *cmd, uint8_t bondable) {
    build_command_header(cmd, 0x0f, 0x00, 1);
    cmd->buffer[4] = bondable;
}

void bgapi_cmd_sm_configure(bgapi_command_t *cmd, uint8_t flags, uint8_t io_capabilities) {
    build_command_header(cmd, 0x0f, 0x01, 2);
    cmd->buffer[4] = flags;
    cmd->buffer[5] = io_capabilities;
}

// Endpoint Commands

void bgapi_cmd_endpoint_send(bgapi_command_t *cmd, uint8_t endpoint, const uint8_t *data, uint8_t data_len) {
    if (data_len > 249) data_len = 249;  // Max payload - header - channel(1) - length(1)
    build_command_header(cmd, 0x0b, 0x00, 1 + 1 + data_len);  // channel(1) + length(1) + data
    cmd->buffer[4] = endpoint;    // Channel/Connection
    cmd->buffer[5] = data_len;    // Data length
    if (data && data_len > 0) {
        memcpy(&cmd->buffer[6], data, data_len);
    }
    
    // Print command buffer for debugging
    debug_message("[ENDPOINT_SEND] Command buffer (%d bytes): ", cmd->length);
    for (int i = 0; i < cmd->length; i++) {
        debug_message("%02X ", cmd->buffer[i]);
    }
    debug_message("\n");
    debug_message("[ENDPOINT_SEND] Data payload (%d bytes): ", data_len);
    for (int i = 0; i < data_len; i++) {
        debug_message("%02X ", data[i]);
    }
    debug_message("\n");
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

// Hardware Commands

void bgapi_cmd_hardware_set_uart_configuration(bgapi_command_t *cmd, uint8_t uart_index, uint32_t baud_rate, 
                                              uint8_t data_bits, uint8_t stop_bits, uint8_t parity, uint8_t flow_control) {
    build_command_header(cmd, 0x0c, 0x08, 9);
    cmd->buffer[4] = uart_index;
    cmd->buffer[5] = baud_rate & 0xFF;
    cmd->buffer[6] = (baud_rate >> 8) & 0xFF;
    cmd->buffer[7] = (baud_rate >> 16) & 0xFF;
    cmd->buffer[8] = (baud_rate >> 24) & 0xFF;
    cmd->buffer[9] = data_bits;
    cmd->buffer[10] = stop_bits;
    cmd->buffer[11] = parity;
    cmd->buffer[12] = flow_control;
}

// Helper: Parse UUID string to byte array
// Input: "72243468-25ee-4025-8fe7-1edd2cf13cd2" (with or without dashes)
// Output: uuid_bytes[16] = {0x72,0x24,0x34,0x68,...}
// Returns 0 on success, -1 on error
int bgapi_parse_uuid(const char *uuid_string, uint8_t *uuid_bytes) {
    int byte_index = 0;
    int i = 0;

    while (uuid_string[i] != '\0' && byte_index < 16) {
        char c = uuid_string[i];

        // Skip dashes
        if (c == '-') {
            i++;
            continue;
        }

        // Parse hex digit
        uint8_t high, low;

        // High nibble
        if (c >= '0' && c <= '9') high = c - '0';
        else if (c >= 'a' && c <= 'f') high = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') high = c - 'A' + 10;
        else return -1;  // Invalid character

        i++;
        c = uuid_string[i];

        // Low nibble
        if (c >= '0' && c <= '9') low = c - '0';
        else if (c >= 'a' && c <= 'f') low = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') low = c - 'A' + 10;
        else return -1;  // Invalid character

        uuid_bytes[byte_index++] = (high << 4) | low;
        i++;
    }

    return (byte_index == 16) ? 0 : -1;
}

// Print command for debugging
void bgapi_print_command(const bgapi_command_t *cmd) {
    debug_message("\n>>> Sending BGAPI Command (%d bytes):\n", cmd->length);
    debug_message("    ");
    for (int i = 0; i < cmd->length; i++) {
        debug_message("%02X ", cmd->buffer[i]);
        if ((i + 1) % 16 == 0 && (i + 1) < cmd->length) {
            debug_message("\n    ");
        }
    }
    debug_message("\n");
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
    if (packet->message_type == BGAPI_MSG_TYPE_RESPONSE && waiting_for_response) {
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
            debug_message(">>> Response: Class=0x%02X Cmd=0x%02X PayloadLen=%d\n", 
                   packet->class_id, packet->command_id, packet->payload_length);
            debug_message(">>> Raw payload: ");
            for (int i = 0; i < packet->payload_length && i < 8; i++) {
                debug_message("%02X ", packet->payload[i]);
            }
            debug_message("\n");
            
            if (error_code != 0) {
                debug_message("Command returned error code: 0x%04X\n", error_code);
            } else {
                debug_message("Command successful (result: 0x%04X)\n", error_code);
            }
        } else if (packet->payload_length == 1) {
            last_response_error = packet->payload[0];
            debug_message(">>> Response: Class=0x%02X Cmd=0x%02X PayloadLen=%d\n", 
                   packet->class_id, packet->command_id, packet->payload_length);
            debug_message(">>> Raw payload: %02X\n", packet->payload[0]);
            
            if (packet->payload[0] != 0) {
                debug_message("Command returned error code: 0x%02X\n", packet->payload[0]);
            } else {
                debug_message("Command successful (result: 0x%02X)\n", packet->payload[0]);
            }
        } else {
            last_response_error = 0;
            debug_message(">>> Response: Class=0x%02X Cmd=0x%02X PayloadLen=%d (no payload)\n", 
                   packet->class_id, packet->command_id, packet->payload_length);
            debug_message("Command successful (no error code)\n");
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
            if (out_packet->message_type == BGAPI_MSG_TYPE_EVENT) {
                if (out_packet->class_id == BGAPI_CLASS_ENDPOINT && 
                    (out_packet->command_id == BGAPI_EVENT_ENDPOINT_DATA || 
                     out_packet->command_id == BGAPI_EVENT_ENDPOINT_DATA_ALT)) {
                    bgapi_handle_endpoint_data(out_packet);
                }
            }

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

// Handle endpoint data events
// Packet format: endpoint (1 byte) + data_len (1 byte) + data (data_len bytes)
// Example: 0xa0050b0106033f730d -> endpoint=6, data_len=3, data=3f730d
void bgapi_handle_endpoint_data(bgapi_packet_t *packet) {
    if (packet->payload_length < 2) {
        debug_message("[ENDPOINT] Invalid endpoint data event - payload too short\n");
        return;
    }

    uint8_t endpoint = packet->payload[0];
    uint8_t data_len = packet->payload[1];
    
    // Save endpoint to packet structure
    packet->endpoint = endpoint;
    
    if (packet->payload_length < (2 + data_len)) {
        debug_message("[ENDPOINT] Invalid endpoint data event - data length mismatch\n");
        return;
    }

    // Move the actual data to the beginning of the payload, removing endpoint and data_len
    if (data_len > 0) {
        memmove(packet->payload, &packet->payload[2], data_len);
        packet->payload_length = data_len;
    } else {
        packet->payload_length = 0;
    }

    debug_message("[ENDPOINT] Data received on endpoint %d (%d bytes): ", endpoint, data_len);
    for (int i = 0; i < data_len; i++) {
        debug_message("%02x", packet->payload[i]);
    }
    debug_message("\n");
}
