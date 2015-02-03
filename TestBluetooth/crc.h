#ifndef BOOTLOADER

extern const unsigned int CRCTable[256] PROGMEM;

#define crc(x) { unsigned char tmp = ((x ^ (crc >> 8)) & 0xff); crc = ((crc << 8) ^ pgm_read_word(&CRCTable[tmp])) & 0xffff; }
//#define crc(x)	crc = crc ^ ((x & 0xff) << 8);	for (char i=0; i<8; i++){	if (crc & 0x8000) crc = (crc << 1) ^ 0x1021;	else crc <<= 1;	}
//#define crc(x)

#endif