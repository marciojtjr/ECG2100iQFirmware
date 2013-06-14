#include <avr/io.h>
#include "rtos.h"
#include "iq.h"
#include "adc_driver.h"
#include "taskAcq.h"
#include "TC_driver.h"
#include "buffers.h"
#include "taskFlash.h"
#include "ulaw.h"
#include "seg7.h"
#include "eeprom.h"
#include "eeprom_driver.h"

#include "taskUSB.h"
#include "taskBT.h"
#include "TaskWatchDog.h"

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------DEBUGGING STUFF---------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
#define LOGGING_TO_FLASH // Define if we want to log to flash memory during preAcq

#ifdef LOGGING_TO_FLASH
#include <string.h>
#endif
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------


// ECG Lead assignments for PortA and PortB

// ADC channel assignments for Port A
#define	ADC_CH_UNUSED_0		ADC_CH_MUXPOS_PIN0_gc
#define ADC_CH_RL_MON 		ADC_CH_MUXPOS_PIN1_gc
#define ADC_CH_BATT				ADC_CH_MUXPOS_PIN2_gc
#define ADC_CH_UNUSED_3		ADC_CH_MUXPOS_PIN3_gc
#define ADC_CH_V6					ADC_CH_MUXPOS_PIN4_gc
#define ADC_CH_V5		 			ADC_CH_MUXPOS_PIN5_gc
#define ADC_CH_V4					ADC_CH_MUXPOS_PIN6_gc
#define ADC_CH_V3					ADC_CH_MUXPOS_PIN7_gc

// ADC channel assignments for Port B
#define ADC_CH_VREF				ADC_CH_MUXPOS_PIN0_gc
#define ADC_CH_RA_MON			ADC_CH_MUXPOS_PIN1_gc
#define ADC_CH_MIC				ADC_CH_MUXPOS_PIN2_gc
#define ADC_CH_V2					ADC_CH_MUXPOS_PIN4_gc
#define ADC_CH_V1					ADC_CH_MUXPOS_PIN5_gc
#define ADC_CH_LL					ADC_CH_MUXPOS_PIN6_gc
#define ADC_CH_LA					ADC_CH_MUXPOS_PIN7_gc

#define PORT_LA_V2 PORTD
#define REZERO_LA	PIN3_bm
#define REZERO_LL	PIN2_bm
#define REZERO_V1	PIN1_bm
#define REZERO_V2	PIN0_bm

#define PORT_V3_V6 PORTC
#define REZERO_V3	PIN3_bm
#define REZERO_V4	PIN2_bm
#define REZERO_V5	PIN1_bm
#define REZERO_V6	PIN0_bm

extern unsigned char AcqIrqState;	// a place to remember what the acquistion interrupt is doing
// when AcqState == ACQ_IDLE, the acquisition interrupt (which is always running) does nothing. All internal pointers are reset
// when AcqState == ACQ_VOICE, the interrupt samples the microphone input and puts samples into the available buffers. If neither buffer is available
// a SystemError.BUF_OVERRUN is flagged
// when AcqState >= ACQ_ECG, the interrupt accumulates ECG sub-samples into accumulators in preparation for decimation (simple average of 16 sub-samples)
//  AcqState is incremented each interrupt to keep track of the accumulation
// when AcqState incrments to ACQ_DECIMATION, the sub-sample accumulators are scaled back to 12 bits and the eight leads are sent off to an available buffer
//   if neither buffer is available, SystemError.BUF_OVERRUN is flagged
enum {ACQ_IRQ_IDLE, ACQ_IRQ_LOGGING, ACQ_IRQ_VOICE, ACQ_IRQ_ECG, ACQ_IRQ_ECG_DECIMATE=ACQ_IRQ_ECG+15};	// ACQ_IRQ_ECG increments into ACQ_IRQ_DECIMATE, then rolls back to ACQ_IRQ_ECG

unsigned char AcqIrqState = ACQ_IRQ_IDLE;	// used to hold the current acquisition interrupt state, a task may control acquisition via this "semaphore"
enum {LOCKED, UNLOCKED};
unsigned char AcqBufferLock = LOCKED;

#define EnableBuffering() AcqBufferLock = UNLOCKED;
#define DisableBuffering() AcqBufferLock = LOCKED;

unsigned int Acc[8];	// sub-sample lead accumulators
int Lead[8];	// final ECG leads

unsigned int sampleIndex; // used to index into the ECG buffer
unsigned int voiceIndex;	// used to index into the Voice buffer

enum {LA, LL, V1,  V2,  V3, V4, V5, V6, V7};	// offsets into Lead/Acc arrays

float VBatt;
unsigned int RA_Mon, RL_Mon;

// Lead Fail Stuff
char LFBiasEnable;	// 1 = inject 250Hz bias, 0 = don't
int LFBiasModel[2][8][4];	// two models (so we can ping-pong) for the eight channels, each model containing four samples
unsigned char M;	// selects which of the two LFBiasModels is ready for examination, 0 or 1
unsigned char LFBiasModelIndex; // used to index into the four subsamples of each LFBias model
int LFLo, LFHi;	// holds extents of LF swing for each channel
unsigned int LFMag, LFMax, LFMin, LFMaxFilt, LFMinFilt; // holds overall LF swing magnitude, the final indication of lead impedance
char QC;	// the final QC determination, used to drive 1/2/3 bars on the LED
unsigned int B1, B2, B3; // Thresholds (obtained from EEPROM) for 2/3 bars due to lead impedance

long AbandonTimer; // used to timeout pre-acquisition if the unit is abandoned
unsigned char LFReady; // used by the acquisition interrupt to tell the LF checker that data is ready
unsigned char BTEnable;	// 1 enables bluetooth transmission
unsigned char BTReady;	// 1 = a leadfail bias model is ready for decimation, 0 = not ready
unsigned char BTBufPhase; // 0 = decimation of LFBiasModel[0] loads the BTBuf, 1 = decimation of LFBiasModel[1] adds to BTBuf, after which /8 produces an average of both models
unsigned int BTBuf[8];	// holds all eight channels of data, decimated from 1KHz to 125Hz.
// unsigned char BTString[29]; // room to store the Bluetooth sample output string.
unsigned char BTString[6+1+1+1+2+8*12+2]; // Bigger now.
unsigned char BTStatus;

//unsigned int test;

//------------------------------------------------------------------------------------------
// return the battery voltage, in mV.
// If acquisition is running (AcqIrqState != ACQ_IRQ_IDLE) we use the raw battery ADC reading
// provided by the interrupt routine. If acquisition is NOT running, we'll do a direct ADC conversion here
unsigned int MeasureBatt(void){
	if(AcqIrqState != ACQ_IRQ_IDLE){
		return(VBatt);
	}
	else{
		return(1);
	}
}

//------------------------------------------------------------------------------------------
// pack an array of eight 16-bit signed ECG samples (of which 12 bits are valid) into 12 bytes
// a 16-bit ECG sample array looks like this in memory (low address first)
// address 0:1 c0 ab (abc0) LA
// address 2:3 f0 de (def0)	LL
// address 4:5 i0 gh (ghi0) V1
// address 6:7 l0 jk (jlk0) V2
// address 8:9 o0 mn (mno0) V3
// address a:b r0 pq (pqr0) V4
// address c:d u0 st (stu0) V5
// address e:f x0 vw (vwx0) V6

// after packing, the samples look like this
// address 0:1 ab cf
// address 2:3 de gh
// address 4:5 il jk
// address 6:7 mn or
// address 8:9 pq st
// address a:b ux wv

void PackECG(unsigned char *out, unsigned char *in){
	unsigned char temp,i;

	for(i=0;i<4;i++){	// pack the eight channels in four pairs (4 bytes packs into 3)
		temp 	 = *in++ & 0xf0; 			// low nibble of first channel in a pair
		*out++ = *in++;							// high byte of first channel in a pair
		*out++ = *in++ >> 4 | temp;	// low nibble of second channel in a pair
		*out++ = *in++;							// high byte of second channel in a pair
	}
}

//------------------------------------------------------------------------------------------
// unpack an array of 12 bytes into eight 16-bit signed values

// all ECG measurements are made with respect to RA
// each ECG lead is digitized to 12-bit resolution (three nibbles) as follows:

// abc LA
// def LL
// ghi V1
// jlk V2
// mno V3
// pqr V4
// stu V5
// vwx V6

// the packed samples look like this
// address 0:1 ab cf
// address 2:3 de gh
// address 4:5 il jk
// address 6:7 mn or
// address 8:9 pq st
// address a:b ux wv

// after unpacking, they look like this (-) is an extension of the sign of the result into the high nibble
// address 0 -abc LA
// address 1 -def LL
// address 2 -ghi V1
// address 3 -jlk V2
// address 4 -mno V3
// address 5 -pqr V4
// address 6 -stu V5
// address 7 -vwx V6

void UnPackECG(signed int *out, unsigned char *in){
	unsigned char i;
	signed int temp;

	for(i=0;i<4;i++){						// unpack the eight channels in four pairs (3 bytes unpack into four)
		temp = *in++<<8; 					// high byte of first channel in a pair
		temp += *in & 0xf0;				// low nibble
		*out++ = temp>>4;					// scale back to 12 bits
		temp = (*in++ & 0xf)<<4;	// low nibble of second sample
		temp += *in++<<8;					// high byte of second channel in a pair
		*out++ = temp>>4;					// scale back to 12 bits
	}
}

#define PWM_PERIOD 64000	// 2KHz

//############################################################################################
// Prepare both A/D converters
void InitADCs(void){
	//------------------------------------------------------------------------------------------
	// setup ADCA
	ADC_CalibrationValues_Load(&ADCA);
  ADC_ConvMode_and_Resolution_Config(&ADCA, ADC_ConvMode_Unsigned, ADC_RESOLUTION_12BIT_gc);
	ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV32_gc);	// 1MSPS @ 32MHZ
	ADC_Reference_Config(&ADCA, ADC_REFSEL_AREFB_gc); // VRef for both channels is on PortB
	ADC_Enable(&ADCA);
	// all conversions are done single ended
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH0, ADC_CH_INPUTMODE_SINGLEENDED_gc, 0);
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH1, ADC_CH_INPUTMODE_SINGLEENDED_gc, 0);
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH2, ADC_CH_INPUTMODE_SINGLEENDED_gc, 0);
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH3, ADC_CH_INPUTMODE_SINGLEENDED_gc, 0);

	//------------------------------------------------------------------------------------------
	// setup ADCB
	ADC_CalibrationValues_Load(&ADCB);
  ADC_ConvMode_and_Resolution_Config(&ADCB, ADC_ConvMode_Unsigned, ADC_RESOLUTION_12BIT_gc);
	ADC_Prescaler_Config(&ADCB, ADC_PRESCALER_DIV32_gc);	// 1MSPS @ 32MHZ
	ADC_Reference_Config(&ADCB, ADC_REFSEL_AREFB_gc); // VRef for both channels is on PortB
	ADC_Enable(&ADCB);
	// all conversions are done single ended
	ADC_Ch_InputMode_and_Gain_Config(&ADCB.CH0, ADC_CH_INPUTMODE_SINGLEENDED_gc, 0);
	ADC_Ch_InputMode_and_Gain_Config(&ADCB.CH1, ADC_CH_INPUTMODE_SINGLEENDED_gc, 0);
	ADC_Ch_InputMode_and_Gain_Config(&ADCB.CH2, ADC_CH_INPUTMODE_SINGLEENDED_gc, 0);
	ADC_Ch_InputMode_and_Gain_Config(&ADCB.CH3, ADC_CH_INPUTMODE_SINGLEENDED_gc, 0);
}

//############################################################################################
// Prepare both A/D converters to measure ECG/voice and battery
void StartAcq(unsigned char state){

	LFBiasEnable = EE_Rd(EE_LFBIAS_ENABLE);
	AcqIrqState = state;
	DisableBuffering();
	InitADCs();

	if(state == ACQ_IRQ_VOICE){
		// we've got all channels on ADCB available for voice
		// might as well use them to try to lower the noise floor on the voice input a little
		ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_BATT, 0);	// monitor battery voltage on ADCA
		ADC_Ch_InputMux_Config(&ADCB.CH0, ADC_CH_MIC, 0);
		ADC_Ch_InputMux_Config(&ADCB.CH1, ADC_CH_MIC, 0);
		ADC_Ch_InputMux_Config(&ADCB.CH2, ADC_CH_MIC, 0);
		ADC_Ch_InputMux_Config(&ADCB.CH3, ADC_CH_MIC, 0);
	}
	else{
		ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_V3, 0);		// set up for ECG measurements across PortA
		ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_V4, 0);
		ADC_Ch_InputMux_Config(&ADCA.CH2, ADC_CH_V5, 0);
		ADC_Ch_InputMux_Config(&ADCA.CH3, ADC_CH_V6, 0);
		ADC_Ch_InputMux_Config(&ADCB.CH0, ADC_CH_LA, 0);		// set up for ECG measurements across PortB
		ADC_Ch_InputMux_Config(&ADCB.CH1, ADC_CH_LL, 0);
		ADC_Ch_InputMux_Config(&ADCB.CH2, ADC_CH_V1, 0);
		ADC_Ch_InputMux_Config(&ADCB.CH3, ADC_CH_V2, 0);

		// set up the PWM channels for the eight offset servos
		/*
		PORTC.DIRSET = PIN0_bm + PIN1_bm + PIN2_bm + PIN3_bm;	// V6-V3
		PORTD.DIRSET = PIN0_bm + PIN1_bm + PIN2_bm + PIN3_bm;	// V2-LA
		TC_SetPeriod( &TCC0, PWM_PERIOD-1 );	// set the TC period for V6-V3
		TC_SetPeriod( &TCD0, PWM_PERIOD-1 );	// set the TC period for V2-LA
		TC0_ConfigWGM( &TCC0, TC_WGMODE_SS_gc );	// set single slope mode
		TC0_ConfigWGM( &TCD0, TC_WGMODE_SS_gc );	// set single slope mode
		TC0_EnableCCChannels( &TCC0, TC0_CCAEN_bm + TC0_CCBEN_bm + TC0_CCCEN_bm + TC0_CCDEN_bm);	// enable compare channels A-D
		TC0_EnableCCChannels( &TCD0, TC0_CCAEN_bm + TC0_CCBEN_bm + TC0_CCCEN_bm + TC0_CCDEN_bm);	// enable compare channels A-D
		TC0_ConfigClockSource( &TCC0, TC_CLKSEL_DIV1_gc );	// start timer by selecting clock source 1x
		TC0_ConfigClockSource( &TCD0, TC_CLKSEL_DIV1_gc );	// start timer by selecting clock source 1x
		HIRESC.CTRL = HIRES_HREN_TC0_gc;
		HIRESD.CTRL = HIRES_HREN_TC0_gc;
		*/
	}
	// start conversions so AcqInterrupt() won't hang waiting for data
	ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm | ADC_CH3START_bm));
	ADC_Conversions_Start(&ADCB, (ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm | ADC_CH3START_bm));
}

//############################################################################################
// disable the acquisition interrupt
void StopAcq(void){
	AcqIrqState = ACQ_IRQ_IDLE;
	DisableBuffering();
}

	//--------------------------------------------------------------------------------------------------------------
	//--------------------------------------------DEBUGGING STUFF---------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------
	void LogMessage(char *message) {
#ifdef LOGGING_TO_FLASH
		size_t len = strlen(message) + 1;

		// If our message + newline won't fit into the buffer, just return
		if(len >= FLASH_SIZE_BYTES || FlashPageNumber>FLASH_VOICE_END_PAGE)
			return;

		// If we're ready to start writing out...
		if(AcqIrqState != ACQ_IRQ_IDLE) {
			//... and buffer 1 is empty
			if(Buf1.Status == BUF_EMPTY) {			
				// Append a newline and set the buffer to the message
				// This is inefficient by design. We want to dump messages out
				// as soon as possible, so wasting page space isn't too big of a deal.
				// We have 14k pages anyway
				memcpy(Buf1.Data.Byte, message, len);

				// Tell TaskFlash we're ready to write
				Buf1.Status = BUF_FULL;
				TaskFlashCmd = TF_WRITE_BUF1;

				while(TaskFlashCmd != TF_IDLE) {
					osYIELD;
				}

				Buf1.Status = BUF_EMPTY; // return the buffer to the interrupt handler
                FlashPageNumber++;
                if(FlashPageNumber>FLASH_VOICE_END_PAGE){
                    return;
                }
			}
			//... and buffer 2 is empty
			else if (Buf2.Status == BUF_EMPTY) {
				memcpy(Buf2.Data.Byte, message, len);

				Buf2.Status = BUF_FULL;
				TaskFlashCmd = TF_WRITE_BUF2;

				while(TaskFlashCmd != TF_IDLE) {
					osYIELD;
				}

				Buf2.Status = BUF_EMPTY; // return the buffer to the interrupt handler
                FlashPageNumber++;
                if(FlashPageNumber>FLASH_VOICE_END_PAGE){
                    return;
                }
			}
		}
#endif
	}
	//--------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// this function is called by the SysTick interrupt
void AcqInterrupt(){

	static int index;	// keeps track of where we are in buffer stuffing
	int VoiceSample;
	static int VoiceBaseline;	// centers VoiceSample around zero for good uLaw compression

	if(AcqIrqState != ACQ_IRQ_IDLE){ // only do something if there is something to do
	// The previous interrupt will have started conversions on the eight ECG leads or the microphone input
	// That'll be done by now, so we can drive straight into getting the conversion values
	// For ECG, during decimation (mode == DECIMATION), we transfer the decimated ECG/monitoring values into the output buffers
	// most times (mode >= ACCUMULATE and < ACQ_DECIMATE) we just accumulate new ECG measurements into the decimation accumulators
	// During voice acquisition, we transfer the voice sample into the output buffer

	//--------------------------------------------------------------------------------------------------------------
	//--------------------------------------------DEBUGGING STUFF---------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------
#ifndef LOGGING_TO_FLASH // If we're logging, don't throw anything out to the buffers
	//--------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------

		if(ADC_Ch_Conversion_Complete(&ADCB.CH3)){	// the ADCs should be ready, no harm in checking

			//------------------------------------------------------------------------------------------
			// process a Voice sample, if the buffers are locked, we can still keep the baseline filter running
			if(AcqIrqState == ACQ_IRQ_VOICE){

				// this is that AcqStart has set up for us...
				// we've got all channels on ADCB available
				// might as well use them to try to lower the noise floor on the voice input a little
				// ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_BATT, 0);	// monitor battery voltage on ADCA
				// ADC_Ch_InputMux_Config(&ADCB.CH1, ADC_CH_MIC, 0);
				// ADC_Ch_InputMux_Config(&ADCB.CH2, ADC_CH_MIC, 0);
				// ADC_Ch_InputMux_Config(&ADCB.CH3, ADC_CH_MIC, 0);

				VBatt = ADCA.CH0.RES;
				VoiceSample  = ADCB.CH0.RES;	// add up 4 conversions to produce a 14-bit, lower noise result
				VoiceSample += ADCB.CH1.RES;
				VoiceSample += ADCB.CH2.RES;
				VoiceSample += ADCB.CH3.RES;
				if(VoiceSample>VoiceBaseline){
					VoiceBaseline++;
				}
				else if(VoiceSample<VoiceBaseline){
					VoiceBaseline--;
				}

				if(AcqBufferLock == UNLOCKED){	// store samples when the task is ready

					VoiceSample -= VoiceBaseline;	// center the voice sample around zero
					VoiceSample = linear2ulaw(VoiceSample);

					// a voice sample is ready, figure out where to put it

					if(index==0){	// we're beginning a new memory page, find a free buffer to put it in
						if(Buf1.Status == BUF_EMPTY){
							Buf1.Status = BUF_FILLING;
						}
						else if(Buf2.Status == BUF_EMPTY){
							Buf2.Status = BUF_FILLING;
						}
						else{
							while(1){
								osWAIT(1);
								DPLEDTog();	// this should never happen
							}
						}
					}
					if(index<FLASH_PAGE_SIZE){ // we're currently filling a memory page, figure out which one
						if(Buf1.Status == BUF_FILLING){
							Buf1.Data.Voice[index++]=VoiceSample;
						}
						else{
							Buf2.Data.Voice[index++]=VoiceSample;
						}
					}
					if(index>=FLASH_PAGE_SIZE){ // we've finished a page
						if(Buf1.Status == BUF_FILLING){
							Buf1.Status = BUF_FULL;
						}
						else{
							Buf2.Status = BUF_FULL;
						}
						index=0;	// next interrupt will start a new buffer
					}
				}
				else{ // AcqBufferLock == LOCKED
					index = 0;
				}
			}

			//------------------------------------------------------------------------------------------
			// Produce an ECG sample set from the average of the previous 16 conversions
			else if(AcqIrqState==ACQ_IRQ_ECG_DECIMATE){

				#define UPPER_THRESH 65534
				#define LOWER_THRESH 3200

				// rezero any lead that goes out of bounds
				if(Acc[V3]>UPPER_THRESH){
					Acc[V3] = 65535;
					PORT_V3_V6.OUTCLR = REZERO_V3;
					PORT_V3_V6.DIRSET = REZERO_V3;
				}
				else if(Acc[V3] < LOWER_THRESH){
					Acc[V3]=0;
					PORT_V3_V6.OUTSET = REZERO_V3;
					PORT_V3_V6.DIRSET = REZERO_V3;
				}
				else {
					PORT_V3_V6.DIRCLR = REZERO_V3;
				}
				// convert the unsigned values to signed and transfer to the Leads
				Lead[V3] = Acc[V3]-0x8000;
				//------------------------------------

				if(Acc[V4]>UPPER_THRESH){
					Acc[V4] = 65535;
					PORT_V3_V6.OUTCLR = REZERO_V4;
					PORT_V3_V6.DIRSET = REZERO_V4;
				}
				else if(Acc[V4] < LOWER_THRESH){
					Acc[V4]=0;
					PORT_V3_V6.OUTSET = REZERO_V4;
					PORT_V3_V6.DIRSET = REZERO_V4;
				}
				else {
					PORT_V3_V6.DIRCLR = REZERO_V4;
				}
				Lead[V4] = Acc[V4]-0x8000;
				//------------------------------------

				if(Acc[V5]>UPPER_THRESH){
					Acc[V4] = 65535;
					PORT_V3_V6.OUTCLR = REZERO_V5;
					PORT_V3_V6.DIRSET = REZERO_V5;
				}
				else if(Acc[V5] < LOWER_THRESH){
					Acc[V5]=0;
					PORT_V3_V6.OUTSET = REZERO_V5;
					PORT_V3_V6.DIRSET = REZERO_V5;
				}
				else {
					PORT_V3_V6.DIRCLR = REZERO_V5;
				}
				Lead[V5] = Acc[V5]-0x8000;
				//------------------------------------

				if(Acc[V6]>UPPER_THRESH){
					Acc[V6] = 65535;
					PORT_V3_V6.OUTCLR = REZERO_V6;
					PORT_V3_V6.DIRSET = REZERO_V6;
				}
				else if(Acc[V6] < LOWER_THRESH){
					Acc[V6]=0;
					PORT_V3_V6.OUTSET = REZERO_V6;
					PORT_V3_V6.DIRSET = REZERO_V6;
				}
				else {
					PORT_V3_V6.DIRCLR = REZERO_V6;
				}
				Lead[V6] = Acc[V6]-0x8000;
				//------------------------------------

				// reload the accumulators with fresh conversions
				Acc[V3] = ADCA.CH0.RES;
				Acc[V4] = ADCA.CH1.RES;
				Acc[V5] = ADCA.CH2.RES;
				Acc[V6] = ADCA.CH3.RES;

				// now that we've got the ADCA lead values, set ADCA up for RL/VBatt monitoring, which is not decimated
				ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_RL_MON, 0);
				ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_BATT, 0);
				ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm |ADC_CH1START_bm));

				// continue working on the ADCB conversions while ADCA converts RL/VBatt
				// rezero any lead that goes out of bounds
				if(Acc[LA]>UPPER_THRESH){
					Acc[LA] = 65535;
					PORT_LA_V2.OUTCLR = REZERO_LA;
					PORT_LA_V2.DIRSET = REZERO_LA;
				}
				else if(Acc[LA] < LOWER_THRESH){
					Acc[LA]=0;
					PORT_LA_V2.OUTSET = REZERO_LA;
					PORT_LA_V2.DIRSET = REZERO_LA;
				}
				else {
					PORT_LA_V2.DIRCLR = REZERO_LA;
				}
				// convert the unsigned values to signed and transfer to the Leads
				Lead[LA] = Acc[LA]-0x8000;

				//------------------------------------
				// continue working on the ADCB conversions while ADCA converts RL/VBatt
				// rezero any lead that goes out of bounds
				if(Acc[LL]>UPPER_THRESH){
					Acc[LL] = 65535;
					PORT_LA_V2.OUTCLR = REZERO_LL;
					PORT_LA_V2.DIRSET = REZERO_LL;
				}
				else if(Acc[LL] < LOWER_THRESH){
					Acc[LL]=0;
					PORT_LA_V2.OUTSET = REZERO_LL;
					PORT_LA_V2.DIRSET = REZERO_LL;
				}
				else {
					PORT_LA_V2.DIRCLR = REZERO_LL;
				}
				// convert the unsigned values to signed and transfer to the Leads
				Lead[LL] = Acc[LL]-0x8000;

				//------------------------------------
				// continue working on the ADCB conversions while ADCA converts RL/VBatt
				// rezero any lead that goes out of bounds
				if(Acc[V1]>UPPER_THRESH){
					Acc[V1] = 65535;
					PORT_LA_V2.OUTCLR = REZERO_V1;
					PORT_LA_V2.DIRSET = REZERO_V1;
				}
				else if(Acc[V1] < LOWER_THRESH){
					Acc[V1]=0;
					PORT_LA_V2.OUTSET = REZERO_V1;
					PORT_LA_V2.DIRSET = REZERO_V1;
				}
				else {
					PORT_LA_V2.DIRCLR = REZERO_V1;
				}
				// convert the unsigned values to signed and transfer to the Leads
				Lead[V1] = Acc[V1]-0x8000;

				//------------------------------------
				// continue working on the ADCB conversions while ADCA converts RL/VBatt
				// rezero any lead that goes out of bounds
				if(Acc[V2]>UPPER_THRESH){
					Acc[V2] = 65535;
					PORT_LA_V2.OUTCLR = REZERO_V2;
					PORT_LA_V2.DIRSET = REZERO_V2;
				}
				else if(Acc[V2] < LOWER_THRESH){
					Acc[V2]=0;
					PORT_LA_V2.OUTSET = REZERO_V2;
					PORT_LA_V2.DIRSET = REZERO_V2;
				}
				else {
					PORT_LA_V2.DIRCLR = REZERO_V2;
				}
				// convert the unsigned values to signed and transfer to the Leads
				Lead[V2] = Acc[V2]-0x8000;

				// reload the accumulators with fresh conversions
				Acc[LA] = ADCB.CH0.RES;
				Acc[LL] = ADCB.CH1.RES;
				Acc[V1] = ADCB.CH2.RES;
				Acc[V2] = ADCB.CH3.RES;

				// now that we've got the ADCB lead values, set ADCB up for RA monitoring, which is not decimated
				ADC_Ch_InputMux_Config(&ADCB.CH0, ADC_CH_RA_MON, 0);
				ADC_Conversions_Start(&ADCB, ADC_CH0START_bm);

				while(!ADC_Ch_Conversion_Complete(&ADCA.CH1));	// ADCA conversions should be done by now, but wait just in case
				RL_Mon = ADCA.CH0.RES;
				VBatt  = ADCA.CH1.RES;

				while(!ADC_Ch_Conversion_Complete(&ADCB.CH0));	// ADCB probably isn't done yet, wait for it
				RA_Mon = ADCB.CH0.RES;

				// now we can restore the input multiplexing to the ECG leads (!!!!! should we reload all the multiplexers?)
				ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_V3, 0); // restore mux, we just used this channel to measure RL_MON
				ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_V4, 0); // restore mux, we just used this channel to measure BATT
				ADC_Ch_InputMux_Config(&ADCB.CH0, ADC_CH_LA, 0); // restore mux, we just used this channel to measure RA_MON
				AcqIrqState = ACQ_IRQ_ECG; // back to accumulation

				// if LFBias is on, we build the bias signal models
				// LFBias is synchronous to sampling at 1/4 the rate. So there are four samples per cycle of the LFBias generator.

				#if BLUETOOTH != 1 // in bluetooth builds, we build the lf model even if lfbias isn't on, because we need to the model for transmission
				if(LFBias()){
				#endif
					LFBiasModelIndex = (LFBiasModelIndex+1) & 3; // construct index into LFBias model, which is four 1KHz samples long
					LFBiasModel[M][LA][LFBiasModelIndex]=Lead[LA];
					LFBiasModel[M][LL][LFBiasModelIndex]=Lead[LL];
					LFBiasModel[M][V1][LFBiasModelIndex]=Lead[V1];
					LFBiasModel[M][V2][LFBiasModelIndex]=Lead[V2];
					LFBiasModel[M][V3][LFBiasModelIndex]=Lead[V3];
					LFBiasModel[M][V4][LFBiasModelIndex]=Lead[V4];
					LFBiasModel[M][V5][LFBiasModelIndex]=Lead[V5];
					LFBiasModel[M][V6][LFBiasModelIndex]=Lead[V6];
					if(LFBiasModelIndex&1){
						LFBiasTog();
					}
					if(LFBiasModelIndex==3){ // we just filled the model, so
						M^=1;	// switch models (ping-pong)
						AbandonTimer--;	// run down the pre-acquisition abandonment timer
						LFReady=1;			// tell the AcqTask the model is ready for analysis
						BTReady=1;			// tell BTTask that the model is ready for decimation
					}
				#if BLUETOOTH != 1
				}
				else{
					LFBiasModelIndex=0;
				}
				#endif

				// an ECG sample is ready, figure out where to put it
				if(AcqBufferLock == UNLOCKED){	// store samples when the task is ready

					if(index==0){	// we're beginning a new memory page, find a free buffer to put it in
						if(Buf1.Status == BUF_EMPTY){
							Buf1.Status = BUF_FILLING;
						}
						else if(Buf2.Status == BUF_EMPTY){
							Buf2.Status = BUF_FILLING;
						}
						else{ // no buffer available, that's a problem
							SystemError.BUF_OVERRUN=1;
							while(1){
								DPLEDTog();
							}
						}
					}
					if(index<ECG_SAMPLES_PER_PAGE){ // we're currently filling a memory page, figure out which one
						if(Buf1.Status == BUF_FILLING){
							PackECG(&Buf1.Data.ECG.Sample[index][0],(unsigned char *)Lead);
						}
						else{
							PackECG(&Buf2.Data.ECG.Sample[index][0],(unsigned char *)Lead);
						}
						index++;
					}
					if(index>=ECG_SAMPLES_PER_PAGE){ // index == ECG_SAMPLES_PER_PAGE, we've finished this page of memory
						if(Buf1.Status == BUF_FILLING){
							Buf1.Status = BUF_FULL;
						}
						else{
							Buf2.Status = BUF_FULL;
						}
						index=0;	// next interrupt will start a new buffer
					}
				}
				else{ //AcqBufferLock == UNLOCKED
					index=0;
				}
			}

			//------------------------------------------------------------------------------------------
			// Accumulate sub-samples to build up the next sample set
			else if(AcqIrqState>=ACQ_IRQ_ECG){
				Acc[V3] += ADCA.CH0.RES;	// Accumulate channel A measurements
				Acc[V4] += ADCA.CH1.RES;
				Acc[V5] += ADCA.CH2.RES;
				Acc[V6] += ADCA.CH3.RES;
				Acc[LA] += ADCB.CH0.RES;	// Accumulate channel B measurements
				Acc[LL] += ADCB.CH1.RES;
				Acc[V1] += ADCB.CH2.RES;
				Acc[V2] += ADCB.CH3.RES;

				AcqIrqState++;	// advance through accumulation on our way to the decimation state
			}
		}
		// Start both converters on all channels for the next Lead/Voice sampling interrupt
		ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm |ADC_CH1START_bm | ADC_CH2START_bm | ADC_CH3START_bm) );
		ADC_Conversions_Start(&ADCB, (ADC_CH0START_bm |ADC_CH1START_bm | ADC_CH2START_bm | ADC_CH3START_bm) );

	//--------------------------------------------------------------------------------------------------------------
	//--------------------------------------------DEBUGGING STUFF---------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------
#endif 
	//--------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------

	}// if(mode)
	else{ // we're idle
		index = 0;
	}
}

//############################################################################################
//############################################################################################
// Handle the acquisition of Voice and ECG by watching the button

// TaskMain will enable us if we were powered up by a button press and we are empty

// When the voice buffer is empty (which means the ECG buffer is also empty)
	// Short press = display status E
	// Long press  = begin recording voice and continue until the button released, or recording limit reached

// When the voice buffer is full (but the ECG buffer is empty) We will be in ECG Pre-acquisition, unless we time-out (then we erase everything)
	// Short press = start recording ECG if QC is OK, ignore otherwise.
	// Long press = abort QC, erase all flash, show busy until button released and flash erased, then show status E

// When recording ECG, display current QC
	// Short press or 300 second timeout = stop recording ECG, show status F for 10-15 seconds, then turn off
	// Long press = abort acquisition, erase all flash, show busy until button released and flash erased, then show status E


BEGIN_TASK(TaskAcq)

	static unsigned int ButtonTimer;
	static unsigned int ShutoffTimer;
	static unsigned char i,j;

	#define SHUTOFF_TIME 5000	// milliseconds
	#define LONG_BUTTON_TIME 1000 // ms
	#define SHORT_BUTTON_TIME 30 // ms

	MainPowerOn();		// keep ourselves on
	GroundUnused();		// ground unused analog inputs

	AcqBufferLock = LOCKED;
	StartAcq(ACQ_IRQ_IDLE); // shut down any acquisition that's already running
	Flags.AcqActive = 1;

	while(1){	// do something until we complete or abort the acquisition process any decision to erase and restart will bring us back here

			//--------------------------------------------------------------------------------------------------------------
			//--------------------------------------------DEBUGGING STUFF---------------------------------------------------
			//--------------------------------------------------------------------------------------------------------------

				// Start throwing logs out to the FLASH_ECG page
#ifdef LOGGING_TO_FLASH
				Buf1.Status = BUF_EMPTY; // make the buffers ready
				Buf2.Status = BUF_EMPTY;
				FlashPageNumber = FLASH_ECG_START_PAGE;
				StartAcq(ACQ_IRQ_LOGGING);
#endif

			//--------------------------------------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------------------------------------

			LogMessage("This is a debugging message! We are starting the acquisition process!\n");

		AnalogPowerOn();
		// at the top of this loop, we should be empty. if we're not, we erase flash spinning the display
		// if the button is still pressed after erasing, we wait for it to be released


		// verify that the battery voltage is high enough to attempt acquisition
		InitADCs(); // start the ADCs and then we'll override a few settings
		ADC_Reference_Config(&ADCA, ADC_REFSEL_INT1V_gc); 		// temporarily use internal bandgap
		ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV256_gc);	// 128KSPS @ 32MHZ
		ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_BATT, 0);		// monitor battery voltage on ADCA
		ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_BATT, 0);		// monitor battery voltage on ADCA
		ADC_Ch_InputMux_Config(&ADCA.CH2, ADC_CH_UNUSED_0, 0);// monitor ground
		ADC_Ch_InputMux_Config(&ADCA.CH3, ADC_CH_UNUSED_0, 0);// monitor ground
		osWAIT(50);	// wait for power to stabilize
		ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm | ADC_CH1START_bm| ADC_CH2START_bm| ADC_CH3START_bm) );
		osWAIT(1); // wait for conversions to complete
		ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm | ADC_CH1START_bm| ADC_CH2START_bm| ADC_CH3START_bm) );
		osWAIT(1); // wait for conversions to complete
		VBatt = (ADCA.CH0.RES + ADCA.CH1.RES - ADCA.CH2.RES - ADCA.CH3.RES)/(8190.0-ADCA.CH2.RES - ADCA.CH3.RES)*4.2;


		if(EE_Rd(EE_VOICE_RECORD_LENGTH)!=0){
			AcqWatchTimer = FLASH_WATCH_TIME;
			while(TaskFlashCmd != TF_IDLE);	// wait for flash to become ready
			TaskFlashCmd = TF_ERASE_VOICE;
			do{
				osWAIT(100);
				Seg7HP(0x22);
				osWAIT(100);
				Seg7HP(0x27);
				osWAIT(100);
				Seg7HP(0x26);
				osWAIT(100);
				Seg7HP(0x21);
			}while(TaskFlashCmd != TF_IDLE);	// wait for erase to complete
			Seg7HP(' ');	// blank and release the LED
		}

		if(EE_Rd(EE_ECG_RECORD_LENGTH)!=0){
			AcqWatchTimer = FLASH_WATCH_TIME;
			while(TaskFlashCmd != TF_IDLE);	// wait for flash to become ready
			TaskFlashCmd = TF_ERASE_ECG;
			do{
				osWAIT(100);
				Seg7HP(0x23);
				osWAIT(100);
				Seg7HP(0x24);
				osWAIT(100);
				Seg7HP(0x25);
				osWAIT(100);
				Seg7HP(0x27);
			}while(TaskFlashCmd != TF_IDLE);	// wait for erase to complete
			Seg7HP(' ');	// blank and release the LED
		}

		Seg7LP('E');	// tell the user we're empty

		if(VBatt < EE_Rd(EE_LB)/1000.0){
			Seg7LP('L');	// display low battery warning
			MainPowerOff();
			while(ButtonPressed()){ // if the button is pressed, hang here
				osWAIT(1);
			}
		}

		while(ButtonPressed()){
			osWAIT(1);	// wait for release of the button
		}
		osWAIT(30);	// wait for button to stop bouncing

		LFBiasOff();

#ifndef LOGGING_TO_FLASH
		StartAcq(ACQ_IRQ_VOICE); // tell the acquisition interrupt to do voice, though we won't start buffering just yet
		Buf1.Status = BUF_EMPTY; // make the buffers ready
		Buf2.Status = BUF_EMPTY;
#endif

		// wait for another button press, shut off if it never comes
		ShutoffTimer = 0;
		while(!ButtonPressed()){
			osWAIT(1);
			if(ShutoffTimer++ >= SHUTOFF_TIME){
				MainPowerOff();
			}
		}

		// The button is pressed again, see how long
		// If the button is pressed for more than VOICE_RECORD_BUTTON_TIME, start recording voice
		// else shut off
		ButtonTimer=0;
		while(ButtonPressed()){
			osWAIT(1);
			if(ButtonTimer++>=LONG_BUTTON_TIME){
				break; // continue, let code below figure out how long the button was pressed
			}
		}
		if(ButtonTimer<LONG_BUTTON_TIME){	// if we were showing 'E' and saw a short press, turn off
			while(1){
				MainPowerOff();
				osWAIT(1);
			}
		}

		// COMMENTED OUT FOR LOGGING
		//FlashPageNumber = FLASH_VOICE_START_PAGE;	// start from the beginning
		// COMMENTED OUT FOR LOGGING

		Seg7LP('r');	// we're recording
		LogMessage("We're recording voice data now\n");
		EnableBuffering();
		while(FlashPageNumber<=FLASH_VOICE_END_PAGE && ButtonPressed() ){ // run while there's still room in flash and the button is pressed
			osWAIT(1);

#ifndef LOGGING_TO_FLASH
			if(Buf1.Status == BUF_FULL){ // wait for Buf1 to fill, which takes 16ms
				TaskFlashCmd = TF_WRITE_BUF1;
				while(TaskFlashCmd != TF_IDLE){	// write Buf1 while the interrupt routine fills Buf2
					osYIELD;
				}
				Buf1.Status = BUF_EMPTY; // return the buffer to the interrupt handler
				FlashPageNumber++;
				if(FlashPageNumber>FLASH_VOICE_END_PAGE){
					break;
				}
			}
			else if(Buf2.Status == BUF_FULL){ // wait for Buf2 to fill, which takes 16ms
				TaskFlashCmd = TF_WRITE_BUF2;
				while(TaskFlashCmd != TF_IDLE){	// write Buf2 while the interrupt routine fills Buf1
					osYIELD;
				}
				Buf2.Status = BUF_EMPTY;
				FlashPageNumber++;
				if(FlashPageNumber>FLASH_VOICE_END_PAGE){
					break;
				}
			}
#endif
		}
		EE_Wr(EE_VOICE_RECORD_LENGTH,FlashPageNumber-FLASH_VOICE_START_PAGE-1);	// remember how much voice we recorded

		//------------------------------------------------------------------------------------------
		// Prepare for Lead QC

		// make fast copies of the LFBias thresholds
		B1=EE_Rd(EE_B1);
		B2=EE_Rd(EE_B2);
		B3=EE_Rd(EE_B3);
		LFMaxFilt=B3;
		LFMinFilt=B1;

		// prepare for LFBiasModel generation by clearing the models and setting the subsample index
		LFBiasModelIndex=0;
		M=0;
		for(i=0;i<=15;i++){ // flush both models
			for(j=0;j<=3;j++){
				LFBiasModel[0][i][j]=0;
			}
		}

		LFBiasOn();
		AbandonTimer = EE_Rd(EE_ABANDON_TIME)*250L;	// set the abandonment timer
		LFReady=0;

#ifndef LOGGING_TO_FLASH
		StartAcq(ACQ_IRQ_ECG);	// switch over to ECG pre-acquisition (buffers will be locked, so nothing is stored)
#endif

		// Flash
		while(FlashPageNumber>FLASH_VOICE_END_PAGE && ButtonPressed()){
			Seg7LP(' ');
			osWAIT(200);
			Seg7LP('r');
			osWAIT(200);
		}

		// start pre-acquisition here so lead QC can get going, but don't start buffering until the button is pressed

		if(LFReady){
			LFReady=0;
		}

		LogMessage("Now we are at preacquisition\n");
		BTBufPhase = 0;
        BTStatus = 0;               // Added by PRV
		BTEnable = 1;				// allow Bluetooth to send data

		while(!ButtonPressed()){	// pre-acquire while the button is not pressed
			osYIELD;
			if(LFReady){	// this will happen every four milliseconds
				LFReady=0;
				LFMag=0;
				
				// we now have a lead fail bias model to examine for the amplitude of 250Hz signal
				// we sort the four samples to determine the difference from the smallest to the largest, which is an approximation of the 250Hz amplitude
				for(unsigned char i=0;i<8;i++){
					LFLo = LFBiasModel[M][i][0];
					LFHi = LFBiasModel[M][i][0];
					if(LFBiasModel[M][i][1]>LFHi)LFHi=LFBiasModel[M][i][1];
					if(LFBiasModel[M][i][1]<LFLo)LFLo=LFBiasModel[M][i][1];
					if(LFBiasModel[M][i][2]>LFHi)LFHi=LFBiasModel[M][i][2];
					if(LFBiasModel[M][i][2]<LFLo)LFLo=LFBiasModel[M][i][2];
					if(LFBiasModel[M][i][3]>LFHi)LFHi=LFBiasModel[M][i][3];
					if(LFBiasModel[M][i][3]<LFLo)LFLo=LFBiasModel[M][i][3];
					LFMag=(LFHi-LFLo)>>4;	// this is the magnitude of the reflected signal for this lead

					// now we look for the maximum and minimum signal magnitudes
					if(i==0){ // prime the min/max
						LFMax = LFMin = LFMag;
					}
					if(LFMag > LFMax)LFMax=LFMag;
					if(LFMag < LFMin)LFMin=LFMag;
				}
				if(LFMax>=LFMaxFilt){
					LFMaxFilt++;
				}
				else{
					LFMaxFilt--;
				}
				if(LFMin>=LFMinFilt){
					LFMinFilt++;
				}
				else{
					LFMinFilt--;
				}
				if(LFMinFilt<B1)QC=1;
				else if(LFMaxFilt<B3)QC=3;
				else if(LFMaxFilt<B2)QC=2;
				else QC=1;
				Seg7LP(QC+0x29);

			}

			if(AbandonTimer<0){
				MainPowerOff();
				while(1){
					osWAIT(1);
				}
			}
		}

		// If we get a short button press, start recording ECG, if we get a long one, empty the voice buffer and start over at 'E'
		// continue recording until the button is pressed again or we use up all the recording time

		ButtonTimer = 0; // start timing the button press
		while(ButtonPressed()){
			osWAIT(1);
			ButtonTimer++;
			if(ButtonTimer>=LONG_BUTTON_TIME){
				break;
			}
		}
		if(ButtonTimer < LONG_BUTTON_TIME)
            {	// start acquisition on a short button press
			osWAIT(30); //make sure button is released so we don't catch a bounce
			LFBiasOff();	// disable lead fail bias for acquisition, so we don't interfere with other instruments (defib?)
			Seg7LP('A');

#ifndef LOGGING_TO_FLASH
			FlashPageNumber = FLASH_ECG_START_PAGE;
			Buf1.Status = BUF_EMPTY;
			Buf2.Status = BUF_EMPTY;
			EnableBuffering();
#endif

			// BTBufPhase = 0;
            BTStatus |= 4;
			while(FlashPageNumber<=FLASH_ECG_END_PAGE && ButtonPressed()==0 )
                { // run while there's still room in flash and button is not pressed
				if(Buf1.Status == BUF_FULL){ // wait for Buf1 to fill, which takes 21ms
                /*
					TaskFlashCmd = TF_WRITE_BUF1;
					while(TaskFlashCmd != TF_IDLE){	// write Buf1 while the interrupt routine fills Buf2
						osYIELD;
					}
                */
					Buf1.Status = BUF_EMPTY; // return the buffer to the interrupt handler
					FlashPageNumber++;
					if(FlashPageNumber>FLASH_ECG_END_PAGE){
						StopAcq();
						break;
					}
				}
				if(Buf2.Status == BUF_FULL){ // wait for Buf1 to fill, which takes 21ms
                /*
					TaskFlashCmd = TF_WRITE_BUF2;
					while(TaskFlashCmd != TF_IDLE){	// write Buf1 while the interrupt routine fills Buf2
						osYIELD;
					}
                */
					Buf2.Status = BUF_EMPTY; // return the buffer to the interrupt handler
					FlashPageNumber++;
					if(FlashPageNumber>FLASH_ECG_END_PAGE){
						StopAcq();
						break;
					}
				}
                osYIELD;
			    }
            
            BTStatus &= ~4;
            osWAIT(250);
			// we ran to the end of the ECG buffer, or the button was pressed
			StopAcq();
#if BLUETOOTH
			BTOff();
#endif
			EE_Wr(EE_ECG_RECORD_LENGTH,FlashPageNumber-FLASH_ECG_START_PAGE-1);	// remember how much ECG we recorded

			// determine if the button is being pressed, and for how long
			// a short press shows 'F' and turns us off, a long one erases flash and sends us back to 'E'
			ButtonTimer=0;
			while(ButtonPressed()){
				osWAIT(1);
				if(ButtonTimer++>LONG_BUTTON_TIME){
					break; // this will cause us to rerun the outer while(1)
				}
			}

            //======= BREAKS CODE BELOW FORCING CANCELATION OF RECORDING! =========================
            //break;      // REMOVE BEFORE FLIGHT! Temporary! Disables F functionality.
            //=====================================================================================

			if(ButtonTimer<LONG_BUTTON_TIME){	// a short or no press means stop and show 'F'
				Seg7LP('F');
				ShutoffTimer = SHUTOFF_TIME;
				while(ShutoffTimer-- && (ButtonPressed() == 0)){
					osWAIT(1);
				}
				ElapsedMinutes = 1;	// Start the elapsed time timer. This tells TaskMain to enter low power mode
				osDISABLE_TASK(TaskAcq); // We're done!
				Flags.AcqActive = 0;
				AnalogPowerOff();
				FlashPowerOff();
				while(1){
					osWAIT(1);
				}
			}
		} // end short button press for acquisition, we only get here if acquisition ended with a long button press
	} // while(1) back to 'E'

END_TASK
