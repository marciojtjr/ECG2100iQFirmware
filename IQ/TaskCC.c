#ifndef BOOTLOADER
/*
 * TaskCC.c
 *
 * Created: 10/31/2025 12:26:16 PM
 *  Author: adunk
 */ 

#include <avr/io.h>
#include "avr_compiler.h"
#include "rtos.h"
#include "iq.h"
#include "taskCC.h"
#include "taskUSB.h"
#include "twi_driver.h"
#include "ltc2959.h"
#include "eeprom.h"
#include "adc_driver.h"
#include "taskAcq.h"
#include "seg7.h"

#define LEGACY_FULL_BATT_CAP_MA	4.2
// Legacy battery ADC calibration (raw values from ADC with AREFB reference)
#define LEGACY_RAW_FULL		0.27f	// Raw ADC value at 4.2V (full charge)
#define LEGACY_RAW_EMPTY	0.206f	// Raw ADC value at 3.2V (empty) = 0.27 * 3.2/4.2
/*
*   A separate task for querying the coulomb counter over TWI.
*/

static float fVoltage = 4.2;		//Start off full before 1st reading
static uint8_t battery_percent = 100;	//Start off full before 1st reading
static bool charge_complete = false;
bool legacy_charger = false;
static bool usb_was_present = false;	//Track USB state for edge detection

BEGIN_TASK(TaskCC)
    //Declare variable for getting coulomb counter information
	cc_legacy_get_batt_volt();
	legacy_charger = fVoltage > 0;
	if (legacy_charger) {
		Seg7LP('L');
	} else {
		Seg7LP('A');
	}
	print_usb("TaskCC LEGACY fVoltage %f\r\n", fVoltage);
	if (!legacy_charger) {
		//Setup twi and coulomb counter
		twi_init(LTC2959_get_i2c_addr());
		LTC2959_init();
	}

    //Start while loop for periodically getting data
	while(1){
		// Display first two digits of voltage on 7-segment
		/*{
			int voltInt = (int)(fVoltage * 10);  // e.g., 9.88 -> 98
			uint8_t digit1 = (voltInt / 10) % 10;  // First digit (9)
			uint8_t digit2 = voltInt % 10;         // Second digit (8)
			Seg7LP('0' + digit1);
			_delay_ms(1000);
			Seg7LP('0' + digit2);
			_delay_ms(1000);
			Seg7LP('.');
			_delay_ms(100);
		}*/
		//No reason to get the charge if the charge is complete
		if (!legacy_charger) {
			fVoltage = LTC2959_get_voltage();
			battery_percent = (100*((float)LTC2959_get_charge()/(float)FULL_BATT_CAP_MA));
		} else {
			//cc_legacy_get_batt_volt();
			// Map raw ADC value to 0-100% using calibrated values
			float battPercent = (fVoltage - LEGACY_RAW_EMPTY) / (LEGACY_RAW_FULL - LEGACY_RAW_EMPTY) * 100.0f;
			battery_percent = (uint8_t)fmax(0, fmin(100, battPercent));
		}
		//EE_Wr(EE_BATT_VOLTAGE, 0.0f);
		if (cc_get_saved_batt_volt() == 0.0f && !USBPresent()) {
			// USB just connected - save the last battery-only voltage reading
			uint32_t voltage_mv = (uint32_t)(fVoltage * 1000.0f);
			EE_Wr(EE_BATT_VOLTAGE, voltage_mv);
			debug_message("Saved batt voltage: %lu mV (raw fV: %.2f)\r\n", voltage_mv, fVoltage);
		}
		debug_message("fVoltage %.2f, ucBattPerc: %d\r\n", fVoltage, battery_percent);
		osWAIT(250);
		osWAIT(250);
		osWAIT(250);
		osWAIT(250);
	}

END_TASK

void cc_legacy_get_batt_volt()
{
	fVoltage = 0.3;
	return;
	AnalogPowerOn();  // Ensure analog power is on for ADC reading
	InitADCs();
	// Use external AREFB reference (same as ECG acquisition) - internal 1V is too low
	ADC_Reference_Config(&ADCA, ADC_REFSEL_AREFB_gc);
	ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV256_gc);	// 128KSPS @ 32MHZ
	ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_BATT, 0);		// monitor battery voltage on ADCA
	ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_BATT, 0);		// monitor battery voltage on ADCA
	ADC_Ch_InputMux_Config(&ADCA.CH2, ADC_CH_UNUSED_0, 0);// monitor ground
	ADC_Ch_InputMux_Config(&ADCA.CH3, ADC_CH_UNUSED_0, 0);// monitor ground
	_delay_ms(50);	// wait for power to stabilize
	ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm| ADC_CH3START_bm) );
	_delay_ms(10);
	ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm| ADC_CH3START_bm) );
	_delay_ms(10);
	debug_message("ADC: CH0=%d CH1=%d CH2=%d CH3=%d\r\n", ADCA.CH0.RES, ADCA.CH1.RES, ADCA.CH2.RES, ADCA.CH3.RES);
	fVoltage = (ADCA.CH0.RES + ADCA.CH1.RES - ADCA.CH2.RES - ADCA.CH3.RES)/(8190.0-ADCA.CH2.RES - ADCA.CH3.RES);
	InitADCs();
}

uint8_t cc_get_batt_per()
{
	return battery_percent;
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
	EE_Wr(EE_FG_VALID, 1);
}

void cc_clear_fg_valid()
{
	EE_Wr(EE_FG_VALID, 0);
}

float cc_get_saved_batt_volt()
{
	uint32_t voltage_mv = EE_Rd(EE_BATT_VOLTAGE);
	// Sanity check: valid raw ADC range is ~150-350 mV (0.15-0.35 raw)
	if (voltage_mv < 150 || voltage_mv > 350) {
		return 0.0f;  // Invalid/uninitialized
	}
	return voltage_mv / 1000.0f;
}
#endif