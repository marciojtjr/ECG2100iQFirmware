#ifndef BOOTLOADER

#ifndef ulaw_h
#define ulaw_h

#include <avr/io.h>
#include <avr/pgmspace.h>

int ulaw2linear(unsigned char ulaw);
int alaw2linear(unsigned char alaw);
unsigned char linear2ulaw(int linear);
unsigned char linear2alaw(int linear);

#endif
#endif