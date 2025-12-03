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
    debug_message("[BT122] Waiting for SYSTEM_INITIALIZED event...\r\n");
    while (readyForCommands == 0) {
        while(btDebugTail != btDebugHead) {
            unsigned char data = btDebugBuf[btDebugTail];
            btDebugTail = (btDebugTail + 1) & (BT_DEBUG_BUF_SIZE - 1);
            result = process_bgapi_data(&data, 1, &packet);
            if (result == 1) {
                if(packet.message_type == BGAPI_MSG_TYPE_EVENT
                    && packet.class_id == BGAPI_CLASS_SYSTEM
                    && packet.command_id == BGAPI_EVENT_SYSTEM_INITIALIZED) {

                    debug_message("[BT122] BOOT COMPLETED\r\n");

                    // STEP 1: Ensure we're NOT discoverable before changing name
                    bgapi_cmd_bt_gap_set_mode(&cmd, 0, 0, 0);  // Not connectable, not discoverable
                    result = bgapi_send_command_and_wait(&cmd, 1000);
                    debug_message("[BT122] Stop advertising: %d\r\n", result);
                    
                    // Set device name
                    bgapi_cmd_system_set_local_name(&cmd, (const uint8_t *)device_name, strlen(device_name));
                    result = bgapi_send_command_and_wait(&cmd, 1000);
                    debug_message("[BT122] Set device name: %d\r\n", result);

                    // Set class of device
                    bgapi_cmd_system_set_class_of_device(&cmd, 0x202);
                    result = bgapi_send_command_and_wait(&cmd, 1000);
                    debug_message("[BT122] Set class of device: %d\r\n", result);

                    // Set GAP mode (connectable, discoverable)
                    bgapi_cmd_bt_gap_set_mode(&cmd, 1, 1, 0);
                    result = bgapi_send_command_and_wait(&cmd, 1000);
                    debug_message("[BT122] Set GAP mode: %d\r\n", result);

                    // Set bondable mode
                    bgapi_cmd_sm_set_bondable_mode(&cmd, 1);
                    result = bgapi_send_command_and_wait(&cmd, 1000);
                    debug_message("[BT122] Set bondable mode: %d\r\n", result);

                    // Add SDP service record - can use string format!
                    uint8_t service_uuid[16];
                    if (bgapi_parse_uuid("72243468-25ee-4025-8fe7-1edd2cf13cd2", service_uuid) == 0) {
                        result = bgapi_sdp_add_rfcomm_service(service_uuid, "SpauldingECGRFCOMM", 2, 1);
                        debug_message("[BT122] Add SDP service: %d\r\n", result);
                    } else {
                        debug_message("[BT122] UUID parse error\r\n");
                    }

                    // Start RFCOMM server (must be after SDP record is added)
                    bgapi_cmd_bt_rfcomm_start_server(&cmd, 2, 0);
                    result = bgapi_send_command_and_wait(&cmd, 1000);
                    debug_message("[BT122] Start RFCOMM server: %d\r\n", result);

                    debug_message("[BT122] Initialization completed successfully\r\n");
                    return 0;  // Success
                }
            }
        }
        elapsed += 1;
    }
    print_usb("[BT122] Initialization timeout\r\n");
    return -1;  // Timeout
}
