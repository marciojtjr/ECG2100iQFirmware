#ifndef BOOTLOADER

#include <avr/io.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <math.h>
#include <avr/sleep.h>

#include "rtos.h"

#include "iq.h"
#include "avr_compiler.h"
#include "TC_driver.h"
#include "clksys_driver.h"
#include "wdt_driver.h"
#include "usart_driver.h"
#include "eeprom_driver.h"
#include "adc_driver.h"
#include "buffers.h"
#include "taskAcq.h"
#include "eeprom.h"
#include "ulaw.h"
#include "crc.h"
#include "seg7.h"
#include "TaskWatchDog.h"
#include "taskBT.h"
#include "taskUSB.h"
#include "taskCC.h"
#include "apple.h"

#ifdef WORKING
#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
(byte & 0x80 ? 1 : 0), \
(byte & 0x40 ? 1 : 0), \
(byte & 0x20 ? 1 : 0), \
(byte & 0x10 ? 1 : 0), \
(byte & 0x08 ? 1 : 0), \
(byte & 0x04 ? 1 : 0), \
(byte & 0x02 ? 1 : 0), \
(byte & 0x01 ? 1 : 0)
#endif

#include "update_handler.h"

#define USB_USART				USARTF0
#define USB_USART_PORT	PORTF

enum { FORMAT_DECIMAL, FORMAT_BINARY }; // various file output formats

//##########################################################################################
// define and allocate globals for the USB task

USART_desc_t USBPortDescriptor;	// a descriptor for the USART connected to the USB bridge chip

static int put_usb( char c, FILE *stream){
	uart_putchar(&USBPortDescriptor,c);
	return(0); 
}

#define put_byte(c) uart_putchar(&USBPortDescriptor,c)
 
FILE usb_str = FDEV_SETUP_STREAM(put_usb, 0, _FDEV_SETUP_WRITE);
FILE *usb;

const unsigned char Base64[64] PROGMEM = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
unsigned char Verbose = 0;	// used to control whether print_debug() messages are printed or ignored
unsigned char Debug = 0; // used to control print_debug() statements
unsigned char FlowControl;
unsigned char EraseInProgress = 0;

// macro to make printing stuff a little easier
#define print_usb(fmt,...)  fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)
//#define print_debug(fmt,...) if(Debug) fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)
#define print_verbose(fmt,...) if(Verbose) fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)

long int result;
unsigned char usbConfigured = 0;

/*
//------------------------------------------------------------------------------------
// Unpack ECG data using array math
void UnPackECGA(signed int out[], unsigned char in[]){
  unsigned char i;	// index into input array
  unsigned char j;	// index into output array

  for(i=0, j=0; i<12; i+=3){	// unpack the eight channels in four pairs (3 bytes unpack into four bytes (two shorts))
    out[j++] = ( (in[i  ]<<8) + ( (in[i+1]   )&0xf0) )/16;
    out[j++] = ( (in[i+2]<<8) + ( (in[i+1]<<4)&0xf0) )/16;
  }
}

//------------------------------------------------------------------------------------
// Unpack ECG data using pointer math
void UnPackECGP(signed int *out, unsigned char *in){
	unsigned char i;
	signed int temp;

	for(i=0;i<4;i++){						// unpack the eight channels in four pairs (3 bytes unpack into four)
		temp = *in++<<8; 					// high byte of first channel in a pair
		temp += *in & 0xf0;				// low nibble
		*out++ = temp>>4;					// scale back to 12 bits
		temp = (*in++ & 0xf)<<4;	// low nibble of second sample
		temp += *in++<<8;					// high byte of second channel in a pair
		*out++ = temp>>4;					// scale back to 12 bits
	}
}
*/

//static unsigned char format;
//static unsigned int crc;
static unsigned int i;
static unsigned char j;

/*
void Dump(unsigned int i){
	unsigned char j;

	if(format == FORMAT_DECIMAL){
		for(j=0;j<12;j++){
			print_usb("%02X ",Buf1.Data.ECG.Sample[i][j]);
		}
		print_usb("\r\n");
	}
	else{ // FORMAT_BINARY
		for(j=0;j<12;j++){
			if(Debug){
				put_byte(j);
				crc(j);
			}
			else{
				put_byte(Buf1.Data.ECG.Sample[i][j]);
				crc(Buf1.Data.ECG.Sample[i][j]);
			}
		}
	}
}
*/

//------------------------------------------------------------------------------------
void Hello(int type, bool isBT122, int bt122_endpoint){
    sprintf(AppleBTString, "\r\n\r\nSpaulding Electrocardiograph BV:%d FV:%s HV:%d SN:9-%02d-%02d-%04d-101\r\n", (int)EE_Rd(EE_BOOT_VERSION), VERSION, MCU.JTAGUID, (int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000));
	j = strlen(AppleBTString);
	if(type == PRINT_USB) {
		print_usb("%s", AppleBTString);
	}
	else if(type == PRINT_BT) {
        if (isBT122)
            BT122_handler_data_send(bt122_endpoint, (uint8_t*)AppleBTString, j);
        else
            print_bt("%s", AppleBTString);
	}
	else if(type == PRINT_BT_APPLE) {
		appleCrc = 0;
		for(i = 0; i < j; i++) {
			appleCrc += AppleBTString[i];
		}

		sendECGPacketWithLength(j);
		//DPLEDTog();
	}
}

//==========================================================================================
// USB USART Configuration Function
// baudrate: desired baudrate (e.g., 115200, 921600)
void ConfigureUSBUSART(unsigned long baudrate)
{
	if(!usbConfigured) {
		int bsel, bscale;

		// Calculate BSEL and BSCALE for common baudrates at 32MHz
		if(baudrate == 115200) {
			bsel = 33;
			bscale = -1;
		} else if(baudrate == 921600) {
			bsel = 19;
			bscale = -4;
		} else {
			// Default to 115200 if unknown baudrate
			bsel = 33;
			bscale = -1;
		}

		// Set up USB descriptor buffers first
		USBPortDescriptor.TxBuf = smallTxBuf;
		USBPortDescriptor.RxBuf = smallRxBuf;
		USBPortDescriptor.TxBufSize = SMALL_TX_BUF_SIZE;
		USBPortDescriptor.RxBufSize = SMALL_RX_BUF_SIZE;
		USBPortDescriptor.TxBufMask = SMALL_TX_BUF_MASK;

		usb = &usb_str;
		USB_USART_PORT.DIRSET = PIN3_bm;  USB_USART_PORT.PIN3CTRL = PORT_OPC_WIREDAND_gc; // TXD is open collector output
		USB_USART_PORT.DIRCLR = PIN2_bm;// RXD is input

		USART_InterruptDriver_Initialize(&USBPortDescriptor, &USB_USART, USART_DREINTLVL_HI_gc);
		USART_RxdInterruptLevel_Set(USBPortDescriptor.usart, USART_RXCINTLVL_HI_gc);
		USART_Format_Set(USBPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N2

		USART_Baudrate_Set(USBPortDescriptor.usart, bsel, bscale);
		USART_Rx_Enable(USBPortDescriptor.usart);
		USART_Tx_Enable(USBPortDescriptor.usart);
		usbConfigured = 1;
		// Wait for USART to be ready
		osWAIT(10);
		print_usb("\r\n[USB] USB USART initialized at %lu baud\r\n", baudrate);
	}
}

//==========================================================================================
// USB Task
BEGIN_TASK(TaskUSB)
	//osENABLE_TASK(TaskFlash);	// we'll need Flash access

	//------------------------------------------------------------------------------------------
	// prepare the USB USART
	if(!usbConfigured) {
		ConfigureUSBUSART(115200);
		usbConfigured = 1;
	}


	//==========================================================================================
	// USB command parser


	static unsigned char argC;
	static char cmd[3];
	static float data1,data2;
	static unsigned long start;//, end;
	//static unsigned char byte;
	
	//static float ecgLength;
	//static unsigned long trueEnd;
	//static int trueEndPage;

	Hello(PRINT_USB, false, -1);

	static char hexRecord[128] = {0};  // Buffer for complete hex record (increased for safety)
	static uint16_t recordPos = 0;
	static uint8_t inFirmwareUpdate = 0;
	static uint8_t firstRun = 1;
	
	// Initialize on first run to prevent garbage values
	if(firstRun) {
		firstRun = 0;
		recordPos = 0;
		inFirmwareUpdate = 0;
		memset(hexRecord, 0, sizeof(hexRecord));
		// Ensure btDebug pointers are valid
		btDebugTail = btDebugTail % BT_DEBUG_BUF_SIZE;
		btDebugHead = btDebugHead % BT_DEBUG_BUF_SIZE;
	}
	
	//update test
	/*while(USBPresent()) {
		// Validate buffer pointers before processing
		if(btDebugTail >= BT_DEBUG_BUF_SIZE) {
			btDebugTail = 0;
		}
		if(btDebugHead >= BT_DEBUG_BUF_SIZE) {
			btDebugHead = 0;
		}
		
		while(btDebugTail != btDebugHead) {
			uint8_t byte = btDebugBuf[btDebugTail];
			
			// Start of new hex record
			if(byte == ':') {
				if(!inFirmwareUpdate && !FW_IsUpdateActive()) {
					FW_InitUpdate();
					inFirmwareUpdate = 1;
				}
				recordPos = 0;  // Reset for new record
			}
			// If firmware update is active, collect hex record (but skip if we just added ':')
			if(inFirmwareUpdate && recordPos < (sizeof(hexRecord) - 1)) {
				hexRecord[recordPos++] = byte;
				
				// End of hex record found
				if(byte == '\n') {
					// Null terminate the record
					hexRecord[recordPos] = '\0';
					
					// Remove \r if present
					if(recordPos > 1 && hexRecord[recordPos-2] == '\r') {
						hexRecord[recordPos-2] = '\0';
					}
					
					// Process the complete hex record
					if(FW_ParseHexRecord(hexRecord) != 0) {
						print_usb("Error processing hex record: %s\r\n", hexRecord);
						FW_CancelUpdate();
						inFirmwareUpdate = 0;
					}
					
					recordPos = 0;  // Reset for next record
				}
				
				// Check if firmware update completed
				if(!FW_IsUpdateActive()) {
					inFirmwareUpdate = 0;
					recordPos = 0;
				}
			}
			
			// Safe increment with bounds checking
			btDebugTail++;
			if(btDebugTail >= BT_DEBUG_BUF_SIZE) {
				btDebugTail = 0;
			}
		}
		osWAIT(1);
	}*/

	while(USBPresent()){
		//DPLEDOn();
		//static int startPage, startIndex, endPage, endIndex;

		USBWatchTimer = FLASH_WATCH_TIME; // longest command is erase all flash "ea" which can take up to 80 seconds, according to the data sheet.

		AnalogPowerOn();

		if(USBPortDescriptor.RxReady){	// we have a command
			//--------------------------------------------------------------------------
			// Auto-detect Intel HEX file by first character ':'
			if(USBPortDescriptor.RxBuf[0] == ':' && !FW_IsUpdateActive()) {
				// Detected Intel HEX file - automatically start firmware update
				FW_InitUpdate();
			}
			
			//--------------------------------------------------------------------------
			// If firmware update is active, process all data through packet handler
			if(FW_IsUpdateActive()) {
				// Get the length of received data (may contain null bytes)
				//uint16_t rxLength = strlen((const char*)USBPortDescriptor.RxBuf);
				//print_usb("rxLength: %d\r\n", rxLength);
				// Use packet processing to handle data that may span multiple packets
				/*if(FW_ProcessPacketData(USBPortDescriptor.RxBuf, rxLength) != 0) {
					print_usb("Error processing firmware data\r\n");
					FW_CancelUpdate();
				}*/
				//USBPortDescriptor.RxReady = 0;
			}
			//--------------------------------------------------------------------------
			else if(USBPortDescriptor.RxBuf[0]==0){	// just a CR, no command, so send out operating summary
				Hello(PRINT_USB, false, -1);
				if(BatteryIsCharging()){
					print_usb("\r\nBattery: Charging");
				}
				else{
					print_usb("\r\nBattery: Charged");
				}
			}

			//--------------------------------------------------------------------------
			else { // we have a command, parse it into a command string and a floating point data value
				argC=sscanf_P( (const char *)USBPortDescriptor.RxBuf,PSTR("%2s%f%f"), &cmd, &data1, &data2);
				strlwr(cmd);
				print_usb("\r\n>");
				argC--;	// discount the command string and count only the numeric arguments

				//-------------------------------------
				// "UPdate" transfer control to the bootstrap loader
				if(strncmp(cmd,"up",2)==0){
					//while(1) {
					//} // wait for watchdog timer to get us
					while(1) {};
				}

				//-------------------------------------
				// accept a query to set/get our serial number
				else if(strncmp(cmd,"sn",2)==0){
					if(argC){	// if there was a numeric argument, rescan the command string looking for an integer rather than a float
						argC=sscanf_P( (const char *)USBPortDescriptor.RxBuf,PSTR("%2s%ld"), &cmd, &start);
						EE_Wr(EE_SERIAL_NUM,start);
					}

					print_usb("SN:9-%02d-%02d-%04d-101",(int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000));
				}
				else if(strncmp(cmd, "ba",2 )==0) {
						AnalogPowerOn();
						VBatt = cc_get_batt_volt();
						AnalogPowerOff();										
						print_usb("Battery voltage: %0.2f\n", VBatt);
				}
				else if(!strncmp("vr", cmd, 2)) {
					VBatt = cc_get_batt_volt();
					short battMV = round(VBatt*100);
					print_usb("Battery voltage = %0.2f volts, %dmV", VBatt, battMV);
				}
				else if(!strncmp("to", cmd, 2)) {
					if(argC){	// if there was a numeric argument, rescan the command string looking for an integer rather than a float
						argC=sscanf_P( (const char *)USBPortDescriptor.RxBuf,PSTR("%2s%ld"), &cmd, &start);
						EE_Wr(EE_TIMEOUT_MINUTES,start);
					}
					
					print_usb("Timeout minutes = %d", EE_Rd(EE_TIMEOUT_MINUTES));
				}
				//-------------------------------------
				// "FWRESET" - Reset after firmware update
				else if(strncmp(cmd,"fwreset",7)==0){
					print_usb("Resetting MCU...\r\n");
					_delay_ms(100);  // Give time for message to send
					cli();
					// Use watchdog to reset
					WDT_EnableAndSetTimeout(WDT_PER_8CLK_gc);
					while(1);  // Wait for watchdog reset
				}

				//-------------------------------------
				// we didn't recognize a command, say ?
				else{
					print_usb("?");
				}
			} // if we have a command, parse it
			//print_usb("\r\n>");	// put out the command line prompt
			USBPortDescriptor.RxReady=0;
		}
		osWAIT(100);
	}

END_TASK

//------------------------------------------------------------------------------------------
// USB USART Interrupts (low level)
ISR(USARTF0_RXC_vect){
	USART_RXComplete(&USBPortDescriptor);
}

ISR(USARTF0_DRE_vect){
	USART_DataRegEmpty(&USBPortDescriptor);
}
#endif
