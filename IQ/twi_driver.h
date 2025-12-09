/*
 * twi_driver.h
 *
 * Created: 10/31/2025 1:06:38 PM
 *  Author: adunk
 */ 


#ifndef TWI_DRIVER_H_
#define TWI_DRIVER_H_

#include <stdint.h>
#include <stdio.h>
#include "twi_master.h"

#define PORT_TWI PORTC
#define TWI_SCL	PIN1_bm
#define TWI_SDA	PIN0_bm

// Function declarations
int twi_init(uint8_t ucAddress);
int twi_write(uint8_t reg, uint8_t *ucpData, uint8_t ucDataSize);
int twi_read(uint8_t reg, uint8_t *ucpData, uint8_t ucDataSize);

#endif /* TWI_DRIVER_H_ */
