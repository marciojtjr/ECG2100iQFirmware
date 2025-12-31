#ifndef BGAPI_COMMANDS_H
#define BGAPI_COMMANDS_H

#include <stdint.h>
#include "bgapi_parser.h"

// BGAPI Command builder structure
typedef struct {
    uint8_t buffer[256];
    uint16_t length;
} bgapi_command_t;

// System Commands
void bgapi_cmd_system_hello(bgapi_command_t *cmd);
void bgapi_cmd_system_reset(bgapi_command_t *cmd, uint8_t boot_in_dfu);
void bgapi_cmd_system_set_local_name(bgapi_command_t *cmd, const uint8_t *name, uint8_t name_len);
void bgapi_cmd_system_set_class_of_device(bgapi_command_t *cmd, uint32_t cod);

// BT GAP Commands (Bluetooth Gap for BT122)
void bgapi_cmd_bt_gap_set_mode(bgapi_command_t *cmd, uint8_t connectable, uint8_t discoverable, uint8_t limited);

// BT RFCOMM Commands  
void bgapi_cmd_bt_rfcomm_start_server(bgapi_command_t *cmd, uint8_t sdp_id, uint8_t streaming_destination);
void bgapi_cmd_bt_rfcomm_stop_server(bgapi_command_t *cmd, uint8_t sdp_id);

// BT SDP Commands
void bgapi_cmd_bt_sdp_load_service(bgapi_command_t *cmd, uint16_t ps_key);

// Security Manager Commands
void bgapi_cmd_sm_set_bondable_mode(bgapi_command_t *cmd, uint8_t bondable);
void bgapi_cmd_sm_configure(bgapi_command_t *cmd, uint8_t flags, uint8_t io_capabilities);

// Endpoint Commands
void bgapi_cmd_endpoint_send(bgapi_command_t *cmd, uint8_t endpoint, const uint8_t *data, uint8_t data_len);

// Legacy BLE GAP Commands (for compatibility)
void bgapi_cmd_gap_set_mode(bgapi_command_t *cmd, uint8_t discover, uint8_t connect);
void bgapi_cmd_gap_discover(bgapi_command_t *cmd, uint8_t mode);
void bgapi_cmd_gap_end_procedure(bgapi_command_t *cmd);
void bgapi_cmd_gap_connect_direct(bgapi_command_t *cmd, const uint8_t *address, uint8_t addr_type,
                                  uint16_t conn_interval_min, uint16_t conn_interval_max,
                                  uint16_t timeout, uint16_t latency);

// Connection Commands
void bgapi_cmd_connection_disconnect(bgapi_command_t *cmd, uint8_t connection);
void bgapi_cmd_connection_get_status(bgapi_command_t *cmd, uint8_t connection);

// Persistent Store Commands
void bgapi_cmd_ps_save(bgapi_command_t *cmd, uint16_t key, const uint8_t *value, uint8_t value_len);
void bgapi_cmd_ps_load(bgapi_command_t *cmd, uint16_t key);
void bgapi_cmd_ps_erase(bgapi_command_t *cmd, uint16_t key);

// Hardware Commands
void bgapi_cmd_hardware_set_uart_configuration(bgapi_command_t *cmd, uint8_t uart_index, uint32_t baud_rate, 
                                              uint8_t data_bits, uint8_t stop_bits, uint8_t parity, uint8_t flow_control);

// Helper: Parse UUID string to byte array
// Input: "72243468-25ee-4025-8fe7-1edd2cf13cd2"
// Output: {0x72,0x24,0x34,0x68,...}
// Returns 0 on success, -1 on parse error
int bgapi_parse_uuid(const char *uuid_string, uint8_t *uuid_bytes);

// Helper function to send command via bt FILE stream
int bgapi_send_command(const bgapi_command_t *cmd);

// Response tracking functions
void bgapi_reset_response_tracking(void);
int bgapi_is_response_received(void);
int bgapi_get_last_response_error(void);
void bgapi_handle_response(const bgapi_packet_t *packet);

// Event handlers
void bgapi_handle_endpoint_data(bgapi_packet_t *packet);

// Print command for debugging
void bgapi_print_command(const bgapi_command_t *cmd);

// Initialize BGAPI subsystem
void bgapi_init(void);

// Process incoming BGAPI data through parser
int process_bgapi_data(const unsigned char *buffer, int length, bgapi_packet_t *out_packet);

#endif // BGAPI_COMMANDS_H
