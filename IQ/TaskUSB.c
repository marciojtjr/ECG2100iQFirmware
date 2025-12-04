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
#include "taskFlash.h"
#include "taskAcq.h"
#include "eeprom.h"
#include "ulaw.h"
#include "crc.h"
#include "seg7.h"
#include "TaskWatchDog.h"
#include "taskBT.h"
#include "taskUSB.h"

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

//#include "sp_driver.h"

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
	if(type == PRINT_USB) {
		print_usb("\r\n\r\nSpaulding Electrocardiograph BV:%d FV:%s HV:%d SN:9-%02d-%02d-%04d-101\r\n", (int)EE_Rd(EE_BOOT_VERSION), VERSION, MCU.JTAGUID, (int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000));
	}
	else if(type == PRINT_BT) {
		print_bt("\r\n\r\nSpaulding Electrocardiograph BV:%d FV:%s HV:%d SN:9-%02d-%02d-%04d-101\r\n", (int)EE_Rd(EE_BOOT_VERSION), VERSION, MCU.JTAGUID, (int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000));
	}
	else if(type == PRINT_BT_APPLE) {
		sprintf(AppleBTString, "\r\n\r\nSpaulding Electrocardiograph BV:%d FV:%s HV:%d SN:9-%02d-%02d-%04d-101\r\n", (int)EE_Rd(EE_BOOT_VERSION), VERSION, MCU.JTAGUID, (int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000));
		j = strlen(AppleBTString);

		appleCrc = 0;
		for(i = 0; i < j; i++) {
			appleCrc += AppleBTString[i];
		}

		sendECGPacketWithLength(j);
		//DPLEDTog();
	}
}


//==========================================================================================
// USB Task
BEGIN_TASK(TaskUSB)
USBPortDescriptor.TxBuf = smallTxBuf;
USBPortDescriptor.RxBuf = smallRxBuf;

	USBPortDescriptor.TxBufSize = SMALL_TX_BUF_SIZE;
	USBPortDescriptor.RxBufSize = SMALL_RX_BUF_SIZE;
	USBPortDescriptor.TxBufMask = SMALL_TX_BUF_MASK;

	//osENABLE_TASK(TaskFlash);	// we'll need Flash access

	//------------------------------------------------------------------------------------------
	// prepare the USB USART
	if(!usbConfigured) {
		usb = &usb_str;
		USB_USART_PORT.DIRSET = PIN3_bm;  USB_USART_PORT.PIN3CTRL = PORT_OPC_WIREDAND_gc; // TXD is open collector output
		USB_USART_PORT.DIRCLR = PIN2_bm;// RXD is input

		USART_InterruptDriver_Initialize(&USBPortDescriptor, &USB_USART, USART_DREINTLVL_HI_gc); // setup USART, DRE interrupt priority is LOW
		USART_RxdInterruptLevel_Set(USBPortDescriptor.usart, USART_RXCINTLVL_HI_gc); // receive interrupt priority is LOW
		USART_Format_Set(USBPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N2

		//USBPortDescriptor.usart->CTRLB |= USART_CLK2X_bm;

		//USART_Baudrate_Set(USBPortDescriptor.usart, 34, 0); //921600 bps
		USART_Baudrate_Set(USBPortDescriptor.usart, 19, -4);
		USART_Rx_Enable(USBPortDescriptor.usart);
		USART_Tx_Enable(USBPortDescriptor.usart);
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

	while(1){
		//DPLEDOn();
		//static int startPage, startIndex, endPage, endIndex;

		USBWatchTimer = FLASH_WATCH_TIME; // longest command is erase all flash "ea" which can take up to 80 seconds, according to the data sheet.

		AnalogPowerOn();

/*
	#if ENABLE_DEBUG
		Verbose = EE_Rd(EE_VERBOSE);
		Debug = EE_Rd(EE_DEBUG);
		format = EE_Rd(EE_FORMAT);
		FlowControl = EE_Rd(EE_FLOW_CONTROL);
	#else
		Verbose = 0;
		Debug = 0;
		FlowControl = 0;
		format=FORMAT_BINARY;
	#endif
*/

		if(USBPortDescriptor.RxReady){	// we have a command
			//--------------------------------------------------------------------------
			if(USBPortDescriptor.RxBuf[0]==0){	// just a CR, no command, so send out operating summary
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
						
						InitADCs(); // start the ADCs and then we'll override a few settings
						ADC_Reference_Config(&ADCA, ADC_REFSEL_INT1V_gc); 		// temporarily use internal bandgap
						ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV256_gc);	// 128KSPS @ 32MHZ
						ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_BATT, 0);		// monitor battery voltage on ADCA
						ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_BATT, 0);		// monitor battery voltage on ADCA
						ADC_Ch_InputMux_Config(&ADCA.CH2, ADC_CH_UNUSED_0, 0);// monitor ground
						ADC_Ch_InputMux_Config(&ADCA.CH3, ADC_CH_UNUSED_0, 0);// monitor ground
						osWAIT(50);	// wait for power to stabilize
						ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm| ADC_CH3START_bm) );
						osWAIT(1); // wait for conversions to complete
						ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm| ADC_CH3START_bm) );
						osWAIT(1); // wait for conversions to complete
						VBatt = (ADCA.CH0.RES + ADCA.CH1.RES - ADCA.CH2.RES - ADCA.CH3.RES)/(8190.0-ADCA.CH2.RES - ADCA.CH3.RES)*4.2;

						AnalogPowerOff();
										
						print_usb("Battery voltage: %0.2f\n", VBatt);
				}


#if ENABLE_DEBUG
				//-------------------------------------
				// set the low battery threshold
				else if(strncmp(cmd,"lb",2)==0){
					if(argC){	// if there was a numeric argument, rescan the command string looking for an integer rather than a float
						if(data1<3.4) {
							data1=3.4;
							print_usb("Limiting to %1.3f",data1);
						}
						else if(data1>4.1) {
							data1=4.1;
							
							print_usb("Limiting to %1.3f",data1);
						}
						EE_Wr(EE_LB,data1*1000);
					}
					print_usb("Low Battery Threshold = %1.3fV",EE_Rd(EE_LB)/1000.0);
				}

				//-------------------------------------
				// report the flash CRC
				else if(strncmp(cmd,"cc",2)==0){
					/*
					if(EE_Rd(EE_APP_CRC) == SP_ApplicationCRC()){
						print_usb("CRC Okay = 0x%08lX", EE_Rd(EE_APP_CRC));
					}
					else{
						print_usb("CRC Fail: Computed = 0x%08lX, EEPROM = 0x%08lX", SP_ApplicationCRC(), EE_Rd(EE_APP_CRC));
					}
					*/
				}
				//-------------------------------------
				// enable/disable Verbose messages
				else if(strncmp(cmd,"ve",2)==0){
					if(argC){
						if(data1){
							Verbose = 1;
						}
						else{
							Verbose = 0;
						}
						EE_Wr(EE_VERBOSE, Verbose);
					}
					print_usb("Verbose messages = %d",Verbose);
				}

				//-------------------------------------
				// enable/disable debugging mode
				else if(strncmp(cmd,"db",2)==0){
					if(argC){
						if(data1){
							Debug = 1;
						}
						else{
							Debug = 0;
						}
					}
					EE_Wr(EE_DEBUG, Debug);
					print_usb("Debug mode = %d",Debug);
				}

				//-------------------------------------
				// enable/disable upload flow control
				else if(strncmp(cmd,"fc",2)==0){
					if(argC){
						if(data1){
							FlowControl = 1;
						}
						else{
							FlowControl = 0;
						}
						EE_Wr(EE_FLOW_CONTROL, FlowControl);
					}
					print_usb("Flow Control = %d",FlowControl);
				}

				//-------------------------------------
				// ulaw test
				else if(strncmp(cmd,"60",2)==0){
					#define LINE_FREQ 60
					#define SAMP_FREQ 250
					#define ADAPTATION_RATE 0.00125
					#define PI 3.1415926535897932384626433

					int i;	// index
					float a, a1=0, a2=0; // current and two previous line noise amplitudes
					float x, x1=0; // current and previous signal amplitudes
					float n = 2*cos(2*PI*LINE_FREQ/SAMP_FREQ);

					for(i = 0;i<2500;i++){
						x = sin(2*PI*LINE_FREQ/SAMP_FREQ*i);	// synthesize a sine wave as our signal, with amplitude of 1000 counts P-P
						a = n*a1-a2;	// compute current noise amplitude estimate
						if( ((x-a)-(x1-a1))>=0 ){ // remove noise estimate from signal
							a+=ADAPTATION_RATE;	//error is positive, increase the noise amplitude by ADAPTATION_RATE
						}
						else{
							a-=ADAPTATION_RATE;	//error is negative, decrease the noise amplitude by ADAPTATION_RATE
						}
						a2=a1;	// propagate the noise amplitudes
						a1=a;
						x1=x;	// propagate the signal amplitude

						print_usb("%d,%f,%f\r\n",i,x,x-a);	// print index, synthesized signal and noise reduced signal
						_delay_us(250);
						wdt_reset();
					}
				}

				//-------------------------------------
				// Set the data output format to decimal csv, for use with Excel
				else if(strncmp(cmd,"fd",2)==0){
					EE_Wr(EE_FORMAT,FORMAT_DECIMAL);
					format = FORMAT_DECIMAL;
					print_usb("Output format set to decimal (csv)");
				}

				//-------------------------------------
				// Set the data format to raw binary, for use by Brock
				else if(strncmp(cmd,"fb",2)==0){
					EE_Wr(EE_FORMAT,FORMAT_BINARY);
					format = FORMAT_BINARY;
					print_usb("Output format set to binary");
				}

				//-------------------------------------
				// read a page of Flash
				else if(strncmp(cmd,"rf",2)==0){
					if(argC){
						print_usb("Flash page %d\r\n",(int)data1);
						FlashPageNumber=data1;
						for(i=0;i<FLASH_PAGE_SIZE;i++){	// erase the buffer so we aren't fooled by a broken read
							Buf1.Data.Byte[i]=0xff;
						}
						if(TaskFlashCmd == TF_IDLE){
							TaskFlashCmd = TF_READ_BUF1;
							while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
								osWAIT(1);
							}
							for(i=0;i<FLASH_PAGE_SIZE;i++){
								print_usb("%02X ",Buf1.Data.Byte[i]);
								if(i%16==15){
									print_usb("\r\n");
									osWAIT(1);
								}
							}
							print_usb("Done");
						}
						else{
							print_usb("Flash Busy");
						}
					}
				}
				
				//-------------------------------------
				// write a page of Flash
				else if(strncmp(cmd,"wf",2)==0){
					if(argC==1){
						if(TaskFlashCmd == TF_IDLE){
							for(i=0;i<FLASH_PAGE_SIZE;i++){	// fill the buffer with a nice pattern
								Buf1.Data.Byte[i]=i;
							}
							FlashPageNumber=data1;
							TaskFlashCmd = TF_WRITE_BUF1;
							while(TaskFlashCmd != TF_IDLE){
								osWAIT(1);
							}
							print_usb("Done!");
						}
						else{
							print_usb("Flash Busy");
						}
					}
				}

				//-------------------------------------
				// set the abandonment time
				else if(strncmp(cmd,"at",2)==0){
					if(argC){
						if(data1>0 && data1<=600){
							EE_Wr(EE_ABANDON_TIME, data1);
						}
						else{
							print_usb("Valid range 1 to 600 seconds, ignoring.\r\n");
						}
					}
					print_usb("Abandon Time = %ds",EE_Rd(EE_ABANDON_TIME));
				}


				//-------------------------------------
				// Enable/Disable LF Bias Injection
				else if(strncmp(cmd,"lf",2)==0){
					if(argC){
						if(data1){
							EE_Wr(EE_LFBIAS_ENABLE, 1);
						}
						else{
							EE_Wr(EE_LFBIAS_ENABLE, 0);
						}
					}
					print_usb("LF Bias Enable = %d",EE_Rd(EE_LFBIAS_ENABLE));
				}


				//-------------------------------------
				// Set 1 bar baseline threshod
				else if(strncmp(cmd,"b1",2)==0){
					if(argC){
						if(data1){
							EE_Wr(EE_B1, data1);
						}
					}
					print_usb("B1 = %d",EE_Rd(EE_B1));
				}


				//-------------------------------------
				// Set 2 bar baseline threshod
				else if(strncmp(cmd,"b2",2)==0){
					if(argC){
						if(data1){
							EE_Wr(EE_B2, data1);
						}
					}
					print_usb("B2 = %d",EE_Rd(EE_B2));
				}


				//-------------------------------------
				// Set 3 bar baseline threshod
				else if(strncmp(cmd,"b3",2)==0){
					if(argC){
						if(data1){
							EE_Wr(EE_B3, data1);
						}
					}
					print_usb("B3 = %d",EE_Rd(EE_B3));
				}

				//-------------------------------------
				// Set default coefficients
				else if(strncmp(cmd,"de",2)==0){
					SetDefaults();
					print_usb("Defaults set");
				}

#endif

				//-------------------------------------
				// Erase All Flash via bulk erase command
				
				/*
				else if(strncmp(cmd,"ea",2)==0){
					MainPowerOn();			// just in case we lose USB
					EraseInProgress=1;	// keep auto power off from tripping if we lose USB
					print_usb("Erasing All Flash\r\n");
					if(TaskFlashCmd == TF_IDLE){
						TaskFlashCmd = TF_ERASE_ALL;
						i=0;
						do{
							osWAIT(100);
							Seg7HP(0x23);
							osWAIT(100);
							Seg7HP(0x24);
							osWAIT(100);
							Seg7HP(0x25);
							osWAIT(100);
							Seg7HP(0x26);
							osWAIT(100);
							Seg7HP(0x21);
							osWAIT(100);
							Seg7HP(0x22);
							print_usb(".");
							if(i++%16==15){
								print_usb("\r\n");
							}
						}while(TaskFlashCmd != TF_IDLE);	// wait for erase to complete
						ElapsedMinutes = 0;
						Seg7HP(' ');
						print_usb("\r\nDone");
						EraseInProgress=0;	// all auto power off on loss of USB
					}
					else{
						print_usb("Flash Busy");
					}
				}
				*/
				
				/*
				//-------------------------------------
				// Report length of the voice recording, in seconds
				else if(strncmp(cmd,"?v",2)==0){
					if(Verbose){
						print_usb("Voice record length = %1.1f sec.\r\n", floor((float)EE_Rd(EE_VOICE_RECORD_LENGTH)*0.016*10.0)/10.0 );
					}
					else{
						print_usb("%1.1f\r\n", floor((float)EE_Rd(EE_VOICE_RECORD_LENGTH)*0.016*10.0)/10.0 );
					}
				}
				
				//-------------------------------------
				// Report length of the ecg recording, in seconds
				else if(strncmp(cmd,"?e",2)==0){
					if(Verbose){
						print_usb("ECG record length = %1.1f sec.\r\n", floor((float)EE_Rd(EE_ECG_RECORD_LENGTH)*0.021*10)/10.0 );
					}
					else{
						print_usb("%1.1f\r\n", floor((float)EE_Rd(EE_ECG_RECORD_LENGTH)*0.021*10)/10.0 );
					}
				}
				
				//--------------------------------------------------------------------------
				// read 100ms of voice data
				// Each page of Flash holds 16ms of voice (16 uLaw bytes/ms)
				// So we must map the start and end points onto:
				//	 A starting page and offset
				//   An ending page and offset
				//	 A range of complete pages to read

				else if(strncmp(cmd,"rv",2)==0){
					if(argC){
						if(argC==1){	// only a start time was provided, dump one BLOCK_LEN of voice
							if(data1>VOICE_RECORD_LENGTH-BLOCK_LEN*2){
								data1 = VOICE_RECORD_LENGTH-BLOCK_LEN*2;
							}
							data2 = data1;
						}
						else{ // argC==2, we were given both a start time and an end time
							if(data2<data1){	// fix end<start
								data2 = data1;
							}
							else if(data2>VOICE_RECORD_LENGTH-BLOCK_LEN){// fix end bigger than record length
								data2 = VOICE_RECORD_LENGTH-BLOCK_LEN;
							}
							else{
								data2 = data2;
							}
						}

						// truncate times to 100ms resolution
						start = data1*10;
						end = data2*10;
						// convert to milliseconds
						start*=100;
						end*=100;
						// set end to the last milliscond of the 100ms block
						end+=99;

						// now we've got valid start and end times, convert that into flash page numbers and indexes
						startPage = start/VOICE_MS_PER_PAGE + FLASH_VOICE_START_PAGE;
						startIndex = start%VOICE_MS_PER_PAGE*16;
						endPage = end/VOICE_MS_PER_PAGE + FLASH_VOICE_START_PAGE;
						endIndex =  end%VOICE_MS_PER_PAGE*16;

						if(Verbose){
							print_usb("s%ld e%ld\r\n", start, end);
							print_usb("Start page  = %d\r\n",startPage);
							print_usb("Start index = %d\r\n",startIndex);
							print_usb("End page    = %d\r\n",endPage);
							print_usb("End index   = %d\r\n",endIndex);
						}

						crc = 0xffff;

						// dump voice data from startPage
						FlashPageNumber=startPage;
						while(TaskFlashCmd != TF_IDLE); // should never happen
						TaskFlashCmd = TF_READ_BUF1;
						while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
							osYIELD;
						}
						for(i=startIndex;i<VOICE_SAMPLES_PER_PAGE;i++){
							if(Debug){
								byte = i;
							}
							else{
								byte = Buf1.Data.Byte[i];
							}
							crc(byte);
							if(format == FORMAT_DECIMAL){
								print_usb("%d,",ulaw2linear(byte));
								if(i%VOICE_SAMPLES_PER_MS==VOICE_SAMPLES_PER_MS-1){
									print_usb("\r\n");
								}
							}
							else{ // FORMAT_BINARY
								put_byte(byte);
							}
						}
						osYIELD;	// mostly to keep the watchdog happy
						print_verbose("Start Page Done\r\n");

						// dump voice data from full pages between startPage and endPage
						while(++FlashPageNumber<endPage){
							while(TaskFlashCmd != TF_IDLE); // should never happen
							TaskFlashCmd = TF_READ_BUF1;
							while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
								osYIELD;
							}
							for(i=0;i<VOICE_SAMPLES_PER_PAGE;i++){
								if(Debug){
									byte = i;
								}
								else{
									byte = Buf1.Data.Byte[i];
								}
								crc(byte);
								if(format == FORMAT_DECIMAL){
									print_usb("%d,",ulaw2linear(byte));
									if(i%VOICE_SAMPLES_PER_MS==VOICE_SAMPLES_PER_MS-1){
										print_usb("\r\n");
									}
								}
								else{ // FORMAT_BINARY
									put_byte(byte);
								}
							}
							osYIELD;
							print_verbose("Interior page %d done\r\n",FlashPageNumber);
						}

						// dump voice data from endPage
						FlashPageNumber=endPage;
						while(TaskFlashCmd != TF_IDLE); // should never happen
						TaskFlashCmd = TF_READ_BUF1;
						while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
							osYIELD;
						}
						for(i=0;i<=endIndex+VOICE_SAMPLES_PER_MS-1;i++){
							if(Debug){
								byte = i;
							}
							else{
								byte = Buf1.Data.Byte[i];
							}
							crc(byte);
							if(format == FORMAT_DECIMAL){
								print_usb("%d,",ulaw2linear(byte));
								if(i%VOICE_SAMPLES_PER_MS==VOICE_SAMPLES_PER_MS-1){
									print_usb("\r\n");
								}
							}
							else{ // FORMAT_BINARY
								put_byte(byte);
							}
						}
						osYIELD;
						if(format == FORMAT_BINARY){
							put_byte(crc);
							put_byte(crc>>8);
						}
						else{
							print_usb("%04x", crc);
						}
							
						print_verbose("End page done\r\n");
					}
				}

				//--------------------------------------------------------------------------
				// Read ECG data
				// Each page of Flash holds 21ms of ECG (12 bytes/sample each ms)
				// So we must map the start and end points onto:
				//	 A starting page and offset
				//   An ending page and offset
				//	 A range of complete pages to read

				else if(strncmp(cmd,"re",2)==0){
					if(argC){
						if(argC==1){	// only a start time was provided, dump one BLOCK_LEN of ECG
							if(data1>ECG_RECORD_LENGTH-BLOCK_LEN*2){
								data1 = ECG_RECORD_LENGTH-BLOCK_LEN*2;
							}
							data2 = data1;
						}
						else if(argC==2){ // we were given both a start time and an end time
							if(data2<data1){	// fix end<start
								data2 = data1;
							}
							else if(data2>ECG_RECORD_LENGTH-BLOCK_LEN){// fix end bigger than record length
								data2 = ECG_RECORD_LENGTH-BLOCK_LEN;
							}
							else{
								data2 = data2;
							}
						}
						ecgLength = floor((float)EE_Rd(EE_ECG_RECORD_LENGTH)*0.021*10)/10.0;
						ecgLength -= 0.1f;

						// truncate times to 100ms resolution
						start = data1*10;
						end = data2*10;
						// convert to milliseconds
						start*=100;
						end*=100;
						// set end to the last milliscond of the 100ms block
						end+=99;

						if(Verbose && (format == FORMAT_BINARY)){
							print_usb("s%ld e%ld\r\n", start, end);
						}
						// now we've got valid start and end times, convert that into flash page numbers and indexes
						startPage = start/ECG_MS_PER_PAGE + FLASH_ECG_START_PAGE;
						startIndex = start%ECG_MS_PER_PAGE;
						endPage = end/ECG_MS_PER_PAGE + FLASH_ECG_START_PAGE;
						endIndex =  end%ECG_MS_PER_PAGE;

						if(Verbose){
							print_usb("Start page  = %d\r\n",startPage);
							print_usb("Start index = %d\r\n",startIndex);
							print_usb("End page    = %d\r\n",endPage);
							print_usb("End index   = %d\r\n",endIndex);
						}

						crc = 0xffff;

						// dump ECG data from startPage
						FlashPageNumber=startPage;
						while(TaskFlashCmd != TF_IDLE); // should never happen
						TaskFlashCmd = TF_READ_BUF1;
						while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
							osYIELD;
						}
						for(i=startIndex;i<ECG_SAMPLES_PER_PAGE;i++){
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
						
						#ifdef BLUETOOTH
						put_byte(Buf1.Data.ECG.Flag1);
						crc(Buf1.Data.ECG.Flag1);
						put_byte(Buf1.Data.ECG.Flag2);
						crc(Buf1.Data.ECG.Flag2);
						#endif
						
						osYIELD;
						print_verbose("Start Page Done\r\n");

						// dump ECG data from full pages between startPage and endPage
						while(++FlashPageNumber<endPage){
							while(TaskFlashCmd != TF_IDLE); // should never happen
							TaskFlashCmd = TF_READ_BUF1;
							while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
								osYIELD;
							}
							for(i=0;i<ECG_SAMPLES_PER_PAGE;i++){
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
							
							#ifdef BLUETOOTH
							put_byte(Buf1.Data.ECG.Flag1);
							crc(Buf1.Data.ECG.Flag1);
							put_byte(Buf1.Data.ECG.Flag2);
							crc(Buf1.Data.ECG.Flag2);
							#endif
							
							osYIELD;
							print_verbose("Interior page %d done\r\n",FlashPageNumber);
						}

						// dump ECG data from endPage
						FlashPageNumber=endPage;
						while(TaskFlashCmd != TF_IDLE); // should never happen
						TaskFlashCmd = TF_READ_BUF1;
						while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
							osYIELD;
						}
						for(i=0;i<=endIndex;i++){
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
							osYIELD;
						}
						
						// If we're at the last page of the request
						trueEnd = ecgLength*10;
						trueEnd*=100;
						trueEnd+=99;
						trueEndPage = trueEnd/ECG_MS_PER_PAGE + FLASH_ECG_START_PAGE;
						if(FlashPageNumber == trueEndPage || endIndex == ECG_SAMPLES_PER_PAGE-1) {
							// Dump the pace detection byte
							#ifdef BLUETOOTH
							put_byte(Buf1.Data.ECG.Flag1);
							crc(Buf1.Data.ECG.Flag1);
							put_byte(Buf1.Data.ECG.Flag2);
							crc(Buf1.Data.ECG.Flag2);
							#endif
						}
						
						if(format == FORMAT_BINARY){
							put_byte(crc);
							put_byte(crc>>8);
						}
						print_verbose("End page done\r\n");
					}
				}

				//-------------------------------------
				// report the elapsed time in minutes since ECG recording
				else if(strncmp(cmd,"?t",2)==0){
					print_usb("%d\r\n", ElapsedMinutes);
				}

				//-------------------------------------
				// erase the elapsed time in minutes since ECG recording
				else if(strncmp(cmd,"et",2)==0){
					ElapsedMinutes = 0;
				}
				else if(!strncmp("v+", cmd, 2)) {
					EE_Wr(EE_ENABLE_VOICE, 1);
				}
				else if(!strncmp("v-", cmd, 2)) {
					EE_Wr(EE_ENABLE_VOICE, 0);
				}
				*/
				else if(!strncmp("vr", cmd, 2)) {
					InitADCs(); // start the ADCs and then we'll override a few settings
					ADC_Reference_Config(&ADCA, ADC_REFSEL_INT1V_gc); 		// temporarily use internal bandgap
					ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV256_gc);	// 128KSPS @ 32MHZ
					ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_BATT, 0);		// monitor battery voltage on ADCA
					ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_BATT, 0);		// monitor battery voltage on ADCA
					ADC_Ch_InputMux_Config(&ADCA.CH2, ADC_CH_UNUSED_0, 0);// monitor ground
					ADC_Ch_InputMux_Config(&ADCA.CH3, ADC_CH_UNUSED_0, 0);// monitor ground
					osWAIT(50);	// wait for power to stabilize
					ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm| ADC_CH3START_bm) );
					osWAIT(1); // wait for conversions to complete
					ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm| ADC_CH3START_bm) );
					osWAIT(1); // wait for conversions to complete
					
					VBatt = (ADCA.CH0.RES + ADCA.CH1.RES - ADCA.CH2.RES - ADCA.CH3.RES)/(8190.0-ADCA.CH2.RES - ADCA.CH3.RES)*4.2;
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
#ifdef WORKING
				else if(!strncmp("fu", cmd, 2)) {
					EE_Wr(EE_LAST_CONNECTION_TYPE, CONNECTION_UNKNOWN);
				}

				else if(!strncmp("bt", cmd, 2)) {
					print_usb("Entering Bluetooth passthrough mode...\r\n");
					
					//osDISABLE_TASK(TaskBT);
					//BTOff();
					
					//iAP2Packet_t packet;
					/*
					USART_Rx_Disable(USBPortDescriptor.usart);
					USART_Tx_Disable(USBPortDescriptor.usart);
					USART_Format_Set(USBPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N2
					USART_Baudrate_Set(USBPortDescriptor.usart, 19, -4); //921600 bps
					USART_Rx_Enable(USBPortDescriptor.usart);
					USART_Tx_Enable(USBPortDescriptor.usart);
					
					USART_Rx_Disable(BTPortDescriptor.usart);
					USART_Tx_Disable(BTPortDescriptor.usart);
					USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N2
					USART_Baudrate_Set(BTPortDescriptor.usart, 19, -4); //921600 bps
					USART_Rx_Enable(BTPortDescriptor.usart);
					USART_Tx_Enable(BTPortDescriptor.usart);
					*/
					
					inBluetoothUpdateMode = 1;

					//RESET_TASK(TaskBT);
					osENABLE_TASK(TaskBT);
					BTOn();				
					
					Seg7LP('B');
					InitButton();
					USBWatchTimer = 0; // Keep us awake
					
					//osWAIT(250);
					//osWAIT(250);
					//osWAIT(250);
					//osWAIT(250);
					//print_bt("SET PROFILE IAP PROD:\"abc\" MF:\"def\" MDL:\"xyz\" SN:\"#12345\" PROTO:\"com.bluegiga.iwrap\" SEED:\"1234567890\" APP:\"com.spauldingclinical.IQApp\" FW:5.0.0 HW:1.0.0 CAPS:0 RF:0 VCD:05 MODE0 PKT2K\r");				
					//print_bt("RESET\r");
					osWAIT(250);
					osWAIT(250);
					osWAIT(250);
					osWAIT(250);
					osWAIT(250);
					//print_bt("SET CONTROL AUTOCALL\r");
					//osWAIT(250);	
					while(!ButtonPressed()) {	
						if(PORTF.IN & PIN7_bm) {
							//uart_putchar(&BTPortDescriptor,appleString[0]);
							//uart_putchar(&BTPortDescriptor,appleString[1]);
							//uart_putchar(&BTPortDescriptor,appleString[2]);
							//uart_putchar(&BTPortDescriptor,appleString[3]);
							//uart_putchar(&BTPortDescriptor,appleString[4]);
							//uart_putchar(&BTPortDescriptor,appleString[5]);
							//print_bt("TEST");
														
						}
						
						
						//if(BTPortDescriptor.RxReady) {
							//for(int i = 0; i < RX_BUF_SIZE; i++)
							//	print_usb("%x ", BTPortDescriptor.RxBuf[i]);
								
							//print_usb("%s ", BTPortDescriptor.RxBuf);					
							
							//print_usb("\r\n\r\n");
							
							//if(iniAPMode) {
							//	getApplePacket(BTPortDescriptor.RxBuf);
							//	printApplePacket(recievedPacket);
							//	checkRecievedPacketAndSendNext();
							//}
							
							//else {
							//									for(int i = 0; i < RX_BUF_SIZE; i++)
							//		print_usb("%x ", BTPortDescriptor.RxBuf[i]);
								
							//}
							//memset(BTPortDescriptor.RxBuf, 0, RX_BUF_SIZE);						
							
							//BTPortDescriptor.RxReady = 0;
						//}
						
						//print_usb("Connection type = %s\r\n", (connectionType == CONNECTION_APPLE ? "APPLE" : "ANDROID"));
						osWAIT(1);
					}
					print_usb("Exiting Bluetooth passthrough mode...\r\n");
					inBluetoothUpdateMode = 0;
					
					BTOff();
				}
				else if(!strncmp("b9", cmd, 2)) {
					osDISABLE_TASK(TaskBT);
					
					BT_USART_PORT.DIRSET = PIN3_bm;// TXD is output
					BT_USART_PORT.DIRCLR = PIN2_bm;// RXD is input
										
					USART_Rx_Disable(BTPortDescriptor.usart);
					USART_Tx_Disable(BTPortDescriptor.usart);
					USART_InterruptDriver_Initialize(&BTPortDescriptor, &BT_USART, USART_DREINTLVL_LO_gc); // setup USART, DRE interrupt priority is LOW
					USART_RxdInterruptLevel_Set(BTPortDescriptor.usart, USART_RXCINTLVL_LO_gc); // receive interrupt priority is LOW
					USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_EVEN_gc, true);	//8N2
					USART_Baudrate_Set(BTPortDescriptor.usart, 33, -1); //115200 bps
					USART_Rx_Enable(BTPortDescriptor.usart);
					USART_Tx_Enable(BTPortDescriptor.usart);
										
					BTOn();
										
					osWAIT(250);
					osWAIT(250);
					osWAIT(250);
					osWAIT(250);								
										
					print_bt("SET CONTROL BAUD 921600,8n1\r");
					
					osWAIT(250);
					osWAIT(250);
					osWAIT(250);
					osWAIT(250);
							
					BTOff();
					
					print_usb("Bluetooth successfully set to 921600,8n1\r");
				}
				else if(!strncmp("b1", cmd, 2)) {
					osDISABLE_TASK(TaskBT);
					
					BT_USART_PORT.DIRSET = PIN3_bm;// TXD is output
					BT_USART_PORT.DIRCLR = PIN2_bm;// RXD is input
										
					USART_Rx_Disable(BTPortDescriptor.usart);
					USART_Tx_Disable(BTPortDescriptor.usart);
					USART_InterruptDriver_Initialize(&BTPortDescriptor, &BT_USART, USART_DREINTLVL_LO_gc); // setup USART, DRE interrupt priority is LOW
					USART_RxdInterruptLevel_Set(BTPortDescriptor.usart, USART_RXCINTLVL_LO_gc); // receive interrupt priority is LOW
					USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N2
					USART_Baudrate_Set(BTPortDescriptor.usart, 19, -4); //921600 bps
					USART_Rx_Enable(BTPortDescriptor.usart);
					USART_Tx_Enable(BTPortDescriptor.usart);
										
					BTOn();
										
					osWAIT(250);
					osWAIT(250);
					osWAIT(250);
					osWAIT(250);										
										
					print_bt("SET CONTROL BAUD 115200,8e1\r");
					
					osWAIT(250);
					osWAIT(250);
					osWAIT(250);
					osWAIT(250);		
					
					BTOff();
					
					print_usb("Bluetooth successfully set to 115200,8e1\r");
				}
				else if(!strncmp("bu", cmd, 2)) {
					osDISABLE_TASK(TaskBT);
								
					USART_Rx_Disable(USBPortDescriptor.usart);
					USART_Tx_Disable(USBPortDescriptor.usart);
					USART_Format_Set(USBPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_EVEN_gc, false);	//8N2
					USART_Baudrate_Set(USBPortDescriptor.usart, 33, -1); //921600 bps
					// 115200
					USART_Rx_Enable(USBPortDescriptor.usart);
					USART_Tx_Enable(USBPortDescriptor.usart);
					
										
					BT_USART_PORT.DIRSET = PIN3_bm;// TXD is output
					BT_USART_PORT.DIRCLR = PIN2_bm;// RXD is input
								
					USART_Rx_Disable(BTPortDescriptor.usart);
					USART_Tx_Disable(BTPortDescriptor.usart);
					USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_EVEN_gc, false);	//8N2
					USART_Baudrate_Set(BTPortDescriptor.usart, 33, -1); //115200 bps
					//USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/BT_BAUD_RATE-1), 0);  // Does not work for all Baud Rates (BAUD_RATE = 921600 for example)
					USART_Rx_Enable(BTPortDescriptor.usart);
					USART_Tx_Enable(BTPortDescriptor.usart);
					
					BTOn();
					
						
					inBluetoothUpdateMode = 1;
					
					Seg7LP('B');
					InitButton();
					USBWatchTimer = 0; // Keep us awake
					
					while(inBluetoothUpdateMode) {
						if(ButtonPressed()) {
							inBluetoothUpdateMode = 0;
						}							
						osWAIT(1);
					}
					inBluetoothUpdateMode = 0;
					
					USART_Rx_Disable(USBPortDescriptor.usart);
					USART_Tx_Disable(USBPortDescriptor.usart);
					USART_Format_Set(USBPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N2
					USART_Baudrate_Set(USBPortDescriptor.usart, 19, -4); //921600 bps
					// 115200
					USART_Rx_Enable(USBPortDescriptor.usart);
					USART_Tx_Enable(USBPortDescriptor.usart);		
				}
				else if(!strncmp("ad", cmd, 2)) {
					InitADCs();
					StartAcq(ACQ_IRQ_ECG);
					//ADC_Reference_Config(&ADCA, ADC_REFSEL_INT1V_gc); 		// temporarily use internal bandgap
					USBWatchTimer = 0;

					while(!ButtonPressed()) {
						osWAIT(50);	// wait for power to stabilize

						float delta = 4095 * 0.05f;
						float zeroVolts = 4095.0f/2.0f - delta;
						int voltage = 0;
						//int voltage = ((RL_Mon-zeroVolts-delta)/(4095.0f-delta))*3.3f*1000;
						
						//VBatt = (ADCB.CH0.RES + ADCB.CH1.RES - ADCA.CH2.RES - ADCA.CH3.RES)/(8190.0-ADCA.CH2.RES - ADCA.CH3.RES)*4.2;

						print_usb("\r%d %d %f %d %dmv", ADCB.CH0.RES, ADCA.CH2.RES, delta, voltage, BTLeadOff);
					}

					StopAcq();
				}
			else if(!strncmp("sp", cmd, 2)) {
					//LFBiasOn();
					acqState = FSM_PREACQ;
					
					InitADCs(); // start the ADCs and then we'll override a few settings
					//ADC_Reference_Config(&ADCB, ADC_REFSEL_VCC_gc); 		    // temporarily use internal bandgap
					//ADC_Prescaler_Config(&ADCB, ADC_PRESCALER_DIV256_gc);	    // 128KSPS @ 32MHZ
					//ADC_Ch_InputMux_Config(&ADCB.CH0, ADC_CH_V1, 0);		// monitor battery voltage on ADCA
					//ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_BATT, 0);		// monitor battery voltage on ADCA
					//ADC_Ch_InputMux_Config(&ADCA.CH2, ADC_CH_UNUSED_0, 0); // monitor ground
					//ADC_Ch_InputMux_Config(&ADCA.CH3, ADC_CH_UNUSED_0, 0); // monitor ground
					osWAIT(50);	// wait for power to stabilize
					USBWatchTimer = 0; // Keep us awak

					StartAcq(ACQ_IRQ_ECG);
					osENABLE_TASK(TaskBT); // Make sure BT the task is running
					BTBufPhase = 0;
					BTStatus = 0;
					BTEnable = 1;
					BTOn(); // if BT got killed, turn it back on
					
					//PORTC.DIRSET = PIN4_bm; PORTC.PIN4CTRL = PORT_OPC_TOTEM_gc;
				//for(i = 0; i < 1000; i++) {
					
					//currentFFTLead = selectedLead;
					//LFBiasOn();
					
					static volatile short min = 0, max = 0;
					static volatile unsigned char needMinMax = 1;
					static volatile unsigned short counter = 0;
					static volatile double accumulator = 0;
					static volatile float alpha = 0.00005f;
					static volatile int16_t compressed = 0;
					static volatile int8_t temp[3];
					while(!ButtonPressed()) {
						temp[0] = Lead[V3]  >> 8;
						temp[1] = (Lead[V3] & 0xF0) | ((0 >> 4) & 0x0F);
						temp[2] = 0 >> 8;

						compressed = (int16_t) ((temp[0] << 4) | ((temp[1] & 0xf0) >> 4));
						//compressed = (Lead[V3] >> 4);

						if(needMinMax) {
							min = max = compressed;
							needMinMax = 0;
						} else {
							if(compressed < min) min = compressed;
							if(compressed > max) max = compressed;
						}

						accumulator = (alpha * compressed) + (1.0 - alpha) * accumulator;

						if(++counter == 1000) {
							counter = 0;
							print_usb("Min = %d, Max = %d, Avg = %lf\r\n", min, max, accumulator);
						}
						//wdt_reset();
						//ADCB.CH2.RES
						//print_usb("%04x\r\n", Lead[V1]);
																													
						//osYIELD;
						osWAIT(1);
					}
					
					StopAcq();
				//}
				//ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm| ADC_CH3START_bm) );
				//osWAIT(1); // wait for conversions to complete
				//VBatt = (ADCA.CH0.RES + ADCA.CH1.RES - ADCA.CH2.RES - ADCA.CH3.RES)/(8190.0-ADCA.CH2.RES - ADCA.CH3.RES)*4.2;
			}
			else if(!strncmp("br", cmd, 2)) {
					// Change baudrate
					osDISABLE_TASK(TaskBT);
					BTOff();
					//USART_Rx_Disable(BTPortDescriptor.usart);
					//USART_Tx_Disable(BTPortDescriptor.usart);
					USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N1
					USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/BT_BAUD_RATE-1), 0);
					//USART_Rx_Enable(BTPortDescriptor.usart);
					//USART_Tx_Enable(BTPortDescriptor.usart);
					
					BTOn();
					
					for(i = 0; i < 50; i++) {
						print_bt("SET CONTROL BAUD 921600,8n1\r");
						osWAIT(50);
					}
					
					BTOff();
					
					//USART_Rx_Disable(BTPortDescriptor.usart);
					//USART_Tx_Disable(BTPortDescriptor.usart);
					USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc,	false);	//8N1
					USART_Baudrate_Set(BTPortDescriptor.usart, 19, -4); //921600 bps
					//USART_Rx_Enable(BTPortDescriptor.usart);
					//USART_Tx_Enable(BTPortDescriptor.usart);
					
					osENABLE_TASK(TaskBT);
				}
			// Command to start a new simple Bluetooth session for Apple testing
			else if(!strncmp("ap", cmd, 2)) {
				resetAppleVariables();
				resetUsartAppleVariables();
				
				USBWatchTimer = 0;
				StartAcq(ACQ_IRQ_ECG);
				osENABLE_TASK(TaskBT); // Make sure BT the task is running
				BTBufPhase = 0;
				BTStatus = 0; 
				BTOn(); // if BT got killed, turn it back on
				while(!ButtonPressed()) {
					// Just keep us awake for testing
					osWAIT(1);
				}
				
			}
#endif


				//-------------------------------------
				// we didn't recognize a command, say ?
				else{
					print_usb("?");
				}
			} // if we have a command, parse it
			print_usb("\r\n>");	// put out the command line prompt
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
