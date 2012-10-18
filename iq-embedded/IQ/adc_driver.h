#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include "avr_compiler.h"

#if F_CPU < 8000000
#define COMMON_MODE_CYCLES 16
#else
#define COMMON_MODE_CYCLES 32
#endif

/* Macros */

/* note After the ADC is enabled the common mode voltage is stable after 12 ADC clock cycles.
	Do one dummy conversion or wait the required number of clock cycles to assure correct conversion. */
#define ADC_Enable(_adc) ((_adc)->CTRLA |= ADC_ENABLE_bm)
#define ADC_Disable(_adc) ((_adc)->CTRLA = (_adc)->CTRLA & (~ADC_ENABLE_bm))
#define ADC_Pipeline_Flush(_adc) ((_adc)->CTRLA |= ADC_FLUSH_bm)


// Configure the ADC for signed or unsigned, set the resolution and alignment
// _signedMode   signed (true), unsigned (false). USE bool type.
// _resolution   Resolution and presentation selection, ADC_RESOLUTION_t type

#define ADC_ConvMode_and_Resolution_Config(_adc, _signedMode, _resolution)     \
	((_adc)->CTRLB = ((_adc)->CTRLB & (~(ADC_RESOLUTION_gm|ADC_CONMODE_bm)))|  \
		(_resolution| ( _signedMode? ADC_CONMODE_bm : 0)))

// macros for increased readability with ADC_ConvMode_and_Resolution_Config
#define ADC_ConvMode_Signed true
#define ADC_ConvMode_Unsigned false


// set the ADC prescaler
// note  The maximum ADC sample rate is always one fourth of the IO clock.
// _adc  Pointer to ADC module register section.
#define ADC_Prescaler_Config(_adc, _div) ((_adc)->PRESCALER = ((_adc)->PRESCALER & (~ADC_PRESCALER_gm)) | _div)


// pick the reference for the selected adc.
// _convRef  Selects reference voltage for all conversions, Use ADC_REFSEL_t type.
#define ADC_Reference_Config(_adc, _convRef) ((_adc)->REFCTRL = ((_adc)->REFCTRL & ~(ADC_REFSEL_gm)) | _convRef)


// set sweep channels
// _sweepChannels  Sweep channel selection. Use ADC_SWEEP_t type
#define ADC_SweepChannels_Config(_adc, _sweepChannels) ((_adc)->EVCTRL = ((_adc)->EVCTRL & (~ADC_SWEEP_gm)) | _sweepChannels)


// configure the event channels used and the event mode.
// Use the eventChannels parameter to select which event
// channel to associate with virtual channel 0 or to trigger a conversion sweep,
// depending on the selected eventMode parameter.
// _eventChannels  The first event channel to be used for triggering. Use ADC_EVSEL_t type.
// _eventMode      Select event trigger mode. Use ADC_EVACT_t type.
#define ADC_Events_Config(_adc, _eventChannels, _eventMode) (_adc)->EVCTRL = ((_adc)->EVCTRL & (~(ADC_EVSEL_gm | ADC_EVACT_gm))) | ((uint8_t) _eventChannels | _eventMode)


// configure interrupt mode and level for one channel.
// The interrupt mode affects the interrupt flag for the virtual channel,
// and thus also affects code that polls this flag instead of using interrupts.
// note  When using the result comparator function, the compare value must be set using the ADC_SetCompareValue function.
// _interruptMode   Interrupt mode, flag on complete or above/below compare value. Use ADC_CH_INTMODE_t type.
// _interruptLevel  Disable or set low/med/high priority for this virtual channel. Use ADC_CH_INTLVL_t type.
#define ADC_Ch_Interrupts_Config(_adc_ch, _interruptMode, _interruptLevel) (_adc_ch)->INTCTRL = (((_adc_ch)->INTCTRL & (~(ADC_CH_INTMODE_gm | ADC_CH_INTLVL_gm))) | ((uint8_t) _interruptMode | _interruptLevel))


// configure input mode and gain for the specified virtual channel.
// _inputMode      Input mode for this channel, differential, single-ended, gain etc. Use ADC_CH_INPUTMODE_t type.
// _gain           The preamplifiers gain value. Use ADC_CH_GAINFAC_t type.
#define ADC_Ch_InputMode_and_Gain_Config(_adc_ch, _inputMode, _gain) (_adc_ch)->CTRL = ((_adc_ch)->CTRL & (~(ADC_CH_INPUTMODE_gm|ADC_CH_GAINFAC_gm))) | ((uint8_t) _inputMode|_gain)


// configure the Positive and negative inputs.
// _posInput  Which pin (or internal signal) to connect to positive ADC input. Use ADC_CH_MUXPOS_enum type.
// _negInput  Which pin to connect to negative ADC input. Use ADC_CH_MUXNEG_t type.
// note:  The negative input is connected to GND for single-ended and internal input modes.
#define ADC_Ch_InputMux_Config(_adc_ch, _posInput, _negInput) ((_adc_ch)->MUXCTRL = (uint8_t) _posInput | _negInput)


// return the channel conversion complete flag..
#define ADC_Ch_Conversion_Complete(_adc_ch) (((_adc_ch)->INTFLAGS & ADC_CH_CHIF_bm) != 0x00)


/* set the value in the ADC compare register.
 *
 *  The value in the ADC compare register is used by the result comparator for
 *  channels that are configured to notify when result is above or below this
 *  value. Even if the ADC compare value register is always left adjusted, the input
 *  to this function is adjusted according to the result presentation setup
 *  for the ADC. This means that the value will be right adjusted unless the
 *  "12-bit left adjust" result mode is selected with
 *  ADC_ConvMode_and_Resolution_Config.
*/
#define ADC_CompareValue_Set(_adc, _value) ((_adc)->CMP = _value)


#define ADC_FreeRunning_Enable(_adc)  ((_adc)->CTRLB |= ADC_FREERUN_bm)
#define ADC_FreeRunning_Disable(_adc) ((_adc)->CTRLB = (_adc)->CTRLB & (~ADC_FREERUN_bm))


// start a one channel conversion (don't use if free-running)
#define ADC_Ch_Conversion_Start(_adc_ch) ((_adc_ch)->CTRL |= ADC_CH_START_bm)


// start conversions on multiple channels
// _channelMask  A bitmask selecting which channels to start
#define ADC_Conversions_Start(_adc, _channelMask) (_adc)->CTRLA |= _channelMask & (ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm | ADC_CH3START_bm)


// note:  If the ADC is enabled the Bandgap Reference is automatically enabled
#define ADC_BandgapReference_Enable(_adc) ((_adc)->REFCTRL |= ADC_BANDGAP_bm)
#define ADC_BandgapReference_Disable(_adc) ((_adc)->REFCTRL &= ~ADC_BANDGAP_bm)

// note  Enabling the temperature reference automatically enables the bandgap reference.
#define ADC_TempReference_Enable(_adc)	((_adc)->REFCTRL |= ADC_TEMPREF_bm)
#define ADC_TempReference_Disable(_adc) ((_adc)->REFCTRL = (_adc)->REFCTRL & (~ADC_TEMPREF_bm))


/* Prototypes for functions. */
void ADC_CalibrationValues_Load(ADC_t * adc);
int16_t ADC_ResultCh_GetWord_Offset(ADC_CH_t * adc_ch, int8_t offset);
int16_t ADC_ResultCh_GetWord(ADC_CH_t * adc_ch);
int8_t ADC_Offset_Get(ADC_CH_t *ch);
void ADC_Wait(ADC_t * adc);
uint8_t SP_ReadCalibrationByte( uint8_t index );

//Offset addresses for production signature row (not in current GCC header files)
#ifndef ADCACAL0_offset
#define ADCACAL0_offset 0x20
#define ADCACAL1_offset 0x21
#define ADCBCAL0_offset 0x24
#define ADCBCAL1_offset 0x25
#endif

#ifndef ADC_CH_MUXPOS_PIN8_gc
#define ADC_CH_MUXPOS_PIN8_gc  (ADC_CH_MUXPOS_PIN0_gc + 0x10<<3)
#define ADC_CH_MUXPOS_PIN9_gc	 (ADC_CH_MUXPOS_PIN1_gc + 0x10<<3)
#define ADC_CH_MUXPOS_PIN10_gc (ADC_CH_MUXPOS_PIN2_gc + 0x10<<3)
#define ADC_CH_MUXPOS_PIN11_gc (ADC_CH_MUXPOS_PIN3_gc + 0x10<<3)
#endif

#endif
