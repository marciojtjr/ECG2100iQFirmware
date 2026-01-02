/*
 * twi_driver.c
 *
 * Created: 10/31/2025 1:06:24 PM
 *  Author: adunk
 */ 

#include "twi_driver.h"

#define TWI_SPEED             50000       //!< TWI data transfer rate
uint8_t uc_addr;

int twi_init(uint8_t ucAddress)
{
	int ret=0;
	uc_addr = ucAddress;
	twi_master_options_t opt;
	opt.speed = TWI_SPEED;
	return ret;
}

int twi_write(void)
{
	int ret=0;
	return ret;
}

int twi_read(void)
{
	int ret=0;
	return ret;
	
}
