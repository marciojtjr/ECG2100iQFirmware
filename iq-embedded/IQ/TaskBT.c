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

#define BT_USART			USARTE0
#define BT_USART_PORT PORTE

unsigned char BTBufReady;
unsigned int BTBuf[8];

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

//==========================================================================================
// Bluetooth Task

BEGIN_TASK(TaskBT)

	//------------------------------------------------------------------------------------------
	// prepare the Bluetooth USART
	#define BT_BAUD_RATE 115200
	bt = &bt_str;
	BT_USART_PORT.DIRSET = PIN3_bm;// TXD is output
	BT_USART_PORT.DIRCLR = PIN2_bm;// RXD is input

	USART_InterruptDriver_Initialize(&BTPortDescriptor, &BT_USART, USART_DREINTLVL_LO_gc); // setup USART, DRE interrupt priority is LOW
	USART_RxdInterruptLevel_Set(BTPortDescriptor.usart, USART_RXCINTLVL_LO_gc); // receive interrupt priority is LOW
	USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N2
//	USART_Baudrate_Set(BTPortDescriptor.usart, 19, -4); //921600 bps
	USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/BT_BAUD_RATE-1), 0);
	USART_Rx_Enable(BTPortDescriptor.usart);
	USART_Tx_Enable(BTPortDescriptor.usart);

	//==========================================================================================
	// Bluetooth command parser

	if( (MCU.JTAGUID != 0xff) && USBPresent() ){	// we can run Bluetooth only on non rev 1 hardware (which has unprogrammed JTAG UID)
		BTOn();

		while(1){

			if(BTReady){	// we have a lead fail model to decimate
				BTReady=0;

				// Bluetooth output will be at 125Hz, so we'll need to average eigth 1KHz samples, which we can do by adding up all the lead fail subsamples for two lead fail cycles

				unsigned char m = M^1;	// use the LFBiasModel NOT being filled at the moment

				// first phase of Bluetooth sample production, we add up all four 16 bit samples from the lead-fail bias model, after scaling them back to 12 bits.
				if(BTBufPhase==0){
					for(unsigned char i=0;i<8;i++){	// do all channels
						BTBuf[i] = (
							LFBiasModel[m][i][0]/16 +
							LFBiasModel[m][i][1]/16 +
							LFBiasModel[m][i][2]/16 +
							LFBiasModel[m][i][3]/16);
					}
					BTBufPhase=1;
				}
				
				// second phase of Bluetooth sample production, we add in the next four LFBiasModel subsamples, after scaling them back to 12 then average the eight subsamples (/8)
				else{
					for(unsigned char i=0;i<8;i++){	// do all channels
						BTBuf[i] = (BTBuf[i]+(
							LFBiasModel[m][i][0]/16 +
							LFBiasModel[m][i][1]/16 +
							LFBiasModel[m][i][2]/16 +
							LFBiasModel[m][i][3]/16))
							/8;
					}
					BTBufPhase=0;

					// now we have eight channels of 125Hz data, ready to go, send three hex bytes for each 12 bit channel, while also accumulating all channels for a checksum
					unsigned char j=0;
					int cs = 0;
					BTString[j++]='*';
					for(unsigned char i=0;i<8;i++){	// do all channels
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
				}

			}

			osYIELD;	// let other stuff run

		}
	}
	else{ // consume as little time as possible, we're not supposed to be running
		while(1){
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
