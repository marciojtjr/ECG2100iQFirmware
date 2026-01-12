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
#include "taskCC.h"
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
unsigned char deviceNeedsToTurnOff = 0;
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

static uint8_t outBufferPos;

static int iBlinkyCount = 0;

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

		}

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
	
	//USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/BT_BAUD_RATE-1), 0);  // Does not work for all Baud Rates (BAUD_RATE = 921600 for example)
	USART_Rx_Enable(BTPortDescriptor.usart);
	USART_Tx_Enable(BTPortDescriptor.usart);
	
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
	static unsigned long start;//, end;
	//static float data1,data2;
	//static int dataInt;
	static unsigned char argC;
	static char cmd[3];
	static int currentBTIndex = BT_DATA0;

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

		print_bt("SET BT PAIRCOUNT 1\r");
		RESET_BT_AND_WAIT();

		print_bt("SET CONTROL CONFIG 0000 2000 2000 1980\r");
		RESET_BT_AND_WAIT();

		print_bt("SET PROFILE IAP PROD:\"ECG\" MF:\"Spaulding\" MDL:\"%d\" SN:\"%02d%02d%04d\" PROTO:\"com.bluegiga.iwrap\" SEED:\"4Y5WV3BJAJ\" APP:\"com.spauldingclinical.SpauldingECG\" FW:%s HW:%d.0.0 CAPS:0 RF:0 VCD:05 MODE0 PKT2K\r", MCU.JTAGUID, (int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000), VERSION, (int)EE_Rd(EE_BOOT_VERSION));
		osWAIT(250);
		RESET_BT_AND_WAIT();
		
		//print_bt("SET CONTROL AUTOCALL 1101 1000 IAP\r");
		ignoreNextRFCOMM = 1;
		print_bt("SDP ADD 72243468-25ee-4025-8fe7-1edd2cf13cd2 SpauldingECGRFCOMM\r");
		//print_bt("SET CONTROL AUTOCALL 72243468-25ee-4025-8fe7-1edd2cf13cd2 1000 RFCOMM\r");
		osWAIT(250);

		volatile int lastConnection = EE_Rd(EE_LAST_CONNECTION_TYPE);
	
		if(lastConnection == CONNECTION_APPLE) {
			print_bt("SET CONTROL AUTOCALL 1101 1000 IAP\r");
			//Seg7HP('9');
		}

		else {
			print_bt("SET CONTROL AUTOCALL\r");
		}
		

#ifdef EMISSIONSTEST
static unsigned char sendTestCmd = 1;
static unsigned char channelOne = 0;
static unsigned char pressed = 0;

// clear the leds
	Seg7A(0);
	Seg7B(0);
	Seg7C(0);
	Seg7D(0);
	Seg7E(0);
	Seg7F(0);
	Seg7G(1);

	DPLEDOn();
	InitButton();


while(1) {
	print_usb("1 TestCmd=0x%x channelOne=0x%x pressed=0x%x\r\n", sendTestCmd, channelOne, pressed);
	
	if (sendTestCmd) {
		print_usb("   TestCmd\r\n");

		if (channelOne) {
			print_usb("      Transmitting on channel 1\r\n");
			print_bt("TEST TXDATA1 2402 0xFF3F\r"); // channel one - second attempt
			//print_bt("TEST TXSTART 2441 0xFF3F 0\r"); // channel one - first attempt for UL
			Seg7G(0); // middle segment is off for channel one
			channelOne = 0; // toggle for next cmd
		}
		else {
			print_usb("      Transmitting on channel 2\r\n");
			print_bt("TEST TXDATA1 2480 0xFF3F\r"); // channel two - second attempt
			//print_bt("TEST TXSTART 2441 0xFF3F 256\r"); // channel two - first attempt for UL
			Seg7G(1); // middle segment is on for channel two
			channelOne = 1; // toggle for next cmd
		}
		sendTestCmd = 0;
		DPLEDTog();
		pressed = 0;
	}
	print_usb("2 TestCmd=0x%x channelOne=0x%x pressed=0x%x\r\n", sendTestCmd, channelOne, pressed);

	Seg7A(1);
	pressed = pressed | ((ButtonPressed() != 0) ? 1 : 0);
	osWAIT(100);
	Seg7B(1);
	Seg7A(0);
	pressed = pressed | ((ButtonPressed() != 0) ? 1 : 0);
	osWAIT(100);
	Seg7C(1);
	Seg7B(0);
	pressed = pressed | ((ButtonPressed() != 0) ? 1 : 0);
	osWAIT(100);
	Seg7D(1);
	Seg7C(0);
	pressed = pressed | ((ButtonPressed() != 0) ? 1 : 0);
	osWAIT(100);
	Seg7E(1);
	Seg7D(0);
	pressed = pressed | ((ButtonPressed() != 0) ? 1 : 0);
	osWAIT(100);
	Seg7F(1);
	Seg7E(0);
	pressed = pressed | ((ButtonPressed() != 0) ? 1 : 0);
	osWAIT(100);
	Seg7F(0);

	print_usb("3 TestCmd=0x%x channelOne=0x%x pressed=0x%x\r\n", sendTestCmd, channelOne, pressed);

	if (pressed) {
		print_usb("*** button pressed ***\r\n");
		sendTestCmd = 1; // so we send the command again
	}

}
#endif // EMISSIONSTEST


		while(1) {		
			if(needToDropPairing) {
				needToDropPairing = 0;
				
				//print_usb("Pairing lost. Resetting.\r\n");
				connectionType = CONNECTION_UNKNOWN;

				osWAIT(25);
				print_bt("SET BT PAIR *\r");
				osWAIT(25);
				
				//resetAppleVariables();
				//resetUsartAppleVariables();
				resetAllVariables();

				EE_Wr(EE_LAST_CONNECTION_TYPE, CONNECTION_UNKNOWN);
			}
			if(!BTEnable && !connectionNeedsReset) {
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

					BTPortDescriptor.RxReady = 0;
				}
			}	
			else if(BTEnable){	
				// if pin goes low for more than 500ms, it's a disconnect	
				
				if((PORTF.IN & PIN7_bm)==0) {
					DPLEDOff();
					ElapsedMinutes = 0;
					// Reset RTC
					RTC_Initialize( RTC_PERIOD, 0, RTC_PERIOD-1, RTC_PRESCALER_DIV1024_gc );
					BTEnable = 0;
					connectionNeedsReset = 0;  // fix lockup issue (see ECR 79)
				}
				
				if(BTPortDescriptor.RxReady) {
					print_usb("We have a command! First two chars: %c%c\r\n", BTPortDescriptor.RxBuf[0], BTPortDescriptor.RxBuf[1]);					
					
					if(iniAPMode) {
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

					if(!strncmp("ka", BTPortDescriptor.RxBuf, 2)) {
						// If we get a keep alive command, reset the timer, connection is still active
						keepAliveTimer = 0;
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
					} else if(!strncmp("to", BTPortDescriptor.RxBuf, 2)) {
						argC=sscanf_P( (const char *)BTPortDescriptor.RxBuf,PSTR("%2s%ld"), &cmd, &start);

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
							print_bt("%d\r\n", EE_Rd(EE_TIMEOUT_MINUTES));
						}
					} else if(!strncmp("po", BTPortDescriptor.RxBuf, 2)) {
						deviceNeedsToTurnOff = 1;
					}
					else if(!strncmp("sn", BTPortDescriptor.RxBuf, 2) && (acqState == FSM_FULL || acqState == FSM_FULL_ON)) {
						if(argC){	// if there was a numeric argument, rescan the command string looking for an integer rather than a float
							argC=sscanf_P( (const char *)BTPortDescriptor.RxBuf,PSTR("%2s%ld"), &cmd, &start);
							EE_Wr(EE_SERIAL_NUM,start);
						}
						print_bt("SN:9-%02d-%02d-%04d-%03d",(int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000), HW_SFX);
					}

					else if(!strncmp("?s", BTPortDescriptor.RxBuf, 2) && (waitingForFirstCommand)) {
						uint8_t battMV = (uint8_t)((double)(cc_get_batt_volt()*1000/20));	//max of 5.1 volts
						BTStateString[6] = acqState;
						BTStateString[7] = battMV;
						BTStateString[8] = (((uint8_t)cc_get_fg_valid() << 7)|(cc_get_batt_per()));

						if(connectionType != CONNECTION_APPLE) {
							fwrite(BTStateString, BT_STATE_SIZE, 1, bt);
							
							//connectionType = CONNECTION_ANDROID;
						} else {
							sentPacket.payload[0] = getSessionID()[0];
							sentPacket.payload[1] = getSessionID()[1];
							strncpy(&sentPacket.payload[2], BTStateString, BT_STATE_SIZE);
							
							sentPacket.start = 0x55;
							sentPacket.length = BT_STATE_SIZE+6;
							sentPacket.lingoID = 0;
							sentPacket.commandID = 0x42;
							sentPacket.transID = getCurrentTransID();
							
							sendPacket();
						}
					}
					else if(!strncmp("ss", BTPortDescriptor.RxBuf, 2) && waitingForFirstCommand) {
						waitingForFirstCommand = 0;
						keepAliveTimer = 0;
					}												
					
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
						
						samplesSinceLastPacket = 0;

						// Blast
						outBufferPos = 0;
						btsp = &BTString[BT_DATA0];
						
						for(k = 0; k < 8; k++) {
							for(lead = 0; lead < 8; lead += 2) {
								btsp[0] = outBuffer[lead][k] >> 8;
								btsp[1] = (outBuffer[lead][k] & 0xF0) | ((outBuffer[lead+1][k] >> 4) & 0x0F);
								btsp[2] = outBuffer[lead+1][k] >> 8;
								btsp += 3;
							}
						}
						
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
						
						if(BTStatus & 8) {;
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
					   		fwrite(BTString, BT_SIZE, 1, bt);
					   }

						FrameIndex++;
	
					}
					
					
					BTBufPhase ^= 1;

					}
				}		
				iBlinkyCount++;
				if(iBlinkyCount%5000==0){
					DPLEDTog();
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