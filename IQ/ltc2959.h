/*
 * ltc2959.h
 *
 * Created: 10/31/2025 12:53:27 PM
 *  Author: adunk
 */ 


#ifndef LTC2959_H_
#define LTC2959_H_

#include <stdint.h>
#include <stdbool.h>

#define FULL_BATT_CAP_MA	5000u

// LTC2959 Hardware Configuration
#define LTC2959_RESISTOR_MOHM (8)      // in milliohms
#define LTC2959_ACR_LSB_NAh (533)      // nanoamphour
#define LTC2959_REF_RESISTOR_MOHM (50) // reference resistor for LTC2959

// LTC2959 Register Addresses
#define LTC2959_STATUS_REG (0x00)
#define LTC2959_CONTROL_REG (0x01)
#define LTC2959_ACR_REG (0x03)         // Accumulated charge register
#define LTC2959_VOLTAGE_REG (0x0F)
#define LTC2959_CURRENT_REG (0x19)

// LTC2959 Operating Modes
typedef enum {
	LTC2959_SLEEP=0,
	LTC2959_SMART_SLEEP=1,  // updates voltage, current, temp every 52 seconds
	LTC2959_CONTINOUS_V=2,
	LTC2959_CONTINOUS_I=3,
	LTC2959_CONTINOUS=4,
	LTC2959_SINGLE_SHOT=5,
	LTC2959_CONTINOUS_B4=6,
} ltc2959_mode;

// Function Declarations
int LTC2959_init(void);
int LTC2959_sleep(void);
int32_t LTC2959_get_charge(void);
int LTC2959_set_full(void);
float LTC2959_get_ma(void);
float LTC2959_get_voltage(void);
bool LTC2959_get_uvlo(void);
uint8_t LTC2959_get_i2c_addr();

#endif /* LTC2959_H_ */
