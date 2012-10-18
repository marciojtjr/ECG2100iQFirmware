#include "adc_driver.h"


// clear the interrupt flag and return the signed conversion result
int16_t ADC_ResultCh_GetWord_Offset(ADC_CH_t * adc_ch, int8_t signedOffset){
	adc_ch->INTFLAGS = ADC_CH_CHIF_bm;	// Clear interrupt flag
	return(adc_ch->RES - signedOffset);
}

// clear the interrupt flag and return the raw conversion result (no offset compensation)
int16_t ADC_ResultCh_GetWord(ADC_CH_t * adc_ch){
	adc_ch->INTFLAGS = ADC_CH_CHIF_bm;	// Clear interrupt flag
	return adc_ch->RES;
}

// wait until adc common mode is settled
void ADC_Wait(ADC_t * adc){
  uint8_t prescaler_val = adc->PRESCALER; // Store current prescaler value
	adc->PRESCALER = ADC_PRESCALER_DIV4_gc;	// Set prescaler value to minimum
	delay_us(8*COMMON_MODE_CYCLES);	// Wait for common mode to settle
	adc->PRESCALER = prescaler_val;	// return to previous prescaler value
}

// get the 8-bit offset of the ADC
int8_t ADC_Offset_Get(ADC_CH_t *ch){
  int16_t offset=0;
  for (int i=0; i<16; i++){
    ADC_Ch_Conversion_Start(ch);
		while(!ADC_Ch_Conversion_Complete(ch));
    offset += ADC_ResultCh_GetWord(ch);
  }
  return ((int8_t)(offset/16));
}

// calibrate the ADC from production calibration memory
void ADC_CalibrationValues_Load(ADC_t * adc){
	if(adc == &ADCA){
		adc->CALL = SP_ReadCalibrationByte( PROD_SIGNATURES_START + ADCACAL0_offset );
		adc->CALH = SP_ReadCalibrationByte( PROD_SIGNATURES_START + ADCACAL1_offset );
	}else {
		adc->CALL = SP_ReadCalibrationByte( PROD_SIGNATURES_START + ADCBCAL0_offset );
		adc->CALH = SP_ReadCalibrationByte( PROD_SIGNATURES_START + ADCACAL1_offset );
	}
}

// read out calibration byte.
uint8_t SP_ReadCalibrationByte( uint8_t index ){
	uint8_t result;
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
 	result = pgm_read_byte(index);
 	NVM_CMD = NVM_CMD_NO_OPERATION_gc; // Clean up NVM Command register
	return result;
}
