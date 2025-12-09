/*
 * ltc2959.c
 *
 * Created: 10/31/2025 12:53:14 PM
 *  Author: adunk
 */

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "ltc2959.h"
#include "twi_driver.h"
#include "taskUSB.h"
#include "taskCC.h"

// TODO: Define FULL_BATT_CAP_MA - investigate battery capacity
#define I2C_ADDR			0x63u

static int set_control(ltc2959_mode mode, bool use_sensen){
	uint8_t reg = LTC2959_CONTROL_REG;
	uint8_t dataVal;
	uint8_t data[1]={0};
	dataVal=((uint8_t)mode)<<5;
	if (use_sensen){
		dataVal=dataVal | 1<<2;
	}

	data[0]=dataVal;

	print_usb("Setting control to 0x%02X", reg);
	return twi_write(reg, data, sizeof(data));  // TODO: Implement with proper data and size parameters
}

static int set_charge(uint32_t mah)
{
	uint8_t reg = LTC2959_ACR_REG;
	uint8_t data[4]={0};
	uint32_t acr;

	float fTemp = (float)LTC2959_REF_RESISTOR_MOHM/(float)LTC2959_RESISTOR_MOHM;
	fTemp = (float)LTC2959_ACR_LSB_NAh * fTemp;
	print_usb("fTemp1 %f", fTemp);
	fTemp = (float)mah/fTemp;
	print_usb("fTemp2 %f", fTemp);

	acr=fTemp*1000000; //scale for device level LSB
	//convert from mah at end so it doesn't roll over

	print_usb("mah %d", mah);
	print_usb("raw ACR %d", acr);

	data[0]=(uint8_t)(acr>>24 & 0x0FF);
	data[1]=(uint8_t)(acr>>16 & 0x0FF);
	data[2]=(uint8_t)(acr>>8 & 0x0FF);
	data[3]=(uint8_t)(acr & 0x0FF);
	return twi_write(reg, data, sizeof(data));  // TODO: Implement with proper data and size parameters
}

int LTC2959_init(){
	int ret;
	ret = set_control(LTC2959_CONTINOUS, false);
	if(0 == ret)
	{
		//check if the fuel gauge lost power
		if(true == LTC2959_get_uvlo())
		{
			print_usb("Invalid FG\r\n");
			//fuel gauge has lost power, ensure battery gauge valid is false
			// TODO: Implement memory_clear_fuel_gauge_valid() equivalent
			set_charge(FULL_BATT_CAP_MA/2); //set to middle because we don't know
			cc_clear_charge_complete();
			cc_clear_fg_valid();
		}
	}

	if(1 == ret)
	{
		print_usb("Err ltc2959 init");
	}

	return ret;
}

int LTC2959_sleep(){
	return set_control(LTC2959_SLEEP, true);
}


//returns charge remaining in amp-hours
int32_t LTC2959_get_charge(){
	uint8_t read_data[4]={0};
	int32_t acr;
	uint8_t reg=LTC2959_ACR_REG;
	int ret;

	ret=twi_read(reg, read_data, sizeof(read_data));  // TODO: Implement with proper reg, read_data, and size parameters
	if(ret) print_usb("read word failed");
	acr= (int32_t)read_data[0]<<24 | (int32_t)read_data[1]<<16 | (int32_t)read_data[2]<<8 | (int32_t)read_data[3];
	//print_usb("raw ACR %d\r\n", acr);      //reads 1500938
	float fTemp = ((float)acr/1000000);
	fTemp = fTemp * (float)LTC2959_ACR_LSB_NAh * (float)LTC2959_REF_RESISTOR_MOHM / (float)LTC2959_RESISTOR_MOHM;
	print_usb("mah %f\r\n", fTemp); 
	return (int32_t)fTemp;
}

int LTC2959_set_full()
{
	return set_charge(FULL_BATT_CAP_MA);
}

float LTC2959_get_ma(){
	uint8_t read_data[2]={0};
	uint32_t res;
	float ma=0.0f;
	uint8_t reg=LTC2959_CURRENT_REG;
	int ret;
	ret=twi_read(reg, read_data, sizeof(read_data));  // TODO: Implement with proper reg, read_data, and size parameters
	if(ret) print_usb("read word failed");

	res= (uint32_t)read_data[0]<<8 | (uint32_t)read_data[1];
	
	print_usb("mA raw %d\r\n", res);
	if (res & (uint32_t)1<<15){
		res= res & ~((uint32_t)1<<15);
		ma=97.5f/(float)LTC2959_RESISTOR_MOHM * (-32768.0f+(float)res)/32768.0f;
		} else{
		ma=97.5f/(float)LTC2959_RESISTOR_MOHM * (float)res/32768.0f;
	}
	return ma*1000.0f;   //turn to ma
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
float LTC2959_get_voltage(){
	uint8_t read_data[2]={0};
	uint32_t res;
	float voltage;
	uint8_t reg=LTC2959_VOLTAGE_REG;
	int ret;
	ret=twi_read(reg, read_data, sizeof(read_data));  // TODO: Implement with proper reg, read_data, and size parameters
	if(ret) print_usb("read word failed");
	
	//combine both bytes into uint16
	res= (uint32_t)read_data[0]<<8 | (uint32_t)read_data[1];
	//print_usb("Raw res:%d\r\n", res);
	voltage = (62.6 * (float)res / 65536);
	print_usb("v %f\r\n", voltage); 
	return voltage;
}

bool LTC2959_get_uvlo() {
	uint8_t reg=LTC2959_STATUS_REG;
	uint8_t data[1];
	int ret;
	ret=twi_read(reg, data, sizeof(data));  // TODO: Implement with proper reg, data, and size parameters
	if (ret){
		ret=twi_read(reg, data, sizeof(data));  // TODO: Implement with proper reg, data, and size parameters
		if (ret){
			print_usb("could not read UVLO");
			return 1; //assume it is set;
		}
	}

	return (bool)(data[0] & 0x01); //bit 0 is the UVLO
}

uint8_t LTC2959_get_i2c_addr() {
	return I2C_ADDR;
}
