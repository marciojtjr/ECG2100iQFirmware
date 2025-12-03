#ifndef BOOTLOADER

#ifndef TASKUSB_H
#define TASKUSB_H

#include <stdio.h>

#include "usart_driver.h"

extern FILE *usb;

extern unsigned char Verbose;
extern unsigned int Delay;
extern unsigned char FlowControl;
extern long int result;
extern unsigned char usbConfigured;

// macro to make sending stuff a little easier
#define print_usb(fmt,...)  fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)
#define print_debug(fmt,...) if(Verbose) fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)
#define debug_message(fmt,...)  fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)

extern USART_desc_t USBPortDescriptor;	// a descriptor for the USART connected to the USB bridge chip
extern FILE usb_str;

#define BLOCK_LEN 0.1	// set the size of a dumped block, in seconds
#define BLOCK_LEN_MS (BLOCK_LEN*1000)	// set the size of a dumped block, in milliseconds

enum {PRINT_USB, PRINT_BT, PRINT_BT_APPLE};
	
void Hello(int type);

#endif
#endif