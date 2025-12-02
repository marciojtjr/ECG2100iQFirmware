#include "bgapi_commands.h"
#include "bgapi_parser.h"
#include "../usart_driver.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

extern USART_desc_t BTPortDescriptor;

// Helper to build BGAPI command header
static void build_command_header(bgapi_command_t *cmd, uint8_t class_id, uint8_t command_id, uint8_t payload_len) {
    cmd->buffer[0] = BGAPI_MSG_TYPE_COMMAND;  // 0x00 for command
    cmd->buffer[1] = payload_len;             // Payload length
    cmd->buffer[2] = class_id;                // Class ID
    cmd->buffer[3] = command_id;              // Command ID
    cmd->length = 4 + payload_len;            // Total length
}

// System Commands

void bgapi_cmd_system_hello(bgapi_command_t *cmd) {
    build_command_header(cmd, BGAPI_CLASS_SYSTEM, 0x00, 0);
}

void bgapi_cmd_system_reset(bgapi_command_t *cmd, uint8_t boot_in_dfu) {
    build_command_header(cmd, BGAPI_CLASS_SYSTEM, 0x01, 1);
    cmd->buffer[4] = boot_in_dfu;
    cmd->length = 5;
}

void bgapi_cmd_system_get_info(bgapi_command_t *cmd) {
    build_command_header(cmd, BGAPI_CLASS_SYSTEM, 0x08, 0);
}

void bgapi_cmd_system_endpoint_tx(bgapi_command_t *cmd, uint8_t endpoint, const uint8_t *data, uint8_t data_len) {
    build_command_header(cmd, BGAPI_CLASS_SYSTEM, 0x09, 2 + data_len);
    cmd->buffer[4] = endpoint;
    cmd->buffer[5] = data_len;
    if (data && data_len > 0) {
        memcpy(&cmd->buffer[6], data, data_len);
    }
    cmd->length = 6 + data_len;
}

// GAP Commands

void bgapi_cmd_gap_set_mode(bgapi_command_t *cmd, uint8_t discover, uint8_t connect) {
    build_command_header(cmd, BGAPI_CLASS_GAP, 0x01, 2);
    cmd->buffer[4] = discover;
    cmd->buffer[5] = connect;
}

void bgapi_cmd_gap_discover(bgapi_command_t *cmd, uint8_t mode) {
    build_command_header(cmd, BGAPI_CLASS_GAP, 0x02, 1);
    cmd->buffer[4] = mode;
}

void bgapi_cmd_gap_end_procedure(bgapi_command_t *cmd) {
    build_command_header(cmd, BGAPI_CLASS_GAP, 0x04, 0);
}

void bgapi_cmd_gap_connect_direct(bgapi_command_t *cmd, const uint8_t *address, uint8_t addr_type,
                                  uint16_t conn_interval_min, uint16_t conn_interval_max,
                                  uint16_t timeout, uint16_t latency) {
    build_command_header(cmd, BGAPI_CLASS_GAP, 0x03, 15);
    memcpy(&cmd->buffer[4], address, 6);  // BD address
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
    build_command_header(cmd, BGAPI_CLASS_CONNECTION, 0x00, 1);
    cmd->buffer[4] = connection;
}

void bgapi_cmd_connection_get_status(bgapi_command_t *cmd, uint8_t connection) {
    build_command_header(cmd, BGAPI_CLASS_CONNECTION, 0x07, 1);
    cmd->buffer[4] = connection;
}

// Persistent Store Commands

void bgapi_cmd_ps_save(bgapi_command_t *cmd, uint16_t key, const uint8_t *value, uint8_t value_len) {
    build_command_header(cmd, BGAPI_CLASS_PERSISTENT, 0x01, 3 + value_len);
    cmd->buffer[4] = key & 0xFF;
    cmd->buffer[5] = (key >> 8) & 0xFF;
    cmd->buffer[6] = value_len;
    if (value && value_len > 0) {
        memcpy(&cmd->buffer[7], value, value_len);
    }
    cmd->length = 7 + value_len;
}

void bgapi_cmd_ps_load(bgapi_command_t *cmd, uint16_t key) {
    build_command_header(cmd, BGAPI_CLASS_PERSISTENT, 0x00, 2);
    cmd->buffer[4] = key & 0xFF;
    cmd->buffer[5] = (key >> 8) & 0xFF;
}

void bgapi_cmd_ps_erase(bgapi_command_t *cmd, uint16_t key) {
    build_command_header(cmd, BGAPI_CLASS_PERSISTENT, 0x02, 2);
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

void bgapi_cmd_set_device_name(bgapi_command_t *cmd, const char *name) {
    uint8_t name_data[20];
    int name_len = strlen(name);
    if (name_len > 20) name_len = 20;
    memcpy(name_data, name, name_len);

    print_usb(">>> Setting Device Name: %s\n", name);
    // Write device name to PS store (key 0x8000 for local name)
    bgapi_cmd_ps_save(cmd, 0x8000, name_data, name_len);
}

// Send BGAPI command to Bluetooth module with flow control
void bgapi_send_command(const bgapi_command_t *cmd) {
    uint16_t timeout;

    if (cmd == NULL || cmd->length == 0) {
        return;
    }

    // Send each byte of the command buffer
    for (uint16_t i = 0; i < cmd->length; i++) {
        // Wait until TX buffer has space (prevent overflow)
        timeout = 10000;
        while(BTPortDescriptor.TxWaiting >= (BTPortDescriptor.TxBufSize - 1) && timeout > 0) {
            timeout--;
            // TX buffer is full, wait for space
        }

        if(timeout == 0) {
            print_usb("[BGAPI] TX buffer timeout on byte %d\r\n", i);
            return;
        }

        uart_putchar(&BTPortDescriptor, cmd->buffer[i]);
    }

    // Wait for entire command to be transmitted
    timeout = 50000;
    while(BTPortDescriptor.TxWaiting > 0 && timeout > 0) {
        timeout--;
    }

    if(timeout == 0) {
        print_usb("[BGAPI] TX completion timeout\r\n");
    }
}

