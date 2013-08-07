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

#include "crc.h"

unsigned char BTBufReady;
unsigned int BTBuf[8];
unsigned char transferingFullData = 0;
unsigned char transferingDataType = FULL_TRANSFER_NONE;
char BTStateString[BT_STATE_SIZE];

enum { FORMAT_DECIMAL, FORMAT_BINARY }; // various file output formats
static unsigned char format = FORMAT_BINARY;

int print_bt_string(char *string) {
	return fprintf(bt, string);
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
	USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N2
	USART_Baudrate_Set(BTPortDescriptor.usart, 19, -4); //921600 bps
	//USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/BT_BAUD_RATE-1), 0);  // Does not work for all Baud Rates (BAUD_RATE = 921600 for example)
	USART_Rx_Enable(BTPortDescriptor.usart);
	USART_Tx_Enable(BTPortDescriptor.usart);
  
    unsigned char ix = 0;
    static unsigned int FrameIndex = 0;
#ifdef COMPARE_VERSION
    short crc;
#else
	static unsigned int crc;
#endif
	static unsigned char sent = 0x00;
	static uint32_t waiting = 0;
	static uint32_t beforeSend = 0;

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
	
/* FOR PACE DETECTION */


    #define BT_PREAMBLE   0
    #define BT_TYPE       6
    #define BT_STATUS     7
    #define BT_LEADOFF    8
	#define BT_PACESPIKE  9
    #define BT_FRMINDEX   10
    #define BT_DATA0     12
    #define BT_DATA1     60
    #define BT_CRC      108

    #define BT_SIZE     110


/* FOR NOT PACE DETECTION */

/*
	#define BT_PREAMBLE   0
	#define BT_TYPE       6
	#define BT_STATUS     7
	#define BT_LEADOFF    8
	#define BT_FRMINDEX   9
	#define BT_DATA0     11
	#define BT_DATA1     59
	#define BT_CRC      107

	#define BT_SIZE     109
*/

	
	static int firstTime = 0;
	static int timer = 0;
	
//#ifdef COMPARE_VERSION
	static float secondsOfData = 0;
	static unsigned int i;
	static unsigned char j;
	static unsigned long start, end;
	static float data1,data2;
	static unsigned char argC;
	static char cmd[3];

	static int startPage, startIndex, endPage, endIndex;
		
	#define BLOCK_LEN 0.1	// set the size of a dumped block, in seconds
	#define BLOCK_LEN_MS (BLOCK_LEN*1000)	// set the size of a dumped block, in milliseconds
//#endif

	//==========================================================================================
	// Bluetooth command parser

	if( (MCU.JTAGUID != 0xff) && !USBPresent() ){	// we can run Bluetooth only on non rev 1 hardware (which has unprogrammed JTAG UID)
		// Wait until we're in preacq before turning on the radio
		// Or if we're full, let it go through as well
		while(acqState < FSM_PREACQ && acqState != FSM_FULL) {
			osWAIT(1);
		}			
		
		BTOn();
		
		// Wait until we've recieved "READY" from the BT chip
		while(readyForCommands == 0) {
			osWAIT(1);
		}			
		
		// Set PIO5 as the CD pin
		print_bt("SET CONTROL CD 20 1\r");
     
		while(1) {		
#if !defined(COMPARE_VERSION) && !defined(LOGGING_VERSION)
			if(!BTEnable && acqState != FSM_FULL) {
				
				// Wait until the CD pin goes high (which happens when we enter data mode)
				if(PORTF.IN & PIN7_bm) {
					BTEnable = 1;
					
					// Clear any status bits that have been set
					BTStatus = 0;
					
					BTStateString[6] = acqState;
					fwrite(BTStateString, BT_STATE_SIZE, 1, bt);
				}
				
			}	
			else if(BTEnable) {
				if(BTPortDescriptor.RxReady) {
					
					// If we have 'ba' in the Rx buffer, it's the command to begin acquisition
					if(!strncmp("ba", BTPortDescriptor.RxBuf, 2) && acqState == FSM_PREACQ) {
						acqState = FSM_ACQURING;
					}
					// If we have 'sa' in the Rx buffer, it's the command to stop acquisition
					else if(!strncmp("sa", BTPortDescriptor.RxBuf, 2) && acqState == FSM_ACQURING) {
						acqState = FSM_WAITINGFORPACKET;
						haveFinalPacket = 0;
						BTStatus &= ~4;
					}
					else if(!strncmp("ea", BTPortDescriptor.RxBuf, 2) && (acqState == FSM_FULL || acqState == FSM_FULL_ON)) {
						unsigned char EraseInProgress = 0;
						
						MainPowerOn();			// just in case we lose USB
						EraseInProgress=1;	// keep auto power off from tripping if we lose USB
						
						if(TaskFlashCmd == TF_IDLE){
							TaskFlashCmd = TF_ERASE_ALL;
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
							}while(TaskFlashCmd != TF_IDLE);	// wait for erase to complete
							ElapsedMinutes = 0;
							Seg7HP(' ');
							
							EraseInProgress=0;	// all auto power off on loss of USB
							acqState = FSM_DONE;
						}						
					}
					else if((!strncmp("re", BTPortDescriptor.RxBuf, 2) || !strncmp("rv", BTPortDescriptor.RxBuf, 2)) && acqState == FSM_FULL_ON) {
						argC=sscanf_P( (const char *)BTPortDescriptor.RxBuf,PSTR("%2s%f%f"), &cmd, &data1, &data2);
						transferingFullData = 1;
						transferingDataType = (cmd[1] == 'e' ? FULL_TRANSFER_ECG : FULL_TRANSFER_VOICE);
						BTReady = 0;
						FlashPageNumber = 0;
						waiting = 0;
					}
					else if(!strncmp("v+", BTPortDescriptor.RxBuf, 2)) {
						EE_Wr(EE_ENABLE_VOICE, 1);
					}
					else if(!strncmp("v-", BTPortDescriptor.RxBuf, 2)) {
						EE_Wr(EE_ENABLE_VOICE, 0);
					}																		
					
					/*
					// Simulates a pacer spike for testing
					else if(!strncmp("spike", BTPortDescriptor.RxBuf, 5)) {
						nextPacketIsSpike = 1;
					}
					*/
					
					
					BTPortDescriptor.RxReady = 0;
				}
				
				if(!(PORTF.IN & PIN7_bm) && (acqState == FSM_FULL || acqState == FSM_FULL_ON)) {
					BTEnable = 0;
					acqState = FSM_DONE;
				}
				
				if(transferingFullData) {
					if(!BTReady && FlashPageNumber == 0) {
						Seg7LP('B');
						
						argC--;
						
						if(argC==1){	// only a start time was provided, dump one BLOCK_LEN of ECG
							if(data1>(transferingDataType == FULL_TRANSFER_VOICE ? VOICE_RECORD_LENGTH : ECG_RECORD_LENGTH)-BLOCK_LEN*2){
								data1 = (transferingDataType == FULL_TRANSFER_VOICE ? VOICE_RECORD_LENGTH : ECG_RECORD_LENGTH)-BLOCK_LEN*2;
							}
							data2 = data1;
						}
						else if(argC==2){ // we were given both a start time and an end time
							if(data2<data1){	// fix end<start
								data2 = data1;
							}
							else if(data2>(transferingDataType == FULL_TRANSFER_VOICE ? VOICE_RECORD_LENGTH : ECG_RECORD_LENGTH)-BLOCK_LEN){// fix end bigger than record length
								data2 = (transferingDataType == FULL_TRANSFER_VOICE ? VOICE_RECORD_LENGTH : ECG_RECORD_LENGTH)-BLOCK_LEN;
							}
							else{
								data2 = data2;
							}
						}

						//if no arguments, dump everything
						if(argC == 0) {
							data1 = 0;
							data2 = (transferingDataType == FULL_TRANSFER_VOICE ?
							floor((float)EE_Rd(EE_VOICE_RECORD_LENGTH)*0.016*10.0)/10.0 :
							floor((float)EE_Rd(EE_ECG_RECORD_LENGTH)*0.021*10)/10.0);
							data2 -= 0.1f; // We request 100ms less on the PC (not sure why?)
						}

						// truncate times to 100ms resolution
						start = data1*10;
						end = data2*10;
						// convert to milliseconds
						start*=100;
						end*=100;
						// set end to the last milliscond of the 100ms block
						end+=99;

						// now we've got valid end times, convert that into flash page numbers and indexes
						if(transferingDataType == FULL_TRANSFER_VOICE) {
							startPage = start/VOICE_MS_PER_PAGE + FLASH_VOICE_START_PAGE;
							startIndex = start%VOICE_MS_PER_PAGE*16;
							endPage = end/VOICE_MS_PER_PAGE + FLASH_VOICE_START_PAGE;
							endIndex =  end%VOICE_MS_PER_PAGE*16;
						}
						else {
							startPage = start/ECG_MS_PER_PAGE + FLASH_ECG_START_PAGE;
							startIndex = start%ECG_MS_PER_PAGE;
							endPage = end/ECG_MS_PER_PAGE + FLASH_ECG_START_PAGE;
							endIndex =  end%ECG_MS_PER_PAGE;
						}						

						crc = 0xffff;
						FlashPageNumber=startPage;
						beforeSend = BTPortDescriptor.TxTotal;
		
						BTReady = 1;
					}	
					else if(BTReady) {
						BTReady = 0;
						// Dump a page
						beforeSend = BTPortDescriptor.TxTotal;
						
						// dump ECG data from full pages between startPage and endPage
						if(FlashPageNumber == startPage) {
							while(TaskFlashCmd != TF_IDLE); // should never happen
							TaskFlashCmd = TF_READ_BUF1;
							while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
								osYIELD;
							}
							for(i=startIndex;i<(transferingDataType == FULL_TRANSFER_VOICE ? VOICE_SAMPLES_PER_PAGE : ECG_SAMPLES_PER_PAGE);i++){
								if(transferingDataType == FULL_TRANSFER_ECG) {
									for(j=0;j<12;j++){
										put_byte(Buf1.Data.ECG.Sample[i][j]);
										crc(Buf1.Data.ECG.Sample[i][j]);
									}
								}
								else {
									put_byte(Buf1.Data.Byte[i]);
									crc(Buf1.Data.Byte[i]);
								}						
							}
							
							if(transferingDataType == FULL_TRANSFER_ECG) {
								put_byte(Buf1.Data.ECG.Flag1);
								waiting++;
								crc(Buf1.Data.ECG.Flag1);
							}
						}
						else if(FlashPageNumber<endPage){	
							while(TaskFlashCmd != TF_IDLE); // should never happen
							TaskFlashCmd = TF_READ_BUF1;
							while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
								osYIELD;
							}		
							for(i=0;i<(transferingDataType == FULL_TRANSFER_VOICE ? VOICE_SAMPLES_PER_PAGE : ECG_SAMPLES_PER_PAGE);i++){
								if(transferingDataType == FULL_TRANSFER_ECG) {
									for(j=0;j<12;j++){
										put_byte(Buf1.Data.ECG.Sample[i][j]);
										waiting++;
										crc(Buf1.Data.ECG.Sample[i][j]);
									}
								}
								else {
									put_byte(Buf1.Data.Byte[i]);
									waiting++;
									crc(Buf1.Data.Byte[i]);
								}							
							}
							if(transferingDataType == FULL_TRANSFER_ECG) {
								put_byte(Buf1.Data.ECG.Flag1);
								waiting++;
								crc(Buf1.Data.ECG.Flag1);
							}
						}
						else {
							FlashPageNumber=endPage;
							while(TaskFlashCmd != TF_IDLE); // should never happen
							TaskFlashCmd = TF_READ_BUF1;
							while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
								osYIELD;
							}
							for(i=0;i<=(transferingDataType == FULL_TRANSFER_VOICE ? endIndex+VOICE_SAMPLES_PER_MS-1 : endIndex);i++){
								if(transferingDataType == FULL_TRANSFER_ECG) {
									for(j=0;j<12;j++){
										put_byte(Buf1.Data.ECG.Sample[i][j]);
										waiting++;
										crc(Buf1.Data.ECG.Sample[i][j])
									}
								}
								else {
									put_byte(Buf1.Data.Byte[i]);
									waiting++;
									crc(Buf1.Data.Byte[i]);
								}						
							}
							
							if(transferingDataType == FULL_TRANSFER_ECG) {
								put_byte(Buf1.Data.ECG.Flag1);
								waiting++;
								crc(Buf1.Data.ECG.Flag1);
							}
							
							put_byte(crc);
							put_byte(crc >> 8);
							waiting+=2;
						}
						
						// Place the pace flag at the end of the page
						if(transferingDataType == FULL_TRANSFER_ECG) {
							put_byte(Buf1.Data.ECG.Flag1);
							waiting++;
							crc(Buf1.Data.ECG.Flag1);
						}						
							
					}												
	
					if(!BTReady && BTPortDescriptor.TxTotal - beforeSend >= waiting) {
						// Spec sheet says max transmission delay is 20ms. Subject to change.
						osWAIT(10);
		
						if(FlashPageNumber==endPage) {
							// Make _absolutely sure_ the BT chip is done transmitting
							osWAIT(200);
							Seg7LP('F');
							transferingFullData = 0;
							transferingDataType = FULL_TRANSFER_NONE;
							BTReady = 0;
						}
						else {
							BTReady = 1;	
							FlashPageNumber++;
							waiting = 0;
						}					
					}		
				}
				if(BTReady && !transferingFullData) {// we have a lead fail model to decimate
					BTReady=0;

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
						BTString[BT_PACESPIKE] = ((BTStatus & 8) ? btSpikeIndex : 0);		// Pacer spike index bits
						
						//if(BTStatus & 8)
						//	print_bt("We have a pacer spike on sample %d of 8\n", btSpikeIndex);
							
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
						
						fwrite(BTString, BT_SIZE, 1, bt);
						
						if(acqState == FSM_WAITINGFORPACKET)
							haveFinalPacket = 1;
							
						BTStatus &= ~8; // Clear pacer bit

						FrameIndex++;
						}

					}
				}				
#endif
#ifdef COMPARE_VERSION
				if(BTEnable) {
					if(!BTReady && FlashPageNumber == 0) {
						Seg7LP('B');
		
						// Initialize the test variables
						secondsOfEcg = floor((float)EE_Rd(EE_ECG_RECORD_LENGTH)*0.021*10)/10.0;

						// truncate times to 100ms resolution
						end = secondsOfEcg*10;
						// convert to milliseconds
						end*=100;
						// set end to the last milliscond of the 100ms block
						end+=99;

						// now we've got valid end times, convert that into flash page numbers and indexes
						endPage = end/ECG_MS_PER_PAGE + FLASH_ECG_START_PAGE;
						endIndex =  end%ECG_MS_PER_PAGE;

						crc = 0xffff;
						FlashPageNumber=FLASH_ECG_START_PAGE;
		
						BTReady = 1;
					}	
					else if(BTReady) {
						BTReady = 0;
						// Dump a page
						beforeSend = BTPortDescriptor.TxTotal;
						
						// dump ECG data from full pages between startPage and endPage
						if(FlashPageNumber<endPage){
			
							while(TaskFlashCmd != TF_IDLE); // should never happen
							TaskFlashCmd = TF_READ_BUF1;
							while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
								osYIELD;
							}
							crc = 0;			
							for(i=0;i<ECG_SAMPLES_PER_PAGE;i++){
								for(j=0;j<12;j++){
									put_byte(Buf1.Data.ECG.Sample[i][j]);
									waiting++;
									crc = CRCCalc(crc, Buf1.Data.ECG.Sample[i][j]);
								}
							}
							put_byte(crc >> 8);
							put_byte(crc);
							waiting+=2;
						}
						else {
							FlashPageNumber=endPage;
							while(TaskFlashCmd != TF_IDLE); // should never happen
							TaskFlashCmd = TF_READ_BUF1;
							while(TaskFlashCmd != TF_IDLE){	// wait for read to complete
								osYIELD;
							}
							for(i=0;i<=endIndex;i++){
								for(j=0;j<12;j++){
									put_byte(Buf1.Data.ECG.Sample[i][j]);
									waiting++;
								}
							}
						}			
							
					}												
	
					if(!BTReady && BTPortDescriptor.TxTotal - beforeSend >= waiting) {
						// Spec sheet says max transmission delay is 20ms. Subject to change.
						osWAIT(25);
		
						if(FlashPageNumber==endPage) {
							// Make _absolutely sure_ the BT chip is done transmitting
							osWAIT(200);
			
							BTEnable = 0;
							BTReady = 0;
						}
						else {
							BTReady = 1;	
							FlashPageNumber++;
							waiting = 0;
						}					
					}		
				}
#endif
#ifdef LOGGING_VERSION
				if(BTEnable) {
					osWAIT(25);
					
					if(BTPortDescriptor.RxReady) {
						
						fprintf(bt, "Message: %s\n", BTPortDescriptor.RxBuf);				
						memset(BTPortDescriptor.RxBuf, 0, 255);
						BTPortDescriptor.RxReady=0;
					}
					
				}					
#endif
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
