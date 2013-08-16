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
#include "taskflash.h"
#include "taskacq.h"
#include "eeprom.h"
#include "ulaw.h"
#include "crc.h"
#include "seg7.h"
#include "TaskWatchDog.h"

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

static FILE usb_str = FDEV_SETUP_STREAM(put_usb, 0, _FDEV_SETUP_WRITE);
FILE *usb;

unsigned char Base64[64] PROGMEM = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
unsigned char Verbose = 0;	// used to control whether print_debug() messages are printed or ignored
unsigned char Debug = 0; // used to control print_debug() statements
unsigned char FlowControl;
unsigned char EraseInProgress = 0;

// macro to make printing stuff a little easier
#define print_usb(fmt,...)  fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)
#define print_debug(fmt,...) if(Debug) fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)
#define print_verbose(fmt,...) if(Verbose) fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)

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

static unsigned char format;
static unsigned int crc;

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

//------------------------------------------------------------------------------------
void Hello(void){
	print_usb("\r\n\r\nSpaulding IQ BV:%d FV:%1.2f HV:%d SN:9-%02d-%02d-%04d-101\r\n", (int)EE_Rd(EE_BOOT_VERSION), VERSION, MCU.JTAGUID, (int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000));
}


//==========================================================================================
// USB Task
BEGIN_TASK(TaskUSB)

	osENABLE_TASK(TaskFlash);	// we'll need Flash access

	//------------------------------------------------------------------------------------------
	// prepare the USB USART

	usb = &usb_str;
	USB_USART_PORT.DIRSET = PIN3_bm;  USB_USART_PORT.PIN3CTRL = PORT_OPC_WIREDAND_gc; // TXD is open collector output
	USB_USART_PORT.DIRCLR = PIN2_bm;// RXD is input

	USART_InterruptDriver_Initialize(&USBPortDescriptor, &USB_USART, USART_DREINTLVL_LO_gc); // setup USART, DRE interrupt priority is LOW
	USART_RxdInterruptLevel_Set(USBPortDescriptor.usart, USART_RXCINTLVL_LO_gc); // receive interrupt priority is LOW
	USART_Format_Set(USBPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N2
	USART_Baudrate_Set(USBPortDescriptor.usart, 19, -4); //921600 bps
	USART_Rx_Enable(USBPortDescriptor.usart);
	USART_Tx_Enable(USBPortDescriptor.usart);


	//==========================================================================================
	// USB command parser


	static unsigned char argC;
	static char cmd[3];
	static float data1,data2;
	static unsigned int i;
	static unsigned char j;
	static unsigned long start, end;
	static unsigned char byte;
	
	static float ecgLength;
	static unsigned long trueEnd;
	static int trueEndPage;

	Hello();

	while(1){

		static int startPage, startIndex, endPage, endIndex;

		USBWatchTimer = FLASH_WATCH_TIME; // longest command is erase all flash "ea" which can take up to 80 seconds, according to the data sheet.

		AnalogPowerOn();

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

		if(USBPortDescriptor.RxReady){	// we have a command
			//--------------------------------------------------------------------------
			if(USBPortDescriptor.RxBuf[0]==0){	// just a CR, no command, so send out operating summary
				Hello();
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
					while(1); // wait for watchdog timer to get us
				}

				//-------------------------------------
				// accept a query to set/get our serial number
				else if(strncmp(cmd,"sn",2)==0){
					if(argC){	// if there was a numeric argument, rescan the command string looking for an integer rather than a float
						argC=sscanf_P( (const char *)USBPortDescriptor.RxBuf,PSTR("%2s%ld"), &cmd, &start);
						EE_Wr(EE_SERIAL_NUM,start);
					}

					print_usb("SN:9-%02d-%02d-%04d-101",(int)(EE_Rd(EE_SERIAL_NUM)/1000000), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000));
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
				// print the battery voltage
				else if(strncmp(cmd,"vb",2)==0){
					print_usb("VBatt = %f/r/n",VBatt);
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

				#define BLOCK_LEN 0.1	// set the size of a dumped block, in seconds
				#define BLOCK_LEN_MS (BLOCK_LEN*1000)	// set the size of a dumped block, in milliseconds

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
						
						put_byte(Buf1.Data.ECG.Flag1);
						crc(Buf1.Data.ECG.Flag1);
						
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
							
							put_byte(Buf1.Data.ECG.Flag1);
							crc(Buf1.Data.ECG.Flag1);
							
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
						if(FlashPageNumber == trueEndPage) {
							// Dump the pace detection byte
							put_byte(Buf1.Data.ECG.Flag1);
							crc(Buf1.Data.ECG.Flag1);
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
