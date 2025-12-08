#include "BT122_handler.h"
#include "bgapi_commands.h"
#include "bgapi_parser.h"
#include "../usart_driver.h"
#include "../taskUSB.h"
#include "../rtos.h"
#include <string.h>

// Initialize BT122 module and wait for it to be ready
// Returns 0 on success, -1 on timeout
int BT122_handler_init(const char *device_name, int timeout_ms) {
    int elapsed = 0;
    bgapi_packet_t packet;
    bgapi_command_t cmd;
    int result;
	int launchStep = 0;
	uint8_t service_uuid[16];
    debug_message("[BT122] Waiting for SYSTEM_INITIALIZED event...\r\n");
    while (readyForCommands == 0) {
        while(btDebugTail != btDebugHead) {
            unsigned char data = btDebugBuf[btDebugTail];
            btDebugTail = (btDebugTail + 1) & (BT_DEBUG_BUF_SIZE - 1);
            result = process_bgapi_data(&data, 1, &packet);
            if (result == 1) {
                if(packet.message_type == BGAPI_MSG_TYPE_EVENT
                    && packet.class_id == BGAPI_CLASS_SYSTEM
                    && packet.command_id == BGAPI_EVENT_SYSTEM_INITIALIZED 
					&& launchStep == 0) {
                    debug_message("[BT122] BOOT COMPLETED\r\n");

                    // STEP 1: Ensure we're NOT discoverable before changing name
                    bgapi_cmd_bt_gap_set_mode(&cmd, 0, 0, 0);  // Not connectable, not discoverable
                    bgapi_send_command(&cmd);
					launchStep = 1;
                    debug_message("[BT122] Stop advertising: %d\r\n", result);
				} else {
					bgapi_print_packet(&packet);
					switch(launchStep) {
						case 1:
							// Set device name
							bgapi_cmd_system_set_local_name(&cmd, (const uint8_t *)device_name, strlen(device_name));
							result = bgapi_send_command(&cmd);
							debug_message("[BT122] Set device name\r\n");
							launchStep = 2;
						break;
						case 2:
						    // Set class of device
						    bgapi_cmd_system_set_class_of_device(&cmd, 0x202);
						    result = bgapi_send_command(&cmd);
						    debug_message("[BT122] Set class of device\r\n");
							launchStep = 3;
						break;
						case 3:
						    // Set GAP mode (connectable, discoverable)
						    bgapi_cmd_bt_gap_set_mode(&cmd, 1, 1, 0);
						    result = bgapi_send_command(&cmd);
						    debug_message("[BT122] Set GAP mode\r\n");
							launchStep = 4;
						break;
						case 4:
						 	// configure as no input/no output + Encryption requires bonding
						 	bgapi_cmd_sm_configure(&cmd, 2, 3);
						 	result = bgapi_send_command(&cmd);
						 	debug_message("[BT122] Set configure\r\n");
							launchStep = 5;
						break;
						case 5:
						    // Set bondable mode
						    bgapi_cmd_sm_set_bondable_mode(&cmd, 1);
						    result = bgapi_send_command(&cmd);
						    debug_message("[BT122] Set bondable mode\r\n");
							launchStep = 6;
						break;
						case 6:
						    bgapi_cmd_bt_rfcomm_start_server(&cmd, 2, 0);
						    result = bgapi_send_command(&cmd);
						    debug_message("[BT122] Start RFCOMM server\r\n");
							launchStep = 7;
						break;
						case 7:
						    debug_message("[BT122] bgapi_cmd_hardware_set_uart_configuration\r\n");
						    readyForCommands = 1;
							bgapi_cmd_hardware_set_uart_configuration(&cmd, 0, 921600, 8, 1, 0, 1);
							bgapi_send_command(&cmd);
						    launchStep = 8;
						break;
						case 8:
							debug_message("[BT122] Initialization completed successfully\r\n");
							readyForCommands = 1;
							return 0;  // Success
						break;
					}
				}
            }
        }
        elapsed += 1;
    }
    print_usb("[BT122] Initialization timeout\r\n");
    return -1;  // Timeout
}

void BT122_handler_data_send(uint8_t endpoint, const uint8_t *data, uint8_t data_len) {
	bgapi_command_t cmd;
	bgapi_cmd_endpoint_send(&cmd, endpoint, data, data_len);
	bgapi_send_command(&cmd);
}

void BT122_handler_reset_to_discoverable(void) {
	bgapi_command_t cmd;
	
	// First ensure RFCOMM server is running
	bgapi_cmd_bt_rfcomm_start_server(&cmd, 2, 0);
	bgapi_send_command(&cmd);
	debug_message("[BT122] Restarted RFCOMM server\r\n");
	
	// Then set GAP mode to discoverable
	bgapi_cmd_bt_gap_set_mode(&cmd, 1, 1, 0);  // connectable, discoverable, not limited
	bgapi_send_command(&cmd);
	debug_message("[BT122] Reset to discoverable mode\r\n");
}
