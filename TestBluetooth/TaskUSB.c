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
#include "taskflash.h"
#include "taskacq.h"
#include "eeprom.h"
#include "ulaw.h"
#include "crc.h"
#include "seg7.h"
#include "TaskWatchDog.h"
#include "taskBT.h"
#include "taskUSB.h"

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
