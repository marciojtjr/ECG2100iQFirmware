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
#include "taskAcq.h"

#define BT_USART			USARTE0
#define BT_USART_PORT PORTE

unsigned char BTBufReady;
unsigned int BTBuf[8];
unsigned char BTIn[252];


//##########################################################################################
// define and allocate globals for the bluetooth task

USART_desc_t BTPortDescriptor;	// a descriptor for the USART connected to the Bluetooth transceiver
static int put_bt( char c, FILE *stream){
	uart_putchar(&BTPortDescriptor,c);
	return(0);
}
static char get_bt(FILE *stream) {
	return ' ';//uart_getchar(&BTPortDescriptor);
}

#define put_byte(c) uart_putchar(&BTPortDescriptor,c)

static FILE bt_str = FDEV_SETUP_STREAM(put_bt, 0, _FDEV_SETUP_WRITE);
static FILE bt_in_str = FDEV_SETUP_STREAM(get_bt, 0, _FDEV_SETUP_READ);
FILE *bt;
FILE *bt_in;

void BTOn(void){
	LogMessage("Bluetooth on--moment of truth. Any messages after _should_ be raw dumps from Rx");
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
	bt_in = &bt_in_str;

	BT_USART_PORT.DIRSET = PIN3_bm;// TXD is output
	BT_USART_PORT.DIRCLR = PIN2_bm;// RXD is input

	USART_InterruptDriver_Initialize(&BTPortDescriptor, &BT_USART, USART_DREINTLVL_LO_gc); // setup USART, DRE interrupt priority is LOW
	USART_RxdInterruptLevel_Set(BTPortDescriptor.usart, USART_RXCINTLVL_LO_gc); // receive interrupt priority is LOW
	USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N2
	USART_Baudrate_Set(BTPortDescriptor.usart, 19, -4); //921600 bps
//	USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/BT_BAUD_RATE-1), 0);  // Does not work for all Baud Rates (BAUD_RATE = 921600 for example)
	USART_Rx_Enable(BTPortDescriptor.usart);
	USART_Tx_Enable(BTPortDescriptor.usart);
  
    unsigned char ix = 0;
    static unsigned int FrameIndex = 0;
    short crc;

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
			if(BTReady && BTEnable)
                {	// we have a lead fail model to decimate
				BTReady=0;

				if(BTPortDescriptor.RxReady) {
					LogMessage(BTPortDescriptor.RxBuf);
				}

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
					fwrite(BTString, BT_SIZE, 1, bt);

					//LogMessage(BTIn);

					FrameIndex++;
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

#endif //BLUETOOTH
