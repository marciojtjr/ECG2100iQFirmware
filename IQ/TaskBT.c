#ifndef BOOTLOADER
#if BLUETOOTH
#include "taskBT.h"

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
#include "taskFlash.h"
#include "eeprom.h"
#include "seg7.h"
#include "taskUSB.h"
#include "taskAcq.h"

#include "apple.h"

#include "crc.h"
#include "BGAPI/bgapi_parser.h"
#include "BGAPI/bgapi_commands.h"
#include "BGAPI/BT122_handler.h"

// External function declaration
extern void ConfigureUSBUSART(unsigned long baudrate);

#define USB_USART				USARTF0
#define USB_USART_PORT	PORTF

unsigned char BTBufReady;
unsigned int BTBuf[8];
char BTLeadOff = 0;
unsigned char transferingFullData = 0;
unsigned char transferingDataType = FULL_TRANSFER_NONE;
char BTStateString[BT_STATE_SIZE];
unsigned char btConfigured = 0;
static unsigned int FrameIndex = 0;
unsigned char connectionNeedsReset = 0;
unsigned char deviceNeedsToTurnOff = 0;
int16_t keepAliveTimer = -1;
unsigned long long appleCrc = 0;

unsigned char connectionType = CONNECTION_ANDROID;
unsigned char waitingForFirstCommand = 1;
char AppleBTString[APPLE_BT_STRING_SIZE];
unsigned char BTString[BT_SIZE]; // Bigger now.
static int appleStringIndex = 0;
unsigned char isThereAnActiveConnection = 0;
unsigned char isBT122 = 0;  // Global variable to track BT122 vs legacy BT

// Are we downsampling to 500Hz?
unsigned char downsample = 0;

//enum { FORMAT_DECIMAL, FORMAT_BINARY }; // various file output formats

//static unsigned char format = FORMAT_BINARY;

static uint8_t outBufferPos;
//##########################################################################################
// define and allocate globals for the bluetooth task

void resetAllVariables(void) {
	outBufferPos = 0;
	BTLeadOff = 0;
	transferingFullData = 0;
	transferingDataType = FULL_TRANSFER_NONE;
	btConfigured = 0;
	FrameIndex = 0;
	connectionNeedsReset = 0;
	keepAliveTimer = -1;
	appleCrc = 0;

	//connectionType = CONNECTION_ANDROID;
	waitingForFirstCommand = 1;
	appleStringIndex = 0;
	isThereAnActiveConnection = 0;
	//RFCOMMTimer = -1;

	RFCOMMConnectionEstablished = 0;
	
	downsample = 0; // Default behavior should ALWAYS be 1000Hz,
					// 500Hz only for special cases
	
	memset((void*)pacerSpikes, 0xFF, 3*sizeof(char));
	
	resetAppleVariables();
	resetUsartAppleVariables();
}

USART_desc_t BTPortDescriptor;	// a descriptor for the USART connected to the Bluetooth transceiver

static int put_bt( char c, FILE *stream){
	//if(connectionType == CONNECTION_ANDROID) {
		//while(!BTRTS()) {
		//	wdt_reset();
			//DPLEDOff();
		//}
	//}
	//DPLEDOn();
	//if(BTRTS()) {
	uart_putchar(&BTPortDescriptor, c);
	return(0);
}

void put_byte(char c) {
	//DPLEDTog();
	if(connectionType == CONNECTION_APPLE) {
		AppleBTString[appleStringIndex++] = c;
		if(appleStringIndex == APPLE_BT_STRING_SIZE) {
			sendECGPacketWithLength(APPLE_BT_STRING_SIZE);
			appleStringIndex = 0;
			//DPLEDTog();
			
			//osWAIT(50);
		}
		//while(!BTRTS()) {
		//	wdt_reset();
			//DPLEDOff();
		//}
		//DPLEDOn();
		//if(BTRTS()) {

		//	DPLEDOn();
		//} else {
		//	DPLEDOff();
		//}
		//DPLEDTog();
	} else {

		uart_large_putchar(&BTPortDescriptor, c);			
	}
}

FILE bt_str = FDEV_SETUP_STREAM(put_bt, 0, _FDEV_SETUP_WRITE);
FILE *bt;

void BTOn(void){
	PORTF.DIRSET = PIN0_bm;
	PORTF.OUTSET = PIN0_bm;
	PORTF.PIN0CTRL = PORT_OPC_TOTEM_gc;
	//isThereAnActiveConnection = 0;
}

void BTOff(void){
	//connectionType = CONNECTION_UNKNOWN;

	PORTF.DIRSET = PIN0_bm;
	PORTF.OUTCLR = PIN0_bm;
	PORTF.PIN0CTRL = PORT_OPC_TOTEM_gc;
	//isThereAnActiveConnection = 0;
	
	resetAllVariables();
}

const unsigned char Hex[16] PROGMEM = {"0123456789ABCDEF"};
char bin2Hex(unsigned char c){
	return(pgm_read_byte(&Hex[c & 0xf]));
}

short
CRCCalc(short crc, short x)
    {
    x = ((crc>>8) ^ x) & 0xff;
    x ^= x>>4;
    return (crc << 8) ^ (x << 12) ^ (x <<5) ^ x;
	}


//==========================================================================================
// Bluetooth USART Configuration Function
// baudrate: desired baudrate (e.g., 115200, 921600)
// enable_rts: 1 to enable RTS (nRTS on PA0/PORTA PIN0), 0 to disable
void ConfigureBTUSART(unsigned long baudrate, unsigned char enable_rts)
{
	int bsel, bscale;

	// Calculate BSEL and BSCALE for common baudrates at 32MHz
	if(baudrate == 115200) {
		bsel = 33;
		bscale = -1;
	} else if(baudrate == 921600) {
		bsel = 19;
		bscale = -4;
	} else {
		// Default to 921600 if unknown baudrate
		bsel = 19;
		bscale = -4;
	}

	BTPortDescriptor.RxBuf = largeRxBuf;
	BTPortDescriptor.TxBuf = largeTxBuf;

	BTPortDescriptor.TxBufSize = LARGE_TX_BUF_SIZE;
	BTPortDescriptor.RxBufSize = LARGE_RX_BUF_SIZE;
	BTPortDescriptor.TxBufMask = LARGE_TX_BUF_MASK;

	bt = &bt_str;
	BT_USART_PORT.DIRSET = PIN3_bm;// TXD is output
	BT_USART_PORT.DIRCLR = PIN2_bm;// RXD is input

    // Configure RTS on PF7/SYNC(55) - BC  for USART
    if(enable_rts) {
        PORTF.DIRSET = PIN7_bm;        // Set PF7 as output
        PORTF.OUTCLR = PIN7_bm;        // Set PF7 low (RTS active, nRTS = 0)
        PORTF.PIN7CTRL = PORT_OPC_TOTEM_gc;  // Totem-pole configuration
            
        // Configure CTS input (PA0)
        PORTA.DIRCLR = PIN0_bm;        // Set PA0 as input
        PORTA.PIN0CTRL = PORT_OPC_PULLUP_gc;  // Enable pull-up
            
        print_usb("[USB] RTS enabled on PF7 (active low)\r\n");
    } else {
        PORTF.DIRCLR = PIN7_bm;        // Set PF7 as input (disabled)
        print_usb("[USB] RTS disabled\r\n");
    } 
	USART_InterruptDriver_Initialize(&BTPortDescriptor, &BT_USART, USART_DREINTLVL_HI_gc);
	USART_RxdInterruptLevel_Set(BTPortDescriptor.usart, USART_RXCINTLVL_HI_gc);
	USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);	//8N1

	USART_Baudrate_Set(BTPortDescriptor.usart, bsel, bscale);
	USART_Rx_Enable(BTPortDescriptor.usart);
	USART_Tx_Enable(BTPortDescriptor.usart);

	btConfigured = 1;

	print_usb("[BT] BT USART initialized at %lu baud\r\n", baudrate);
}

//==========================================================================================
// Bluetooth Task

BEGIN_TASK(TaskBT)
    if (USBPresent()) {
        // Configure USB USART for logging with RTS enabled on PF1
        ConfigureUSBUSART(115200);
    }
	// Configure Bluetooth USART with RTS enabled (PA0/nRTS active for BT122)
	ConfigureBTUSART(115200, 1);
	static unsigned char ix = 0;
	static unsigned int crc;

    BTString[ix++] = 0xAA;  // Preamble byte 1
    BTString[ix++] = 0x55;  // Preamble byte 2
    BTString[ix++] = 0xB3;  // Preamble byte 3
    BTString[ix++] = 0x3B;  // Preamble byte 4
    BTString[ix++] = 0xAF;  // Preamble byte 5
    BTString[ix++] = 0xFA;  // Preamble byte 6
	
	BTStateString[0] = 0xFA; // Status preamble byte 1
	BTStateString[1] = 0xAF; // Status preamble byte 2
	BTStateString[2] = 0x3B; // Status preamble byte 3
	BTStateString[3] = 0xB3; // Status preamble byte 4
	BTStateString[4] = 0x55; // Status preamble byte 5
	BTStateString[5] = 0xAA; // Status preamble byte 6
	
	static unsigned int i;
	static unsigned char j;
	static unsigned long start;
	static unsigned char argC;
	static char cmd[3];
	// static int currentBTIndex = BT_DATA0;  // Unused - removed to save 2 bytes RAM
	static char device_name[20];  // Buffer for device name (e.g., "SpauCliQ010203")

	static unsigned char *btsp = &BTString[BT_DATA0];
	static unsigned char k;
	static unsigned char lead;
	static unsigned char jx;

	static int32_t y0;
	static int32_t y1;
	static int32_t yPrior[8];
	static int16_t z0, z1;
	static int16_t outBuffer[8][8];

    // Initialize BGAPI subsystem
    bgapi_init();

	// Format device name from serial number
	unsigned long serial_num = EE_Rd(EE_SERIAL_NUM);
	sprintf(device_name, "SpauCliQ%02d%02d%04d",
	        (int)((serial_num/1000000)%100),
	        (int)((serial_num/10000)%100),
	        (int)(serial_num%10000));

	// Bluetooth command parser
	debug_message("[BT] TaskBT started, USBPresent=%d\r\n", USBPresent());
	if(true || !USBPresent()){
		BTOn();
		debug_message("[BT] Bluetooth waiting for ready...\r\n");
        unsigned char firstByte = 0;
        while (firstByte == 0) {
            while(btDebugTail != btDebugHead && firstByte == 0) {
                firstByte = btDebugBuf[btDebugTail];
                debug_message("firstByte: %c %d\r\n", firstByte, firstByte);
            }
        }
        static int execution_id = 0;
        execution_id++;
        isBT122 = (firstByte == 160);
        debug_message("[BT] Execution %d: firstByte=%d, isBT122=%d\r\n", execution_id, firstByte, isBT122);
        if (isBT122) {
            BT122_handler_init(device_name, 30000);
			ConfigureBTUSART(921600, 1);
        } else {
			ConfigureBTUSART(921600, 0);
			// Make sure our UART settings are correct
			// we can run Bluetooth only on non rev 1 hardware (which has unprogrammed JTAG UID)
			// Wait until we're in preacq before turning on the radio
			// Or if we're full, let it go through as well
			while(acqState < FSM_PREACQ && acqState != FSM_FULL) {
				osWAIT(1);
			}
			acqState = FSM_PREACQ;
            legacybt_handler_init(device_name, 30000);
        }
		static uint8_t bt122_endpoint;       
		static char extracted_command[3];
		while(1) {
			bgapi_packet_t bt122_packet;
			extracted_command[0] = 0;
			extracted_command[1] = 0;	
			if (isBT122) {		
				while(btDebugTail != btDebugHead) {
					unsigned char data = btDebugBuf[btDebugTail];
					btDebugTail = (btDebugTail + 1) & (BT_DEBUG_BUF_SIZE - 1);
					result = process_bgapi_data(&data, 1, &bt122_packet);
					if (result == 1) {
						if (bt122_packet.endpoint != 0) {
							bt122_endpoint = bt122_packet.endpoint;
						}
						bgapi_print_packet(&bt122_packet);
						extracted_command[0] = bt122_packet.payload[0];
						extracted_command[1] = bt122_packet.payload[1];

						if (bt122_packet.message_type == BGAPI_MSG_TYPE_EVENT) {
							debug_message("[BT] BT122 EVENT: Class=0x%02X Cmd=0x%02X\n", bt122_packet.class_id, bt122_packet.command_id);
                            if (bt122_packet.class_id == BGAPI_CLASS_BT_RFCOMM &&
							    bt122_packet.command_id == BGAPI_EVENT_BT_RFCOMM_OPENED) {
								debug_message("[BT] RFCOMM socket connected (RFCOMM opened)\n");
                                keepAliveTimer = -1;
                                DPLEDOn();
                                BTEnable = 1;
                                waitingForFirstCommand = 1;
                                BTStatus = 0;
							}
							else if (bt122_packet.class_id == BGAPI_CLASS_ENDPOINT &&
							         bt122_packet.command_id == BGAPI_EVENT_ENDPOINT_CLOSED) {
								debug_message("[BT] RFCOMM socket disconnected (endpoint closed)\n");
								BTEnable = 0;
							}
							else if (bt122_packet.class_id == BGAPI_CLASS_BT_CONNECTION &&
							         bt122_packet.command_id == BGAPI_EVENT_BT_CONNECTION_CLOSED) {
								debug_message("[BT] BT client disconnected (connection closed)\n");
								BTEnable = 0; 
							}
							else {
								debug_message("[BT] Unhandled BT122 event: Class=0x%02X Cmd=0x%02X\n", bt122_packet.class_id, bt122_packet.command_id);
							}
						}
					}
				}
			}
			
			if(needToDropPairing) {
				needToDropPairing = 0;
				debug_message("[BT] Pairing dropped - resetting connection\r\n");
				connectionType = CONNECTION_UNKNOWN;
				osWAIT(25);
				print_bt("SET BT PAIR *\r");
				osWAIT(25);
				resetAllVariables();
				EE_Wr(EE_LAST_CONNECTION_TYPE, CONNECTION_UNKNOWN);
			}
			if(!BTEnable && !connectionNeedsReset) {
				// Wait until the CD pin goes high (which happens when we enter data mode)
				if(PORTF.IN & PIN7_bm) {
					//pinTimer = 0;
					keepAliveTimer = -1;
					DPLEDOn();
					debug_message("[BT] CD pin high - entering data mode\r\n");
					BTEnable = 1;					
					waitingForFirstCommand = 1;
					// Clear any status bits that have been set
					BTStatus = 0;					
				}
				if(BTPortDescriptor.RxReady) {
					BTPortDescriptor.RxReady = 0;
				}
			}	
			else if(BTEnable){	
				// if pin goes low for more than 500ms, it's a disconnect
				if(!isBT122 && (PORTF.IN & PIN7_bm)==0) {
					debug_message("[BT] Execution %d: Connection dropped (CD pin low): %d\r\n", execution_id, isBT122);
					DPLEDOff();
					ElapsedMinutes = 0;
					// Reset RTC
					RTC_Initialize( RTC_PERIOD, 0, RTC_PERIOD-1, RTC_PRESCALER_DIV1024_gc );
					BTEnable = 0;
					connectionNeedsReset = 0;  // fix lockup issue (see ECR 79)
				}
				// For BT122, if BTEnable goes from 1 to 0, ensure we're discoverable
				static uint8_t prev_BTEnable = 0;
				if (isBT122 && prev_BTEnable == 1 && BTEnable == 0) {
					debug_message("[BT] BT122 connection lost - resetting to discoverable\r\n");
					BT122_handler_reset_to_discoverable();
				}
				prev_BTEnable = BTEnable;				
				if(isBT122 || BTPortDescriptor.RxReady) {
					if (!isBT122) {
						strncpy(extracted_command, BTPortDescriptor.RxBuf, 2);
						extracted_command[2] = '\0';
					}		
					if(iniAPMode) {
						debug_message("[BT] iAP mode - processing Apple packet\r\n");
						getApplePacket(BTPortDescriptor.RxBuf);
						checkRecievedPacketAndSendNext();
						if(haveCommandFromAppleDevice) {
							haveCommandFromAppleDevice = 0;
						}
						else {
							BTPortDescriptor.RxReady = 0;
							// Don't continue with parsing commands if we don't have one
							continue;
						}
					}
					if(!strncmp("ka", extracted_command, 2)) {
						// If we get a keep alive command, reset the timer, connection is still active
						keepAliveTimer = 0;
						debug_message("[BT] Keep alive received\r\n");
					}					
					else if(!strncmp("do", extracted_command, 2) && (waitingForFirstCommand)) {
						debug_message("[BT] 'do' command - sending Hello\r\n");
						if(connectionType == CONNECTION_APPLE) {
							Hello(PRINT_BT_APPLE, isBT122, bt122_endpoint);
						} else {
							Hello(PRINT_BT, isBT122, bt122_endpoint);
						}
					} else if(!strncmp("lo", extracted_command, 2)) {
						debug_message("[BT] Downsample enabled (500Hz mode)\r\n");
						downsample = 1;
						outBufferPos = 0;
					} else if(!strncmp("hi", extracted_command, 2)) {
						debug_message("[BT] Downsample disabled (1000Hz mode)\r\n");
						downsample = 0;
						outBufferPos = 0;
					} else if(!strncmp("to", extracted_command, 2)) {
						argC = sscanf_P((const char *)(isBT122 ? bt122_packet.payload : BTPortDescriptor.RxBuf), PSTR("%2s%ld"), &cmd, &start);
						print_usb("[BT] Timeout set to %ld minutes\r\n", start);
						EE_Wr(EE_TIMEOUT_MINUTES, start);
												
						if(connectionType == CONNECTION_APPLE) {
							sprintf(AppleBTString, "%d\r\n", EE_Rd(EE_TIMEOUT_MINUTES));
							j = strlen(AppleBTString);
							appleCrc = 0;
							for(i = 0; i < j; i++) {
								appleCrc += AppleBTString[i];
							}
							sendECGPacketWithLength(j);
						} else {
							char timeout_str[16];
							int len = sprintf(timeout_str, "%d\r\n", EE_Rd(EE_TIMEOUT_MINUTES));
							if (isBT122) {
								BT122_handler_data_send(bt122_endpoint, (uint8_t*)timeout_str, len);
							} else {
								print_bt("%s", timeout_str);
							}
						}
					} else if(!strncmp("po", extracted_command, 2)) {
						debug_message("[BT] Power off requested\r\n");
						deviceNeedsToTurnOff = 1;
					}
					else if(!strncmp("sn", extracted_command, 2) && (acqState == FSM_FULL || acqState == FSM_FULL_ON)) {
						if(argC){	// if there was a numeric argument, rescan the command string looking for an integer rather than a float
							argC = sscanf_P( (const char *)(isBT122 ? bt122_packet.payload : BTPortDescriptor.RxBuf), PSTR("%2s%ld"), &cmd, &start);
							EE_Wr(EE_SERIAL_NUM,start);
							print_usb("[BT] Serial number set to %ld\r\n", start);
						}
						char serial_str[24];
						int len = sprintf(serial_str, "SN:9-%02d-%02d-%04d-101",(int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000));
						if (isBT122)
							BT122_handler_data_send(bt122_endpoint, (uint8_t*)serial_str, len);
						else
							print_bt("%s", serial_str);
						debug_message("[BT] Serial number query: SN:9-%02d-%02d-%04d-101\r\n",(int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000));
					}
					else if(!strncmp("?s", extracted_command, 2) && (waitingForFirstCommand)) {
						// Write out our current state
						short battMV = round(VBatt*100);
						BTStateString[6] = acqState;
						BTStateString[7] = battMV;
						BTStateString[8] = battMV >> 8;
						debug_message("[BT] Status query - State: %d, Battery: %dmV bt122_endpoint: %d\r\n", acqState, battMV, bt122_endpoint);
						debug_message("%s", BTStateString);
						if(connectionType != CONNECTION_APPLE) {
							if (isBT122) {
								debug_message("[BT] Using endpoint %d for status response\r\n", bt122_endpoint);
								BT122_handler_data_send(bt122_endpoint, (uint8_t*)BTStateString, BT_STATE_SIZE);
							}
							else	
								fwrite(BTStateString, BT_STATE_SIZE, 1, bt);
						} else {
							sentPacket.payload[0] = getSessionID()[0];
							sentPacket.payload[1] = getSessionID()[1];
							strncpy(&sentPacket.payload[2], BTStateString, BT_STATE_SIZE);
							
							sentPacket.start = 0x55;
							sentPacket.length = BT_STATE_SIZE+6;
							//sentPacket.length = 6+APPLE_BT_STRING_SIZE;
							sentPacket.lingoID = 0;
							sentPacket.commandID = 0x42;
							sentPacket.transID = getCurrentTransID();							
							sendPacket();
						}
					}
					else if(!strncmp("ss", extracted_command, 2) && waitingForFirstCommand) {
						print_usb("[BT] Start streaming command received\r\n");
						waitingForFirstCommand = 0;
						keepAliveTimer = 0;
					}												
					BTPortDescriptor.RxReady = 0;
				}

				if(BTReady && !waitingForFirstCommand) {// we have a lead fail model to decimate
					BTReady=0;
					unsigned char m = M^1;	// use the LFBiasModel NOT being filled at the moment					
					// fill our buffer
					for(i = 0; i < 8; i++) {
						if(downsample) {
							y0 = (int32_t)LFBiasModel[m][i][0]+(int32_t)LFBiasModel[m][i][1];
							y1 = (int32_t)LFBiasModel[m][i][2]+(int32_t)LFBiasModel[m][i][3];
							z0 = (yPrior[i]+y0)/4;
							z1 = (y0+y1)/4;		
							yPrior[i] = y1;
							outBuffer[i][outBufferPos] = z0;
							outBuffer[i][outBufferPos+1] = z1;
						} else {
							outBuffer[i][outBufferPos] = LFBiasModel[m][i][0];
							outBuffer[i][outBufferPos+1] = LFBiasModel[m][i][1];
							outBuffer[i][outBufferPos+2] = LFBiasModel[m][i][2];
							outBuffer[i][outBufferPos+3] = LFBiasModel[m][i][3];
						}
					}
					if(downsample) {
						outBufferPos+=2;
					}
					else {
						outBufferPos+=4;
					}
					if(outBufferPos == 8) {					
						// Set up the BT spike indexes
						for(i = 0; i < 3; i ++) {
							if(pacerSpikes[i] != 0xFF) {
								BTStatus |= 8;
								if(downsample) {
									if(btSpikeIndex == 0xFF)
										btSpikeIndex = (pacerSpikes[i]/2.0f);
									else if(btSpikeIndex2 == 0xFF)
										btSpikeIndex2 = (pacerSpikes[i]/2.0f);
									else if(btSpikeIndex3 == 0xFF)
										btSpikeIndex3 = (pacerSpikes[i]/2.0f);
								}
								else {
									if(btSpikeIndex == 0xFF) 
										btSpikeIndex = pacerSpikes[i];
									else if(btSpikeIndex2 == 0xFF)
										btSpikeIndex2 = pacerSpikes[i];
								}
								pacerSpikes[i] = 0xFF;
							}
						}
						samplesSinceLastPacket = 0;
						// Blast
						outBufferPos = 0;
						btsp = &BTString[BT_DATA0];						

						// Pack
						for(k = 0; k < 8; k++) {
							for(lead = 0; lead < 8; lead += 2) {
								btsp[0] = outBuffer[lead][k] >> 8;
								btsp[1] = (outBuffer[lead][k] & 0xF0) | ((outBuffer[lead+1][k] >> 4) & 0x0F);
								btsp[2] = outBuffer[lead+1][k] >> 8;
								btsp += 3;
							}
						}
					
						//BTString[BT_TYPE] = 1;     // Record type
						BTString[BT_STATUS] = BTStatus;     // Status Flags
						// Pack in our bad leads
						BTLeadOff = 0;
						for(i = 0; i < 8; i++) {
							BTLeadOff |= (badLeads[i]<<i);
						}
						BTString[BT_LEADOFF] = BTLeadOff;     // Lead Off Indicator Bits
						BTString[BT_PACESPIKE] = ((BTStatus & 8) ? btSpikeIndex : 0);		// Pacer spike index bits
						if(btSpikeIndex2 != 0xFF && (BTStatus & 8)) BTString[BT_PACESPIKE] |= (btSpikeIndex2 << 4);
						if(btSpikeIndex3 != 0xFF && (BTStatus & 8)) BTString[BT_PACESPIKE2] = btSpikeIndex3;
						else btSpikeIndex3 = 0;
						
						if(BTStatus & 8) {
							BTStatus &= ~8; // Clear pacer bit 
							btSpikeIndex = 0xFF; // Clear spike indecies
							btSpikeIndex2 = 0xFF;
							btSpikeIndex3 = 0xFF;
						}				
							
						BTString[BT_FRMINDEX] = FrameIndex >> 8;     // Lead Off Indicator Bits
						BTString[BT_FRMINDEX+1] = FrameIndex;     // Lead Off Indicator Bits

						if(connectionType != CONNECTION_APPLE) {
							crc = 0;
							for (jx = BT_PACESPIKE2; jx < BT_CRC; jx++)
								crc = CRCCalc(crc, BTString[jx]);

							BTString[BT_CRC] = crc >> 8;
							BTString[BT_CRC+1] = crc;
						} else {
								for(jx = BT_PACESPIKE2; jx < BT_PACESPIKE2+SMALL_BT_SIZE; jx++) {
									appleCrc += BTString[jx];
								}
						}
					   if(successfulAppleConnection && iniAPMode) {
							if(appleStringIndex < APPLE_BT_STRING_SIZE) {
							   memcpy(&AppleBTString[appleStringIndex], &BTString[BT_PACESPIKE2], SMALL_BT_SIZE);
							   appleStringIndex+=SMALL_BT_SIZE;
							}
							if(appleStringIndex >= APPLE_BT_STRING_SIZE) {
								sendECGPacketWithLength(APPLE_BT_STRING_SIZE);
								appleStringIndex = 0;
								appleCrc = 0;
							}																			   
					   } else if(connectionType == CONNECTION_ANDROID) {
							if (isBT122)
								BT122_handler_data_send(bt122_endpoint, (uint8_t*)BTString, BT_SIZE);
							else
					   			fwrite(BTString, BT_SIZE, 1, bt);
					   }
						FrameIndex++;
					}		
					
					BTBufPhase ^= 1;
					}
				}			
			    osYIELD;	// let other stuff run
		    }
	    }

END_TASK

//------------------------------------------------------------------------------------------
// Bluetooth USART Interrupts (low level)
ISR(USARTE0_RXC_vect){
	USART_RXComplete(&BTPortDescriptor);
}

ISR(USARTE0_DRE_vect){
	USART_DataRegEmpty(&BTPortDescriptor);
}

#endif
#endif