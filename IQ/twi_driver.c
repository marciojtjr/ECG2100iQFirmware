/*
 * twi_driver.c
 *
 * Created: 10/31/2025 1:06:24 PM
 *  Author: adunk
 */ 

#include "twi_driver.h"
#include "twi_master.h"

#define TWI_MUX				(&TWIC)
#define TWI_SPEED             50000       //!< TWI data transfer rate

twi_master_options_t opt;

int twi_init(uint8_t ucAddress)
{
	int ret=0;
	
	opt.speed = TWI_SPEED;
	opt.chip = ucAddress;
	// Manually calculate baud rate for 32MHz CPU (bypass incorrect sysclk_get_cpu_hz())
	opt.speed_reg = TWI_BAUD(32000000UL/2, TWI_SPEED);	//dividing by 2 to get 100k???
	// Initialize the TWI master driver.
	return twi_master_setup(TWI_MUX, &opt);
}

int twi_write(uint8_t reg, uint8_t *ucpData, uint8_t ucDataSize)
{
	twi_package_t packet = {
		.chip = opt.chip,
		.addr[0]      = reg,
		.addr_length  = sizeof (uint8_t),    // TWI slave memory address data size
		.buffer = ucpData,
		.length = ucDataSize,
	};
	int ret=0;
	
	// Perform a multi-byte write access then check the result.
	if (twi_master_write(TWI_MUX, &packet) != TWI_SUCCESS)
		return 1;
	
	return ret;
}
/*
typedef struct
{
	//! TWI chip address to communicate with.
	char chip;
	//! TWI address/commands to issue to the other chip (node).
	uint8_t addr[3];
	//! Length of the TWI data address segment (1-3 bytes).
	int addr_length;
	//! Where to find the data to be written.
	void *buffer;
	//! How many bytes do we want to write.
	unsigned int length;
	//! Whether to wait if bus is busy (false) or return immediately (true)
	bool no_wait;
} twi_package_t;*/
int twi_read(uint8_t reg, uint8_t *ucpData, uint8_t ucDataSize)
{
	twi_package_t packet = {
		.chip = opt.chip,
		.addr[0]      = reg,
		.addr_length  = sizeof (reg),    // TWI slave memory address data size
		.buffer = ucpData,
		.length = ucDataSize,
	};

	
	// Perform a multi-byte read access then check the result.
	if (twi_master_read(TWI_MUX, &packet) != TWI_SUCCESS)
		return 1;
	
	return 0;
	
}
