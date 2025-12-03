#ifndef BGAPI_COMMANDS_H
#define BGAPI_COMMANDS_H

#include <stdint.h>
#include "bgapi_parser.h"
#include "../taskBT.h"

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

// Security Manager Commands
void bgapi_cmd_sm_set_bondable_mode(bgapi_command_t *cmd, uint8_t bondable);

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

// Helper function to send command
/*int bgapi_send_command(const bgapi_command_t *cmd);

// Command sending with response waiting
int bgapi_send_command_and_wait(bgapi_command_t *cmd, int timeout_ms);
int bgapi_read_and_process_until_response(int timeout_ms);*/

// Response tracking functions
void bgapi_reset_response_tracking(void);
int bgapi_is_response_received(void);
int bgapi_get_last_response_error(void);
void bgapi_handle_response(const bgapi_packet_t *packet);

// Print command for debugging
void bgapi_print_command(const bgapi_command_t *cmd);

#endif // BGAPI_COMMANDS_H
