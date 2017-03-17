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

unsigned char BTBufReady;
unsigned int BTBuf[8];
char BTLeadOff = 0;
unsigned char transferingFullData = 0;
unsigned char transferingDataType = FULL_TRANSFER_NONE;
char BTStateString[BT_STATE_SIZE];
unsigned char btConfigured = 0;
static unsigned int FrameIndex = 0;
unsigned char connectionNeedsReset = 0;
int16_t keepAliveTimer = -1;
unsigned long long appleCrc = 0;

unsigned char connectionType = CONNECTION_ANDROID;
unsigned char waitingForFirstCommand = 1;
char AppleBTString[APPLE_BT_STRING_SIZE];
unsigned char BTString[BT_SIZE]; // Bigger now.
static int appleStringIndex = 0;
unsigned char isThereAnActiveConnection = 0;

// Are we downsampling to 500Hz?
unsigned char downsample = 0;

//enum { FORMAT_DECIMAL, FORMAT_BINARY }; // various file output formats

//static unsigned char format = FORMAT_BINARY;

static uint8_t outBufferPos;

#define RESET_BT_AND_WAIT() { readyForCommands = 0; print_bt("RESET\r"); while(readyForCommands == 0) { osWAIT(1); } }

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
// Bluetooth Task

BEGIN_TASK(TaskBT)
	//------------------------------------------------------------------------------------------
	// prepare the Bluetooth USART
	/*
#ifdef BAUDRATECHANGER
	#define BT_BAUD_RATE 115200
#else
	#define BT_BAUD_RATE 921600
#endif
*/
	
	BTPortDescriptor.RxBuf = largeRxBuf;
	BTPortDescriptor.TxBuf = largeTxBuf;

	BTPortDescriptor.TxBufSize = LARGE_TX_BUF_SIZE;
	BTPortDescriptor.RxBufSize = LARGE_RX_BUF_SIZE;
	BTPortDescriptor.TxBufMask = LARGE_TX_BUF_MASK;

	bt = &bt_str;
	BT_USART_PORT.DIRSET = PIN3_bm;// TXD is output
	BT_USART_PORT.DIRCLR = PIN2_bm;// RXD is input

	//BTPortDescriptor.usart->CTRLB |= USART_CLK2X_bm;

	if(!btConfigured) {
		USART_InterruptDriver_Initialize(&BTPortDescriptor, &BT_USART, USART_DREINTLVL_HI_gc); // setup USART, DRE interrupt priority is LOW
		USART_RxdInterruptLevel_Set(BTPortDescriptor.usart, USART_RXCINTLVL_HI_gc); // receive interrupt priority is HIGH
#ifdef BAUDRATECHANGER
		USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N2
		USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/BT_BAUD_RATE-1), 0);
#else
		//USA_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/BT_BAUD_RATE-1), 0);
		USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc,	false);	//8N1
		//USART_Baudrate_Set(BTPortDescriptor.usart, 34, 0); //921600 bps
		USART_Baudrate_Set(BTPortDescriptor.usart, 19, -4);
		//USART_Baudrate_Set(USBPortDescriptor.usart, 30, -4);
		//Seg7HP('b');
#endif

		btConfigured = 1;
	}
	
	//BTPortDescriptor.usart->CTRLA = 0;
	
	//USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_EVEN_gc, false);	//8E1
	//USART_Baudrate_Set(BTPortDescriptor.usart, 33, -1); //921600 bps
	//USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/115200-1), 0);
	
	//USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/BT_BAUD_RATE-1), 0);  // Does not work for all Baud Rates (BAUD_RATE = 921600 for example)
	USART_Rx_Enable(BTPortDescriptor.usart);
	USART_Tx_Enable(BTPortDescriptor.usart);
	
	static unsigned char ix = 0;
	//unsigned int FrameIndex = 0;
	//#ifdef COMPARE_VERSION
	//    short crc;
	//#else
	static unsigned int crc;
	//#endif
	//static uint32_t waiting = 0;
	//static uint32_t beforeSend = 0;

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
	//static unsigned char j;
	static unsigned long start;//, end;
	//static float data1,data2;
	//static int dataInt;
	static unsigned char argC;
	static char cmd[3];
	static int currentBTIndex = BT_DATA0;

	//static float ecgLength;
	//static unsigned long trueEnd;
	//static int trueEndPage;

	//static int startPage, startIndex, endPage, endIndex;

	//#ifdef COMPARE_VERSION
	//	static int secondsOfEcg = 0;
	//#endif

	//static int pinTimer = 0;

	static unsigned char *btsp = &BTString[BT_DATA0];
	static unsigned char k;
	static unsigned char lead;
	static unsigned char jx;

	static int32_t y0;
	static int32_t y1;
	static int32_t yPrior[8];
	static int16_t z0, z1;
	static int16_t outBuffer[8][8];
	
//#endif

	//==========================================================================================
	// Bluetooth command parser

	if(!USBPresent()){
		// Make sure our UART settings are correct
#ifdef BAUDRATECHANGER
		while(1) {
			print_bt("SET CONTROL BAUD 921600,8n1\r");
			osWAIT(250);
		}	
#endif	
		//Seg7HP('b');
		//while(1) {
		//	osWAIT(50);
		//}
		// we can run Bluetooth only on non rev 1 hardware (which has unprogrammed JTAG UID)
		// Wait until we're in preacq before turning on the radio
		// Or if we're full, let it go through as well
		while(acqState < FSM_PREACQ && acqState != FSM_FULL) {
			osWAIT(1);
		}			
		//acqState = FSM_PREACQ;
		
		//BTReady = 1;	
		BTOn();
		
		// Wait until we've recieved "READY" from the BT chip
		while(readyForCommands == 0) {
			osWAIT(1);
		}

		print_bt("SET CONTROL INIT SET CONTROL AUTOCALL\r");	
		RESET_BT_AND_WAIT();
		
		// Set PIO5 as the CD pin
		print_bt("SET CONTROL CD 20 0\r");
		RESET_BT_AND_WAIT();

		// Make sure our name is our serial number
		print_bt("SET BT NAME SpauCliQ%02d%02d%04d\r",(int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000));
		RESET_BT_AND_WAIT();
		//print_bt("SET BT NAME SpauCliQ69696969\r");
		//print_bt("SET BT NAME SN:90000000101\r");
		// Make status updates as fast as possible
		//print_bt("SET BT PAGEMODE 4 2000 1\r");
		//print_bt("SET BT ROLE 0 f 7d00\r");

		print_bt("SET BT PAIRCOUNT 1\r");
		RESET_BT_AND_WAIT();

		print_bt("SET CONTROL CONFIG 0000 2000 2000 1980\r");
		RESET_BT_AND_WAIT();

		//print_bt("SET PROFILE SPP\r");
		//osWAIT(50);
		
		//print_bt("")
		//osWAIT(50);
		// Add entry for iOS devices
		//print_usb("SET PROFILE IAP PROD:\"ECG\" MF:\"Spaulding\" MDL:\"BT2.0\" SN:\"%02d%02d%04d\" PROTO:\"com.bluegiga.iwrap\" SEED:\"4Y5WV3BJAJ\" APP:\"none\" FW:%s HW:%d.0.0 CAPS:0 RF:0 VCD:05 MODE0 PKT2K\r", (int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000), VERSION, MCU.JTAGUID);
		print_bt("SET PROFILE IAP PROD:\"ECG\" MF:\"Spaulding\" MDL:\"%d\" SN:\"%02d%02d%04d\" PROTO:\"com.bluegiga.iwrap\" SEED:\"4Y5WV3BJAJ\" APP:\"com.spauldingclinical.SpauldingECG\" FW:%s HW:%d.0.0 CAPS:0 RF:0 VCD:05 MODE0 PKT2K\r", MCU.JTAGUID, (int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000), VERSION, (int)EE_Rd(EE_BOOT_VERSION));
		osWAIT(250);
		RESET_BT_AND_WAIT();
		
		//print_bt("SET CONTROL AUTOCALL 1101 1000 IAP\r");
		ignoreNextRFCOMM = 1;
		print_bt("SDP ADD 72243468-25ee-4025-8fe7-1edd2cf13cd2 SpauldingECGRFCOMM\r");
		//print_bt("SET CONTROL AUTOCALL 72243468-25ee-4025-8fe7-1edd2cf13cd2 1000 RFCOMM\r");
		osWAIT(250);

		volatile int lastConnection = EE_Rd(EE_LAST_CONNECTION_TYPE);

		//connectionType = CONNECTION_APPLE;
		//if(lastConnection == CONNECTION_APPLE) {
			
			
		if(lastConnection == CONNECTION_APPLE) {
			print_bt("SET CONTROL AUTOCALL 1101 1000 IAP\r");
			//Seg7HP('9');
		}
		//else if(lastConnection == CONNECTION_ANDROID) {
			//ignoreNextRFCOMM = 1;
		//	print_bt("SET CONTROL AUTOCALL 72243468-25ee-4025-8fe7-1edd2cf13cd2 1000 RFCOMM\r");
			//Seg7HP('A');
		//}
		else {
			print_bt("SET CONTROL AUTOCALL\r");
		}
		
		//print_bt("SET CONTROL AUTOCALL 1101 2000 IAP\r");
		
		//print_usb("Flushing BTRxBuffer\r\n");
		//memset((void*)BTPortDescriptor.RxBuf, 0, LARGE_RX_BUF_SIZE);

		//print_usb("TaskBT set up, going for the loop\r\n");

		// We're all done
		//Seg7HP('b');

		while(1) {		
			if(needToDropPairing) {
				needToDropPairing = 0;
				
				//print_usb("Pairing lost. Resetting.\r\n");
				connectionType = CONNECTION_UNKNOWN;
				//Seg7HP('P');
				//ignoreNextRFCOMM = 1;
				//print_bt("SET CONTROL AUTOCALL 1101 1000 RFCOMM\r");
				osWAIT(25);
				print_bt("SET BT PAIR *\r");
				osWAIT(25);
				
				//resetAppleVariables();
				//resetUsartAppleVariables();
				resetAllVariables();
				//connectionType = CONNECTION_ANDROID;
				//connectionNeedsReset = 1;

				//connectionType = CONNECTION_UNKNOWN;
				EE_Wr(EE_LAST_CONNECTION_TYPE, CONNECTION_UNKNOWN);
			}
			if(!BTEnable && !connectionNeedsReset) {
				//Seg7HP('b');
				// Wait until the CD pin goes high (which happens when we enter data mode)
				if(PORTF.IN & PIN7_bm) {
					//pinTimer = 0;
					keepAliveTimer = -1;
					DPLEDOn();
					//print_usb("Pin goes high\r\n");
					BTEnable = 1;
					
					waitingForFirstCommand = 1;
					// Clear any status bits that have been set
					BTStatus = 0;
					
				}

				if(BTPortDescriptor.RxReady) {
					//memset((void*)BTPortDescriptor.RxBuf, 0, RX_BUF_SIZE);
					BTPortDescriptor.RxReady = 0;
				}
			}	
			//if(BTEnable) {
			else if(BTEnable){	
				// if pin goes low for more than 500ms, it's a disconnect	
				
				if((PORTF.IN & PIN7_bm)==0) {
					//print_usb("Death by connection drop.\r\n");
					//inBluetoothUpdateMode = 1;
					//print_usb("Exiting data mode...\r\n");
					//waitingForFirstCommand = 1;
					//BTOff();
					//DPLEDOff();
					//resetAppleVariables();
					//resetUsartAppleVariables();
					//resetAllVariables();
					BTEnable = 0;
					//Seg7HP('d');
					//waitingForFirstCommand = 1;
					//if(acqState == FSM_FULL || acqState == FSM_FULL_ON) acqState = FSM_DONE;

					connectionNeedsReset = 1;
				}
				
				if(BTPortDescriptor.RxReady) {
					print_usb("We have a command! First two chars: %c%c\r\n", BTPortDescriptor.RxBuf[0], BTPortDescriptor.RxBuf[1]);
					//if(ignoreNextRxReady) {
					//	BTPortDescriptor.RxReady = 0;
//ignoreNextRxReady = 0;
						//Seg7HP('r');
					//	continue;
					//}
					
					// We have a command, drop the timer

					/*
					if(RFCOMMTimer != -1) {
						RFCOMMTimer = -1;
						//Seg7HP('r');
					}
					*/						
					
					if(iniAPMode) {
						//DPLEDTog();
						//Seg7HP('A');
						getApplePacket(BTPortDescriptor.RxBuf);
						//memset(BTPortDescriptor.RxBuf, 0, RX_BUF_SIZE);

						//keepAliveTimer = 0;

						// Clear buffer right away
						//memset(BTPortDescriptor.RxBuf, 0, RX_BUF_SIZE);
						//printApplePacket(recievedPacket);
						checkRecievedPacketAndSendNext();

						if(haveCommandFromAppleDevice) {
							//DPLEDTog();
							
							haveCommandFromAppleDevice = 0;
						}
						else {
							BTPortDescriptor.RxReady = 0;
							// Don't continue with parsing commands if we don't have one
							continue;
						}
					}

					if(!strncmp("ka", BTPortDescriptor.RxBuf, 2)) {
						// If we get a keep alive command, reset the timer, connection is still active
						keepAliveTimer = 0;
						//Seg7HP('A');
						//DPLEDTog();
						//print_usb("Keep alive recieved.\r\n");
					}	
					
					else if(!strncmp("do", BTPortDescriptor.RxBuf, 2) && (waitingForFirstCommand)) {
						if(connectionType == CONNECTION_APPLE) {
							Hello(PRINT_BT_APPLE);
						} else {
							Hello(PRINT_BT);
						}
					} else if(!strncmp("lo", BTPortDescriptor.RxBuf, 2)) {
						downsample = 1;
						outBufferPos = 0;
					} else if(!strncmp("hi", BTPortDescriptor.RxBuf, 2)) {
						downsample = 0;
						outBufferPos = 0;
					}
					/*
					else if(!strncmp("dc", BTPortDescriptor.RxBuf, 2)) {
						
						BTOff();

						DPLEDOff();

						resetAppleVariables();
						resetUsartAppleVariables();

						//Seg7HP('d');

						//print_usb("Pin goes low\r\n");
						BTEnable = 0;
						//BTOff();
						//osWAIT(100);
						connectionNeedsReset = 1;
						*/
								/*
								BTOff();

								DPLEDOff();

								resetAppleVariables();
								resetUsartAppleVariables();

								//Seg7HP('d');

							//print_usb("Pin goes low\r\n");
								BTEnable = 0;
								//osWAIT(100);
								connectionNeedsReset = 1;
								keepAliveTimer = -1;
								*/
					//}			
					/*
					else if(!strncmp("pa", BTPortDescriptor.RxBuf, 2)&& (acqState == FSM_FULL || acqState == FSM_FULL_ON)) {
						currentBTIndex = BT_DATA0;
						//volatile int dataInt;

						argC=sscanf_P( (const char *)BTPortDescriptor.RxBuf,PSTR("%2s%d"), &cmd, &dataInt);

						start = dataInt*8;
						end = start + 8;

						//print_usb("CurrentBTIndex = %d\r\n", currentBTIndex);

						//print_usb("Start = %d\r\	n", start);
						//print_usb("End = %d\r\n", end);

						startPage = (start)/ECG_MS_PER_PAGE + FLASH_ECG_START_PAGE;
						//print_usb("Startpage = %d\r\n", startPage);
						startIndex = (start)%ECG_MS_PER_PAGE;
						//print_usb("StartIndex = %d\r\n", startIndex);

						endPage = (end)/ECG_MS_PER_PAGE + FLASH_ECG_START_PAGE;
						//print_usb("EndPage = %d\r\n", endPage);
						endIndex = (end)%ECG_MS_PER_PAGE;
						//print_usb("EndIndex = %d\r\n", endIndex);

						BTString[BT_TYPE] = 1;     // Record type
						BTString[BT_STATUS] = 4;     // Status Flags
						BTString[BT_LEADOFF] = 0;     // Lead Off Indicator Bits	
							
						BTString[BT_FRMINDEX] = dataInt >> 8;     // Lead Off Indicator Bits
						BTString[BT_FRMINDEX+1] = dataInt;     // Lead Off Indicator Bits

						FlashPageNumber = startPage;
						while(TaskFlashCmd != TF_IDLE); // should never happen
						TaskFlashCmd = TF_READ_BUF1;
						while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
							osYIELD;
						}
						for(i = startIndex; i < (startPage == endPage ? endIndex : ECG_MS_PER_PAGE); i++) {
							// POPULATE
							for(j = 0; j < 12; j++, currentBTIndex++) {
								BTString[(currentBTIndex)] = Buf1.Data.ECG.Sample[i][j];
							}
							
						}
						//print_usb("CurrentBTIndex after first page = %d\r\n", currentBTIndex);
						btSpikeIndex = 0xFF;
						btSpikeIndex2 = 0xFF;

						uint16_t temp = ((Buf1.Data.ECG.Flag1 << 8) | Buf1.Data.ECG.Flag2);

						
						if(temp & 1) {
							usbSpikeIndex = (temp >> 1) & 0x1F;
							usbSpikeIndex2 = (temp >> 6) & 0x1F;
							usbSpikeIndex3 = (temp >> 11) & 0x1F;

							if(usbSpikeIndex && usbSpikeIndex >= startIndex && (startPage == endPage ? usbSpikeIndex <= endIndex : usbSpikeIndex <= ECG_MS_PER_PAGE)) {
								btSpikeIndex = usbSpikeIndex-startIndex;
							}
							if(usbSpikeIndex2 && usbSpikeIndex2 >= startIndex && (startPage == endPage ? usbSpikeIndex2 <= endIndex : usbSpikeIndex2 <= ECG_MS_PER_PAGE)) {
								if(btSpikeIndex == 0xFF)
									btSpikeIndex = usbSpikeIndex2-startIndex;
								else
									btSpikeIndex2 = usbSpikeIndex2-startIndex;
							}
							if(usbSpikeIndex3 && usbSpikeIndex3 >= startIndex && (startPage == endPage ? usbSpikeIndex3 <= endIndex : usbSpikeIndex3 <= ECG_MS_PER_PAGE)) {
								if(btSpikeIndex == 0xFF)
									btSpikeIndex = usbSpikeIndex3-startIndex;
								else
									btSpikeIndex2 = usbSpikeIndex3-startIndex;
							}
						}
						

						if(startPage != endPage) {
							FlashPageNumber = endPage;
							while(TaskFlashCmd != TF_IDLE); // should never happen
							TaskFlashCmd = TF_READ_BUF1;
							while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
								osYIELD;
							}
							for(i = 0; i < endIndex; i++) {
								for(j = 0; j < 12; j++, currentBTIndex++) {
									BTString[(currentBTIndex)] = Buf1.Data.ECG.Sample[i][j];
								}
							}
							//print_usb("CurrentBTIndex after second page = %d\r\n", currentBTIndex);

							uint16_t temp = ((Buf1.Data.ECG.Flag1 << 8) | Buf1.Data.ECG.Flag2);

							
							if(temp & 1) {
								usbSpikeIndex = (temp >> 1) & 0x1F;
								usbSpikeIndex2 = (temp >> 6) & 0x1F;
								usbSpikeIndex3 = (temp >> 11) & 0x1F;

								if(usbSpikeIndex && usbSpikeIndex >= 0 && usbSpikeIndex <= endIndex) {
									if(btSpikeIndex == 0xFF)
										btSpikeIndex = usbSpikeIndex;
									else
										btSpikeIndex2 = usbSpikeIndex;
								}
								if(usbSpikeIndex2 && usbSpikeIndex2 >= 0 && usbSpikeIndex2 <= endIndex) {
									if(btSpikeIndex == 0xFF)
										btSpikeIndex = usbSpikeIndex2;
									else
										btSpikeIndex2 = usbSpikeIndex2;
								}
								if(usbSpikeIndex3 && usbSpikeIndex3 >= 0 && (startPage == endPage ? usbSpikeIndex3 <= endIndex : usbSpikeIndex3 <= ECG_MS_PER_PAGE)) {
									if(btSpikeIndex == 0xFF)
										btSpikeIndex = usbSpikeIndex3;
									else
										btSpikeIndex2 = usbSpikeIndex3;
								}
							}
							
						}
						

						if(btSpikeIndex != 0xFF || btSpikeIndex2 != 0xFF) {
							BTStatus |= 8;
						}

						BTString[BT_PACESPIKE] = ((temp & 1) ? btSpikeIndex : 0);		// Pacer spike index bits
						if(btSpikeIndex2 != 0xFF && (temp & 1)) BTString[BT_PACESPIKE] |= (btSpikeIndex2 << 4);	

						crc = 0;
						for (unsigned char jx = BT_TYPE; jx < BT_CRC; jx++)
							crc = CRCCalc(crc, BTString[jx]);

						BTString[BT_CRC] = crc >> 8;
						BTString[BT_CRC+1] = crc;

						if(connectionType == CONNECTION_ANDROID) {
							fwrite(BTString, BT_SIZE, 1, bt);
						} else {
							memcpy(AppleBTString, BTString, BT_SIZE);
							sendECGPacketWithLength(BT_SIZE);
						}

						//print_bt("we want packet %d", dataInt);
					}
					
					else if(!strncmp("v+", BTPortDescriptor.RxBuf, 2)) {
						EE_Wr(EE_ENABLE_VOICE, 1);
					}
					else if(!strncmp("v-", BTPortDescriptor.RxBuf, 2)) {
						EE_Wr(EE_ENABLE_VOICE, 0);
					}
					else if(!strncmp("s+", BTPortDescriptor.RxBuf, 2)) {
						continuousMode = 1;
						EE_Wr(EE_ENABLE_CONTINUOUS, 1);
					}
					else if(!strncmp("s-", BTPortDescriptor.RxBuf, 2)) {
						continuousMode = 0;
						EE_Wr(EE_ENABLE_CONTINUOUS, 0);
					}

					else if(!strncmp("q+", BTPortDescriptor.RxBuf, 2)) {
						LFBiasEnable = 1;
						EE_Wr(EE_LFBIAS_ENABLE, 1);
					}
					else if(!strncmp("q-", BTPortDescriptor.RxBuf, 2)) {
						LFBiasEnable = 0;
						EE_Wr(EE_LFBIAS_ENABLE, 0);
					}
					*/
					else if(!strncmp("sn", BTPortDescriptor.RxBuf, 2) && (acqState == FSM_FULL || acqState == FSM_FULL_ON)) {
						if(argC){	// if there was a numeric argument, rescan the command string looking for an integer rather than a float
							argC=sscanf_P( (const char *)BTPortDescriptor.RxBuf,PSTR("%2s%ld"), &cmd, &start);
							EE_Wr(EE_SERIAL_NUM,start);
						}
						print_bt("SN:9-%02d-%02d-%04d-101",(int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000));
					}
					/*
					else if(!strncmp("et", BTPortDescriptor.RxBuf, 2) && (acqState == FSM_FULL || acqState == FSM_FULL_ON)) {
						ElapsedMinutes = 0;
					}
					*/

					else if(!strncmp("?s", BTPortDescriptor.RxBuf, 2) && (waitingForFirstCommand)) {
						//DPLEDOn();
						
						// Report status
						//fwrite(BTStateString, BT_STATE_SIZE, 1, bt);

						// Write out our current state
						short battMV = round(VBatt*100);
						BTStateString[6] = acqState;
						BTStateString[7] = battMV;
						BTStateString[8] = battMV >> 8;

						//Seg7HP('S');
						
						if(connectionType != CONNECTION_APPLE) {
							fwrite(BTStateString, BT_STATE_SIZE, 1, bt);
							
							//connectionType = CONNECTION_ANDROID;
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
					else if(!strncmp("ss", BTPortDescriptor.RxBuf, 2) && waitingForFirstCommand) {
						//inBluetoothUpdateMode = 0;
						//print_usb("Entering data mode...\r\n");
						
						//isThereAnActiveConnection = 1;
						waitingForFirstCommand = 0;
						keepAliveTimer = 0;
						//continue;

						//Seg7HP('s');
						//startStreaming();
					}												
					
					//memset((void*)BTPortDescriptor.RxBuf, 0 , RX_BUF_SIZE);
					BTPortDescriptor.RxReady = 0;
				}

				if(BTReady && !waitingForFirstCommand) {// we have a lead fail model to decimate
					BTReady=0;
					//DPLEDTog();
					// Bluetooth output will be at 125Hz, so we'll need to average eigth 1KHz samples, which we can do by adding up all the lead fail subsamples for two lead fail cycles

					
					unsigned char m = M^1;	// use the LFBiasModel NOT being filled at the moment
					
					// fill our buffer
					for(i = 0; i < 8; i++) {
					
						// Fill our buffer
//#if SAMPLING_RATE == 500
						if(downsample) {
							y0 = (int32_t)LFBiasModel[m][i][0]+(int32_t)LFBiasModel[m][i][1];
							y1 = (int32_t)LFBiasModel[m][i][2]+(int32_t)LFBiasModel[m][i][3];
							z0 = (yPrior[i]+y0)/4;
							z1 = (y0+y1)/4;						
													
							yPrior[i] = y1;
													
							outBuffer[i][outBufferPos] = z0;
							outBuffer[i][outBufferPos+1] = z1;
							//outBuffer[i][outBufferPos] = LFBiasModel[m][i][0];
							//outBuffer[i][outBufferPos] = LFBiasModel[m][i][3];
						} else {
//#else
							
							outBuffer[i][outBufferPos] = LFBiasModel[m][i][0];
							outBuffer[i][outBufferPos+1] = LFBiasModel[m][i][1];
							outBuffer[i][outBufferPos+2] = LFBiasModel[m][i][2];
							outBuffer[i][outBufferPos+3] = LFBiasModel[m][i][3];
						}
//#endif
					}
//#if SAMPLING_RATE == 500
					if(downsample) {
						outBufferPos+=2;
						//outBufferPos++;
					}
//#else
					else {
						outBufferPos+=4;
					}
//#endif
					
					if(outBufferPos == 8) {
					
						// Set up the BT spike indexes
						for(i = 0; i < 3; i ++) {
							if(pacerSpikes[i] != 0xFF) {
								BTStatus |= 8;
//#if SAMPLING_RATE == 500
								if(downsample) {
									if(btSpikeIndex == 0xFF)
										btSpikeIndex = (pacerSpikes[i]/2.0f);
									else if(btSpikeIndex2 == 0xFF)
										btSpikeIndex2 = (pacerSpikes[i]/2.0f);
									else if(btSpikeIndex3 == 0xFF)
										btSpikeIndex3 = (pacerSpikes[i]/2.0f);
								}
								
//#else
								else {
									if(btSpikeIndex == 0xFF) 
										btSpikeIndex = pacerSpikes[i];
									else if(btSpikeIndex2 == 0xFF)
										btSpikeIndex2 = pacerSpikes[i];
								}
								pacerSpikes[i] = 0xFF;
//#endif						
							}
						}
						
						//memset((void*)pacerSpikes, 0xFF, 3*sizeof(char));

						samplesSinceLastPacket = 0;

						// Blast
						outBufferPos = 0;
						btsp = &BTString[BT_DATA0];
						

						// Pack
//#if BITS == 12
						for(k = 0; k < 8; k++) {
							for(lead = 0; lead < 8; lead += 2) {
								btsp[0] = outBuffer[lead][k] >> 8;
								btsp[1] = (outBuffer[lead][k] & 0xF0) | ((outBuffer[lead+1][k] >> 4) & 0x0F);
								btsp[2] = outBuffer[lead+1][k] >> 8;
								btsp += 3;
							}
						}
						
//#else 
/*
						for(k = 0; k < 8; k++) {
							for(lead = 0; lead < 8; lead++) {
								//*btsp++ = (outBuffer[lead][k] >> 8);
								//*btsp++ = (outBuffer[lead][k] & 0xFF);
								*((int16_t*)(btsp)) = outBuffer[lead][k];
								btsp+=2;
							}
						}
*/
//#endif
					
						//BTString[BT_TYPE] = 1;     // Record type
						BTString[BT_STATUS] = BTStatus;     // Status Flags
						// Pack in our bad leads
						BTLeadOff = 0;
						for(i = 0; i < 8; i++) {
							BTLeadOff |= (badLeads[i]<<i);
						}
						BTString[BT_LEADOFF] = BTLeadOff;     // Lead Off Indicator Bits
						BTString[BT_PACESPIKE] = ((BTStatus & 8) ? btSpikeIndex : 0);		// Pacer spike index bits
						//BTString[BT_PACESPIKE2] = (((BTStatus & 8) && btSpikeIndex3 != 0xFF )? btSpikeIndex3:0);
						if(btSpikeIndex2 != 0xFF && (BTStatus & 8)) BTString[BT_PACESPIKE] |= (btSpikeIndex2 << 4);
						if(btSpikeIndex3 != 0xFF && (BTStatus & 8)) BTString[BT_PACESPIKE2] = btSpikeIndex3;
						else btSpikeIndex3 = 0;
						
						if(BTStatus & 8) {
							//print_bt("Pacer spike detected at index: %d, %d, sample index: %d\r\n", btSpikeIndex, btSpikeIndex2, FrameIndex);
							//print_bt("Pacer spike index 1: %d\r", btSpikeIndex);
							//print_bt("Pacer Spike index 2: %d\r", btSpikeIndex2);
							//print_bt("We have a pacer spike: "BYTETOBINARYPATTERN"\r", BYTETOBINARY(BTString[BT_PACESPIKE]));
							BTStatus &= ~8; // Clear pacer bit 
							btSpikeIndex = 0xFF; // Clear spike indecies
							btSpikeIndex2 = 0xFF;
							btSpikeIndex3 = 0xFF;
							
							//memset(pacerSpikes, 0xFF, 3);
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
							//BTString[BT_CRC] = 0;
							//BTString[BT_CRC+1] = 0;
							//if(appleStringIndex < SMALL_BT_SIZE) {
								for(jx = BT_PACESPIKE2; jx < BT_PACESPIKE2+SMALL_BT_SIZE; jx++) {
									appleCrc += BTString[jx];
								}
							//}
						}
						
				       //for(i = 0; i < APPLE_BT_STRING_SIZE; i++) {
						//   AppleBTString[i] = 'T';
					   //}
					   if(successfulAppleConnection && iniAPMode) {
						   //print_usb("Full Apple BT String Size = %d\r\n", APPLE_BT_STRING_SIZE);
						   //for(int i = 0; i < BT_SIZE; i++) {
						//	   AppleBTString[appleStringIndex++] = BTString[i];
						//   }
							if(appleStringIndex < APPLE_BT_STRING_SIZE) {
							   memcpy(&AppleBTString[appleStringIndex], &BTString[BT_PACESPIKE2], SMALL_BT_SIZE);
							   appleStringIndex+=SMALL_BT_SIZE;
							}
							if(appleStringIndex >= APPLE_BT_STRING_SIZE) {
								sendECGPacketWithLength(APPLE_BT_STRING_SIZE);
								//askIdentity();
								
								//DPLEDOff();
								//Seg7HP('O');
								
								appleStringIndex = 0;
								appleCrc = 0;
							}																			   
					   } else if(connectionType == CONNECTION_ANDROID) {
					   		fwrite(BTString, BT_SIZE, 1, bt);
					   }
					   						   
						//fwrite(BTString, BT_SIZE, 1, bt);
						/*
						if(successfulAppleConnection && iniAPMode && appleStringIndex >= APPLE_BT_STRING_SIZE) {
							//print_usb("Full Apple BT String Size = %d\r\n", APPLE_BT_STRING_SIZE);
							//print_usb("Apple BT String Size = %d\r\n", appleStringIndex);
							//DPLEDOn();
							//Seg7HP('A');
							//sendECGPacket();
							//memset(AppleBTString, 0, APPLE_BT_STRING_SIZE);
							sendECGPacketWithLength(APPLE_BT_STRING_SIZE);
							//askIdentity();
							
							//DPLEDOff();
							//Seg7HP('O');
							
							appleStringIndex = 0;
							appleCrc = 0;

							//if(connectionType == CONNECTION_APPLE && acqState == FSM_WAITINGFORPACKET)
							//	haveFinalPacket = 1;
						}*/ /*else if(!successfulAppleConnection) {
							Seg7HP('A');
						} else if (!iniAPMode) {
							Seg7HP('P');
						} else {
							Seg7HP('b');
						}*/
						//print_bt("TEST");
						
						//if(acqState == FSM_WAITINGFORPACKET) {
						//	haveFinalPacket = 1;
							//DPLEDOff();
						//}

						FrameIndex++;

						//DPLEDTog();

						
					}
					
					
					BTBufPhase ^= 1;
					

						
						

						//osWAIT(8);
						//BTReady = 1;
						//}

					}
				}			
			    osYIELD;	// let other stuff run
		    }
	    }
	
	else
        { // consume as little time as possible, we're not supposed to be running
		while(1)
            {
			osWAIT(250);
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