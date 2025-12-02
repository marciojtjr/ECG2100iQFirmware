#ifndef BGAPI_COMMANDS_H
#define BGAPI_COMMANDS_H

#include <stdint.h>

// BGAPI Command builder structure
typedef struct {
    uint8_t buffer[256];
    uint16_t length;
} bgapi_command_t;

// System Commands
void bgapi_cmd_system_hello(bgapi_command_t *cmd);
void bgapi_cmd_system_reset(bgapi_command_t *cmd, uint8_t boot_in_dfu);
void bgapi_cmd_system_get_info(bgapi_command_t *cmd);
void bgapi_cmd_system_endpoint_tx(bgapi_command_t *cmd, uint8_t endpoint, const uint8_t *data, uint8_t data_len);

// GAP Commands (Generic Access Profile)
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
int bgapi_send_command(int fd, const bgapi_command_t *cmd);

// Print command for debugging
void bgapi_print_command(const bgapi_command_t *cmd);

#endif // BGAPI_COMMANDS_H
