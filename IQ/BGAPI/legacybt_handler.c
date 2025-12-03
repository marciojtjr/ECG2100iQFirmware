#include "BT122_handler.h"
#include "bgapi_commands.h"
#include "bgapi_parser.h"
#include "../usart_driver.h"
#include "../taskUSB.h"
#include "../taskBT.h"
#include "../rtos.h"
#include "../eeprom.h"
#include "../iq.h"
#include <string.h>

#define RESET_BT_AND_WAIT() { readyForCommands = 0; print_bt("RESET\r"); while(readyForCommands == 0) { osWAIT(1); } }

// Initialize BT122 module and wait for it to be ready
// Returns 0 on success, -1 on timeout
int legacybt_handler_init(const char *device_name, int timeout_ms) {    
    debug_message("[BT] Bluetooth chip ready, initializing...\r\n");
    print_bt("SET CONTROL INIT SET CONTROL AUTOCALL\r");
    RESET_BT_AND_WAIT();

    // Set PIO5 as the CD pin
    print_bt("SET CONTROL CD 20 0\r");
    RESET_BT_AND_WAIT();

    // Make sure our name is our serial number
    print_bt("SET BT NAME %s\r", device_name);
    RESET_BT_AND_WAIT();
    print_bt("SET BT PAIRCOUNT 1\r");
    RESET_BT_AND_WAIT();

    print_bt("SET CONTROL CONFIG 0000 2000 2000 1980\r");
    RESET_BT_AND_WAIT();
    print_bt("SET PROFILE IAP PROD:\"ECG\" MF:\"Spaulding\" MDL:\"%d\" SN:\"%02d%02d%04d\" PROTO:\"com.bluegiga.iwrap\" SEED:\"4Y5WV3BJAJ\" APP:\"com.spauldingclinical.SpauldingECG\" FW:%s HW:%d.0.0 CAPS:0 RF:0 VCD:05 MODE0 PKT2K\r", MCU.JTAGUID, (int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000), VERSION, (int)EE_Rd(EE_BOOT_VERSION));
    osWAIT(250);
    RESET_BT_AND_WAIT();
    ignoreNextRFCOMM = 1;
    print_bt("SDP ADD 72243468-25ee-4025-8fe7-1edd2cf13cd2 SpauldingECGRFCOMM\r");
    osWAIT(250);

    volatile int lastConnection = EE_Rd(EE_LAST_CONNECTION_TYPE);

    if(lastConnection == CONNECTION_APPLE) {
        debug_message("[BT] Last connection: APPLE - setting IAP autocall\r\n");
        print_bt("SET CONTROL AUTOCALL 1101 1000 IAP\r");
    }
    else {
        debug_message("[BT] Last connection: UNKNOWN/ANDROID - clearing autocall\r\n");
        print_bt("SET CONTROL AUTOCALL\r");
    }
    return 0;  // Timeout
}
