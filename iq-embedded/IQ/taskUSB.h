#ifndef TASKUSB_H
#define TASKUSB_H

#include <stdio.h>

extern FILE *usb;

extern unsigned char Verbose;
extern unsigned int Delay;
extern unsigned char FlowControl;

// macro to make sending stuff a little easier
#define print_usb(fmt,...)  fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)
#define print_debug(fmt,...) if(Verbose) fprintf_P(usb, PSTR(fmt), ## __VA_ARGS__)

void dump_flags();

#endif
