#include "iq.h"
#include "eeprom.h"

union data {
	unsigned long l;
	unsigned int i[2];
	unsigned char b[4];
};

//__________________________________________________________________________________________
// EEPROM Routines to read/write words rather than bytes
// The read routine reads until it gets two matches (fixes a bug in the hardware?)

#define EE_ELEMENT_SIZE 4 //# of bytes in each thing we store in EEPROM
//_____________________________________________
// Read an element from EEPROM
unsigned long EE_Rd(unsigned int offset){

	unsigned char addr;
	unsigned char page;

	union data data1,data2;

	addr = (offset*EE_ELEMENT_SIZE)%EEPROM_PAGE_SIZE;
	page = offset*EE_ELEMENT_SIZE/EEPROM_PAGE_SIZE;

	do{
		data1.b[0] = EEPROM_ReadByte(page,addr+0);
		data1.b[1] = EEPROM_ReadByte(page,addr+1);
		data1.b[2] = EEPROM_ReadByte(page,addr+2);
		data1.b[3] = EEPROM_ReadByte(page,addr+3);

		data2.b[0] = EEPROM_ReadByte(page,addr+0);
		data2.b[1] = EEPROM_ReadByte(page,addr+1);
		data2.b[2] = EEPROM_ReadByte(page,addr+2);
		data2.b[3] = EEPROM_ReadByte(page,addr+3);
	}while(data1.l!=data2.l);
	return(data1.l);
}

//_____________________________________________
// Write an element to EEPROM
void EE_Wr(unsigned int offset, unsigned long data){

	if(offset == EE_APP_CRC){
#ifndef BOOTLOADER
		Error(1);
#endif
	}

	unsigned char addr;
	unsigned char page;

	addr = (offset*EE_ELEMENT_SIZE)%EEPROM_PAGE_SIZE;
	page = offset*EE_ELEMENT_SIZE/EEPROM_PAGE_SIZE;

	EEPROM_WriteByte(page, addr+0, data>> 0);
	EEPROM_WriteByte(page, addr+1, data>> 8);
	EEPROM_WriteByte(page, addr+2, data>>16);
	EEPROM_WriteByte(page, addr+3, data>>24);
}
