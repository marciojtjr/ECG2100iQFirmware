/*
 * TaskCC.c
 *
 * Created: 10/31/2025 12:26:16 PM
 *  Author: adunk
 */ 
#ifndef BOOTLOADER
#include <avr/io.h>
#include "avr_compiler.h"
#include "rtos.h"
#include "iq.h"
#include "taskCC.h"
#include "taskUSB.h"
#include "twi_driver.h"
#include "ltc2959.h"
#include "eeprom.h"

/*
*   A separate task for querying the coulomb counter over TWI.
*/

static float fVoltage = 4.2;		//Start off full before 1st reading
static uint8_t ucBattPerc = 100;	//Start off full before 1st reading
static bool charge_complete = false;
static bool bFuelGaugeValid = true;

static void calc_batt_perc(int iBattCharge)
{
	ucBattPerc = (100*((float)iBattCharge/(float)FULL_BATT_CAP_MA));
	//print_usb("Batt chg:%d\r\n", ucBattPerc);
}

BEGIN_TASK(TaskCC)

    //Declare variables necessary for TWI

    //Declare variable for getting coulomb counter information

    //Setup twi and coulomb counter
	twi_init(LTC2959_get_i2c_addr());
	LTC2959_init();
	
	//Read the eeprom FG validity value
	//int fg = EE_Rd(EE_FG_VALID);
	//print_usb("FG valid:%d\r\n", fg); 

    //Start while loop for periodically getting data
	while(1){
		//print_usb("Batt V:%f\n", LTC2959_get_voltage());
		//No reason to get the charge if the charge is complete
		if(!charge_complete)
			calc_batt_perc(LTC2959_get_charge());
		fVoltage = LTC2959_get_voltage();
		//Read the eeprom FG validity value
		//int fg = EE_Rd(EE_FG_VALID);
		//print_usb("FG valid:%d\r\n", fg); 
		osWAIT(250);
		osWAIT(250);
		osWAIT(250);
		osWAIT(250);
	}

END_TASK

uint8_t cc_get_batt_per()
{
	return ucBattPerc;
}

float cc_get_batt_volt()
{
	return fVoltage;
}

void cc_set_charge_complete()
{
	if(!charge_complete)
	{
		charge_complete = true;
		LTC2959_set_full();
		print_usb("Set charge complete\r\n"); 
		cc_set_fg_valid();
	}
}

void cc_clear_charge_complete()
{
	charge_complete = false;
}

bool cc_get_charge_complete()
{
	return charge_complete;
}

void cc_set_fg_valid()
{
	bFuelGaugeValid = true;
	EE_Wr(EE_FG_VALID, 1);
}

void cc_clear_fg_valid()
{
	bFuelGaugeValid = false;
	EE_Wr(EE_FG_VALID, 0);
}

bool cc_get_fg_valid()
{
	return bFuelGaugeValid;
}
#endif