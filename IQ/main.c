#ifdef BOOTLOADER

#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "avr_compiler.h"
#include "peanut.h"
#include "defines.h"
#include "serial.h"
#include "sp_driver.h"
#include "eeprom_driver.h"
#include "clksys_driver.h"
#include "seg7.h"
#include "eeprom.h"
#include "update_handler.h"

#define BOOT_VERSION 3	// the first version was actually 0xffffffff

/*If the code space is critical, user can uncomment the following lines*/
//#define REMOVE_AVRPROG_SUPPORT
//#define REMOVE_BLOCK_SUPPORT

#define LARGE_MEMORY

#ifdef LARGE_MEMORY
#  define ADDR_T unsigned long
#else  /* !LARGE_MEMORY */
#  define ADDR_T unsigned int
#endif /* LARGE_MEMORY */

long unsigned int AppCRC; // holds the CRC of the application flash after loading, will be programmed it EEPROM upon successful completion of an update

//-------------------------------------------------------------------------
void (*appVec)( void ) = 0x0000; // Set up function pointer to RESET vector.

/*	Seg7A(1); - top segment
	Seg7B(2); - top right segment
	Seg7C(0); - bottom right
	Seg7D(0); - bottom
	Seg7E(0); - bottom left
	Seg7F(0); - middle 
	Seg7G(0); - top left
	Seg7DP(0); - dot*/

/*void BootSeg7(char c) {
	Seg7A(0); Seg7B(0); Seg7C(0); Seg7D(0); Seg7E(0); Seg7F(0); Seg7G(0);
	switch(c) {
		case '0': Seg7A(1); Seg7B(1); Seg7C(1); Seg7D(1); Seg7E(1); Seg7G(1); break;
		case '1': Seg7B(1); Seg7C(1); break;
		case '2': Seg7A(1); Seg7B(1); Seg7D(1); Seg7E(1); Seg7F(1); break;
		case '3': Seg7A(1); Seg7B(1); Seg7C(1); Seg7D(1); Seg7F(1); break;
		case '4': Seg7B(1); Seg7C(1); Seg7F(1); Seg7G(1); break;
		case '5': Seg7A(1); Seg7C(1); Seg7D(1); Seg7F(1); Seg7G(1); break;
		case '6': Seg7A(1); Seg7C(1); Seg7D(1); Seg7E(1); Seg7F(1); Seg7G(1); break;
		case '7': Seg7A(1); Seg7B(1); Seg7C(1); break;
		case '8': Seg7A(1); Seg7B(1); Seg7C(1); Seg7D(1); Seg7E(1); Seg7F(1); Seg7G(1); break;
		case '9': Seg7A(1); Seg7B(1); Seg7C(1); Seg7D(1); Seg7F(1); Seg7G(1); break;
		case 'P': case 'p': Seg7A(1); Seg7B(1); Seg7E(1); Seg7F(1); Seg7G(1); break;
		case 'D': case 'd': Seg7B(1); Seg7C(1); Seg7D(1); Seg7E(1); Seg7G(1); break;
		case 'N': case 'n': Seg7C(1); Seg7E(1); Seg7F(1); break;
		case 'E': case 'e': Seg7A(1); Seg7D(1); Seg7E(1); Seg7F(1); Seg7G(1); break;
		case 'A': case 'a': Seg7A(1); Seg7B(1); Seg7C(1); Seg7E(1); Seg7F(1); Seg7G(1); break;
		case 'B': case 'b': Seg7C(1); Seg7D(1); Seg7E(1); Seg7F(1); Seg7G(1); break;
		case 'C': Seg7A(1); Seg7D(1); Seg7E(1); Seg7G(1); break;
		case 'c': Seg7D(1); Seg7E(1); Seg7F(1); break;
		case 'F': case 'f': Seg7A(1); Seg7E(1); Seg7F(1); Seg7G(1); break;
		case 'L': case 'l': Seg7D(1); Seg7E(1); Seg7G(1); break;
		case 'U': case 'u': Seg7B(1); Seg7C(1); Seg7D(1); Seg7E(1); Seg7G(1); break;
		case '.': Seg7DP(1); break;
		case '-': Seg7F(1); break;
	}
}*/

void gotoApp(void){
	SP_WaitForSPM();
	SP_LockSPM();
	EIND = 0x00;
	appVec();
}

void FastClock(void){
	//------------------------------------------------------------------------------------------
	// Set the CPU clock to 32MHz using an external 16MHz crystal and the PLL set to x2
	CLKSYS_XOSC_Config( OSC_FRQRANGE_12TO16_gc, false, OSC_XOSCSEL_XTAL_16KCLK_gc );	// setup for 16MHz external crystal
	CLKSYS_Enable( OSC_XOSCEN_bm );	// enable the external crystal oscillator
	while ( CLKSYS_IsReady( OSC_XOSCRDY_bm ) == 0 );	// wait till it's stable for use as the PLL input
	CLKSYS_PLL_Config( OSC_PLLSRC_XOSC_gc, 2 );	// drive the PLL from the external crystal, set multiplication to 2x
	CLKSYS_Enable( OSC_PLLEN_bm );	// enable the PLL
	CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc );	// set up the prescalers
	while ( CLKSYS_IsReady( OSC_PLLRDY_bm ) == 0 );	// wait for PLL to stabilize
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_PLL_gc );	// start clocking the system from it
	CLKSYS_Disable( OSC_RC2MEN_bm | OSC_RC32KEN_bm);
}

//==============================================================================================================
//==============================================================================================================
//==============================================================================================================
int main(void) {
	ADDR_T address = 0;
	volatile unsigned int temp_int = 0;
	volatile unsigned char cmd = 0;
	if(EE_Rd(EE_BOOT_VERSION)!= BOOT_VERSION){
		EE_Wr(EE_BOOT_VERSION, BOOT_VERSION);
	}

	// The bootstrap loader is entered after every reset because the reset vector fuse is set to BOOTSTRAP
	// Resets can be caused by a power up, watchdog timeout, brown-out, software reset or hardware reset
	// A powerup (and hence power-up reset) can be caused by either a press of the button or insertion of the USB cable, both normal occurances
	// A brown-out reset would be caused by a low battery. While this is not expected, it's not an indication of a firmware problem
	// A software reset should never occur, as the firmware doesn't make use of it
	// An external hardware reset should never occur because the reset line (part of the PDI interface) is not connected to anything

	// A watchdog reset is an indication of either a firmware problem, a glitch or an intentional watchdog timeout in the application from the "up"date USB command

	// If we don't see USB power, we could not have gotten a USB "up" command, so we go directly to the application

	// If examination of the reset status register (RST.STATUS) shows that we did NOT get a watchdog timeout, we jump to the application code
	// If the

	MainPowerOff(); // USB and/or button press will keep us from powering down, make sure we do power down if disconnected from USB or if the button is released
	InitButton();
	InitUSBDetect();	// prepare to look for USB power

	// blank the LED
	//BootSeg7('U');
	FastClock();

	if(SP_ApplicationCRC() == EE_Rd(EE_APP_CRC)){ // if the application is valid, see if we should run it
		if(!USBPresent()){	// if we're not being powered by USB, there's no way to update, so go to the app
			sendStr("Bootloader USB not present\r\n");
            gotoApp();
		}
		if( (RST.STATUS & RST_WDRF_bm)==0 ){ // if we did not get a watchdog reset, go to the application and let it handle the reset
			gotoApp();
		}
	}
	//-------------------------------------------------------------------------
	// We're here because of a watchdog timer reset that occurred while we were connected to USB
	// If we were reset by the watchdog, that means the application is either damaged, or has received a firmware update command and wants to
	// turn control over to us. If we do not detect USB power, we turn off. If there was a glitch in the application software that tripped the watchdog timer
	// a power cycle may clear it. If there was damage to the application, there's nothing we can do until we get connected to a PC via USB



	//-------------------------------------------------------------------------
	// start of bootloader

	EEPROM_FlushBuffer();
	EEPROM_DisableMapping();

	//initbootuart(19, -4); // 921600
	initbootuart(33, -1); //115200
	
	static char hexRecord[128] = {0};  // Buffer for complete hex record
	static uint8_t recordPos = 0;
	static uint8_t inFirmwareUpdate = 0;
	static uint8_t waitForCancel = 0;

	// Check if update was requested by app
	if(EE_Rd(EE_UPDATE_REQUESTED)) {
		EE_Wr(EE_UPDATE_REQUESTED, 0);  // Clear flag
		sendStr("Bootloader update mode\r\n");
	} else {
		sendStr("Bootloader running app\r\n");
		gotoApp();
	}
	while(1) {
		uint8_t byte = recchar();
		if (waitForCancel) {
			FW_CancelUpdate();
			waitForCancel = RTS();
			if (!waitForCancel) {
				sendStr("Update restarted \r\n");
			}
			send_xon();
		}
		// Start of new hex record
		if(!waitForCancel && byte == ':') {
			if(!inFirmwareUpdate) {
				//sendStr("Update started ...\r\n");
				FW_InitUpdate();  // Fresh start
				inFirmwareUpdate = 1;
			}
			recordPos = 0;  // Reset for new record
		}
		// If firmware update is active, collect hex record
		if(inFirmwareUpdate && recordPos < (sizeof(hexRecord) - 1)) {
			hexRecord[recordPos++] = byte;
			// End of hex record found
			if(byte == '\n') {
				send_xoff();  // Pause sender while processing
				// Null terminate the record
				hexRecord[recordPos] = '\0';
				// Remove \r if present
				if(recordPos > 1 && hexRecord[recordPos-2] == '\r') {
					hexRecord[recordPos-2] = '\0';
				}
				// Process the complete hex record
				if(FW_ParseHexRecord(hexRecord) != 0) {
					FW_CancelUpdate();
					flush_uart();  // Clear any garbage
					waitForCancel = 1;
					inFirmwareUpdate = 0;
					recordPos = 0;
				} else {
					sendchar('.');
					recordPos = 0;  // Reset for next record
					send_xon();  // Resume sender
				}
			}
				
			// Check if firmware update completed
			if(!waitForCancel && !FW_IsUpdateActive()) {
				sendStr("App start\r\n");
				gotoApp();
				inFirmwareUpdate = 0;
				recordPos = 0;
			}
		}
	}
}
#endif