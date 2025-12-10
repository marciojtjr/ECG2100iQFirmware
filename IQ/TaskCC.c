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

#define LEGACY_FULL_BATT_CAP_MA	4.2
/*
*   A separate task for querying the coulomb counter over TWI.
*/

static float fVoltage = 4.2;		//Start off full before 1st reading
static uint8_t ucBattPerc = 100;	//Start off full before 1st reading
static bool charge_complete = false;
bool legacy_charger = false;

BEGIN_TASK(TaskCC)
    //Declare variables necessary for TWI
	debug_message("TaskCC\r\n");
    //Declare variable for getting coulomb counter information
	cc_legacy_get_batt_volt();
	legacy_charger = fVoltage > 0;
	debug_message("LEGACY fVoltage %f\r\n", fVoltage);
	if (!legacy_charger) {
		//Setup twi and coulomb counter
		twi_init(LTC2959_get_i2c_addr());
		LTC2959_init();
	}	
    //Start while loop for periodically getting data
	while(1){
		//No reason to get the charge if the charge is complete
		if (!legacy_charger) {
			fVoltage = LTC2959_get_voltage();
		} else {
			cc_legacy_get_batt_volt();
		}
		if(!charge_complete) {
			if (legacy_charger) {
				float vBattDelta = VBatt-3.70f;
				vBattDelta *= 200.0f;
				ucBattPerc = (int)fmax(0, vBattDelta/10);
			} else {
				ucBattPerc = (100*((float)fVoltage/(float)(legacy_charger ? LEGACY_FULL_BATT_CAP_MA : FULL_BATT_CAP_MA)));
			}
		}
		//Read the eeprom FG validity value
		int fg = EE_Rd(EE_FG_VALID);
		//debug_message("fVoltage %.2f, ucBattPerc: %d, fg: %d\r\n", fVoltage, ucBattPerc, fg);
		osWAIT(250);
		osWAIT(250);
		osWAIT(250);
		osWAIT(250);
	}

END_TASK

void cc_legacy_get_batt_volt()
{
	InitADCs();
	ADC_Reference_Config(&ADCA, ADC_REFSEL_INT1V_gc); 		// temporarily use internal bandgap
	ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV256_gc);	// 128KSPS @ 32MHZ
	ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_BATT, 0);		// monitor battery voltage on ADCA
	ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_BATT, 0);		// monitor battery voltage on ADCA
	ADC_Ch_InputMux_Config(&ADCA.CH2, ADC_CH_UNUSED_0, 0);// monitor ground
	ADC_Ch_InputMux_Config(&ADCA.CH3, ADC_CH_UNUSED_0, 0);// monitor ground
	osWAIT(50);	// wait for power to stabilize
	ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm| ADC_CH3START_bm) );
	osWAIT(1);
	ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm| ADC_CH3START_bm) );
	osWAIT(1);
		
	fVoltage = (ADCA.CH0.RES + ADCA.CH1.RES - ADCA.CH2.RES - ADCA.CH3.RES)/(8190.0-ADCA.CH2.RES - ADCA.CH3.RES)*4.2;
	InitADCs();
}

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
	EE_Wr(EE_FG_VALID, 1);
}

void cc_clear_fg_valid()
{
	EE_Wr(EE_FG_VALID, 0);
}
