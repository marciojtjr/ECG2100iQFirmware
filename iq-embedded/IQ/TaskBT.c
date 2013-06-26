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
#include "taskacq.h"
#include "taskflash.h"
#include "buffers.h"
#include "crc.h"

#define LOGGING

#define BT_USART			USARTE0
#define BT_USART_PORT PORTE

unsigned char BTBufReady;
unsigned int BTBuf[8];
char logBuffer[109];
unsigned char logStatus = LOG_IDLE;

void LogMessage(char *message) {
	logStatus = LOG_WAITING;

	strncpy(logBuffer, message, 109);

	while(logStatus != LOG_IDLE)
		osYIELD;
}


//##########################################################################################
// define and allocate globals for the bluetooth task

USART_desc_t BTPortDescriptor;	// a descriptor for the USART connected to the Bluetooth transceiver
static int put_bt( char c, FILE *stream){
	uart_putchar(&BTPortDescriptor,c);
	return(0);
}

#define put_byte(c) uart_putchar(&BTPortDescriptor,c)

static FILE bt_str = FDEV_SETUP_STREAM(put_bt, 0, _FDEV_SETUP_WRITE);
FILE *bt;

void BTOn(void){
	PORTF.DIRSET = PIN0_bm;
	PORTF.OUTSET = PIN0_bm;
	PORTF.PIN0CTRL = PORT_OPC_TOTEM_gc;
}

void BTOff(void){
	PORTF.DIRSET = PIN0_bm;
	PORTF.OUTCLR = PIN0_bm;
	PORTF.PIN0CTRL = PORT_OPC_TOTEM_gc;
}

unsigned char Hex[16] PROGMEM = {"0123456789ABCDEF"};
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
	#define BT_BAUD_RATE 921600
	bt = &bt_str;
	BT_USART_PORT.DIRSET = PIN3_bm;// TXD is output
	BT_USART_PORT.DIRCLR = PIN2_bm;// RXD is input

	USART_InterruptDriver_Initialize(&BTPortDescriptor, &BT_USART, USART_DREINTLVL_LO_gc); // setup USART, DRE interrupt priority is LOW
	USART_RxdInterruptLevel_Set(BTPortDescriptor.usart, USART_RXCINTLVL_LO_gc); // receive interrupt priority is LOW
	USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);	//8N2
	USART_Baudrate_Set(BTPortDescriptor.usart, 19, -4); //921600 bps
//	USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/BT_BAUD_RATE-1), 0);  // Does not work for all Baud Rates (BAUD_RATE = 921600 for example)
	USART_Rx_Enable(BTPortDescriptor.usart);
	USART_Tx_Enable(BTPortDescriptor.usart);
  
    unsigned char ix = 0;
    static unsigned int FrameIndex = 0;
    short crc;
	static unsigned char firstTime = 1;

    BTString[ix++] = 0xAA;  // Preamble byte 1
    BTString[ix++] = 0x55;  // Preamble byte 2
    BTString[ix++] = 0xB3;  // Preamble byte 3
    BTString[ix++] = 0x3B;  // Preamble byte 4
    BTString[ix++] = 0xAF;  // Preamble byte 5
    BTString[ix++] = 0xFA;  // Preamble byte 6

    #define BT_PREAMBLE   0
    #define BT_TYPE       6
    #define BT_STATUS     7
    #define BT_LEADOFF    8
    #define BT_FRMINDEX   9
    #define BT_DATA0     11
    #define BT_DATA1     59
    #define BT_CRC      107

    #define BT_SIZE     109
   
	//==========================================================================================
	// Bluetooth command parser

	if( (MCU.JTAGUID != 0xff) && !USBPresent() ){	// we can run Bluetooth only on non rev 1 hardware (which has unprogrammed JTAG UID)
		BTOn();
     
		while(1)
            {
#ifndef COMPARE_VERSION
			if(BTReady && BTEnable)
                {	// we have a lead fail model to decimate
#ifndef LOGGING
				BTReady=0;
#endif

				//if(logStatus == LOG_WAITING) {
				//	char out = '@';
				//	fwrite(&out, 1, 1, bt);
				//	logStatus = LOG_IDLE;
				//}

				if(BTPortDescriptor.RxReady) {
					char c = '+';
					//sscanf( (const char *)BTPortDescriptor.RxBuf, "%c", &c);
					fwrite((const char *)BTPortDescriptor.RxBuf, 255, 1, bt);
					BTPortDescriptor.RxReady=0;
					//print_bt("%c", c);

					if(firstTime) {
						//osWAIT(255);
						//osWAIT(255);
						//osWAIT(255);
						//osWAIT(255);
						//fwrite(&c, 1, 1, bt);
						//fwrite(&c, 1, 1, bt);
						//fwrite(&c, 1, 1, bt);
						//osWAIT(255);
						//osWAIT(255);
						//osWAIT(255);
						//osWAIT(255);

						//fwrite("at\n", 4, 1, bt);
						//fwrite("set", 2, 1, bt);

						firstTime = 0;
					}
				}

#ifndef LOGGING
				// Bluetooth output will be at 125Hz, so we'll need to average eigth 1KHz samples, which we can do by adding up all the lead fail subsamples for two lead fail cycles

				unsigned char m = M^1;	// use the LFBiasModel NOT being filled at the moment

				// first phase of Bluetooth sample production, we add up all four 16 bit samples from the lead-fail bias model, after scaling them back to 12 bits.
				if(BTBufPhase==0)
                    {
                    unsigned char *btsp = &BTString[BT_DATA0];
                    for (unsigned char k=0; k<4; k++)
                        {
                        for (unsigned char lead=0; lead<8; lead+=2)
                            {
                            btsp[0] = LFBiasModel[m][lead][k] >> 8;
                            btsp[1] = (LFBiasModel[m][lead][k] & 0xF0) | ((LFBiasModel[m][lead+1][k] >> 4) & 0x0F);
                            btsp[2] = LFBiasModel[m][lead+1][k] >> 8;
                            btsp += 3;
                            }
                        }

                    /*
                    for(unsigned char i=0; i<8; i++)
                        {	// do all channels
                        BTBuf[i] = ( LFBiasModel[m][i][0]/16 + LFBiasModel[m][i][1]/16 + LFBiasModel[m][i][2]/16 + LFBiasModel[m][i][3]/16);
                        }
                    */
					BTBufPhase=1;
				    }
				
				// second phase of Bluetooth sample production, we add in the next four LFBiasModel subsamples, after scaling them back to 12 then average the eight subsamples (/8)
				else
                    {
                    unsigned char *btsp = &BTString[BT_DATA1];
                    for (unsigned char k=0; k<4; k++)
                        {
                        for (unsigned char lead=0; lead<8; lead+=2)
                            {
                            btsp[0] = LFBiasModel[m][lead][k] >> 8;
                            btsp[1] = (LFBiasModel[m][lead][k] & 0xF0) | ((LFBiasModel[m][lead+1][k] >> 4) & 0x0F);
                            btsp[2] = LFBiasModel[m][lead+1][k] >> 8;
                            btsp += 3;
                            }
                        }
                    /*
					for(unsigned char i=0;i<8;i++)
                        {	// do all channels
						BTBuf[i] = (BTBuf[i]+( LFBiasModel[m][i][0]/16 + LFBiasModel[m][i][1]/16 + LFBiasModel[m][i][2]/16 + LFBiasModel[m][i][3]/16)) /8;
					    }
                    */
					BTBufPhase=0;
                    /*
					// now we have eight channels of 125Hz data, ready to go, send three hex bytes for each 12 bit channel, while also accumulating all channels for a checksum
					unsigned char j=0;
					int cs = 0;
					BTString[j++]='*';
					for(unsigned char i=0;i<8;i++)
                        {	// do all channels
						cs+=BTBuf[i];
						BTString[j++]=bin2Hex(BTBuf[i]>>8);
						BTString[j++]=bin2Hex(BTBuf[i]>>4);
						BTString[j++]=bin2Hex(BTBuf[i]);
					    }
					BTString[j++]=bin2Hex(cs>>4);
					BTString[j++]=bin2Hex(cs);
					BTString[j++]=13; // carriage return
					BTString[j++]=0;
					print_bt("%s", BTString);
                    */

                    BTString[BT_TYPE] = 1;     // Record type
                    BTString[BT_STATUS] = BTStatus;     // Status Flags
                    BTString[BT_LEADOFF] = 0;     // Lead Off Indicator Bits
                    BTString[BT_FRMINDEX] = FrameIndex >> 8;     // Lead Off Indicator Bits
                    BTString[BT_FRMINDEX+1] = FrameIndex;     // Lead Off Indicator Bits

                    crc = 0;
                    for (unsigned char jx = BT_TYPE; jx < BT_CRC; jx++)
                        crc = CRCCalc(crc, BTString[jx]);

                    BTString[BT_CRC] = crc >> 8;
                    BTString[BT_CRC+1] = crc;
                    /*
					for (jx = 0; jx < ix; jx++)
						print_bt("%02X",BTString[jx]);
					print_bt("\n");
					*/

					//fwrite(BTString, BT_SIZE, 1, bt);

					FrameIndex++;
				    }
#endif
#endif

#ifdef COMPARE_VERSION
if(BTEnable) {
	for(int i = 0; i < 255; i++)
		put_byte('@');
	BTEnable = 0;
#endif

#ifndef COMPARE_VERSION
			if(BTEnable)
                {	// we have a lead fail model to decimate
					#define BLOCK_LEN 0.1	// set the size of a dumped block, in seconds
					#define BLOCK_LEN_MS (BLOCK_LEN*1000)	// set the size of a dumped block, in milliseconds

					unsigned char Verbose = 0;	// used to control whether print_debug() messages are printed or ignored
					unsigned char Debug = 0; // used to control print_debug() statements

					static unsigned char argC;
					static char cmd[3];
					static float data1,data2;
					static unsigned int i;
					static unsigned char j;
					static unsigned long start, end;
					static unsigned char byte;

					static unsigned char format;
					static unsigned int crc;

					static int startPage, startIndex, endPage, endIndex;

					enum { FORMAT_DECIMAL, FORMAT_BINARY }; // various file output formats

					argC = 2;
					data1 = 0.0f;
					data2 = 9.9f;

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

					// truncate times to 100ms resolution
					start = data1*10;
					end = data2*10;
					// convert to milliseconds
					start*=100;
					end*=100;
					// set end to the last milliscond of the 100ms block
					end+=99;

					if(Verbose && (format == FORMAT_BINARY)){
						print_bt("s%ld e%ld\r\n", start, end);
					}
					// now we've got valid start and end times, convert that into flash page numbers and indexes
					startPage = start/ECG_MS_PER_PAGE + FLASH_ECG_START_PAGE;
					startIndex = start%ECG_MS_PER_PAGE;
					endPage = end/ECG_MS_PER_PAGE + FLASH_ECG_START_PAGE;
					endIndex =  end%ECG_MS_PER_PAGE;

					if(Verbose){
						print_bt("Start page  = %d\r\n",startPage);
						print_bt("Start index = %d\r\n",startIndex);
						print_bt("End page    = %d\r\n",endPage);
						print_bt("End index   = %d\r\n",endIndex);
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
								print_bt("%02X ",Buf1.Data.ECG.Sample[i][j]);
							}
							print_bt("\r\n");
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
					osYIELD;
					print_bt_verbose("Start Page Done\r\n");

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
									print_bt("%02X ",Buf1.Data.ECG.Sample[i][j]);
								}
								print_bt("\r\n");
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
						osYIELD;
						print_bt_verbose("Interior page %d done\r\n",FlashPageNumber);
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
								print_bt("%02X ",Buf1.Data.ECG.Sample[i][j]);
							}
							print_bt("\r\n");
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
					if(format == FORMAT_BINARY){
						put_byte(crc);
						put_byte(crc>>8);
					}

					BTEnable = 0;						
#endif
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

#endif //BLUETOOTH
