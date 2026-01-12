/*
 * taskCC.h
 *
 * Created: 10/31/2025 12:26:41 PM
 *  Author: adunk
 */ 


#ifndef TASKCC_H_
#define TASKCC_H_

uint8_t cc_get_batt_per();
float cc_get_batt_volt();
void cc_set_charge_complete();
void cc_clear_charge_complete();
bool cc_get_charge_complete();
void cc_set_fg_valid();
void cc_clear_fg_valid();
bool cc_get_fg_valid();



#endif /* TASKCC_H_ */