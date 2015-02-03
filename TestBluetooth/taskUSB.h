#ifndef BOOTLOADER

#ifndef TASKUSB_H
#define TASKUSB_H

#include <stdio.h>

#include "usart_driver.h"

// macro to make sending stuff a little easier
#define print_usb(fmt,...)  fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)
#define print_debug(fmt,...) if(Verbose) fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)

#define BLOCK_LEN 0.1	// set the size of a dumped block, in seconds
#define BLOCK_LEN_MS (BLOCK_LEN*1000)	// set the size of a dumped block, in milliseconds

#endif
#endif