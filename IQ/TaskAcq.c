#ifndef BOOTLOADER

#include <avr/io.h>
#include "taskUSB.h"
#include "taskBT.h"
#include "rtos.h"
#include "iq.h"
#include "adc_driver.h"
#include "taskAcq.h"
#include "TC_driver.h"
#include "buffers.h"
#include "ulaw.h"
#include "seg7.h"
#include "eeprom.h"
#include "eeprom_driver.h"
#include "TaskWatchDog.h"
#include "taskCC.h"

#include <math.h>

extern unsigned char AcqIrqState;	// a place to remember what the acquistion interrupt is doing

unsigned char acqState = FSM_START;

#ifdef BLUETOOTH
 char btSpikeIndex = 0xFF;
 char btSpikeIndex2 = 0xFF;
 char btSpikeIndex3 = 0xFF;

 char pacerSpikes[3];

 unsigned char samplesSinceLastPacket = 0;

#endif
unsigned int acqTimer = 0;
unsigned char acqMinutes = 0;
long long leadFailTimer = 0;

unsigned char AcqIrqState = ACQ_IRQ_IDLE;	// used to hold the current acquisition interrupt state, a task may control acquisition via this "semaphore"
enum {LOCKED, UNLOCKED};
unsigned char AcqBufferLock = LOCKED;

#define EnableBuffering() AcqBufferLock = UNLOCKED;
#define DisableBuffering() AcqBufferLock = LOCKED;

int lastCheckState;

//uint32_t
//uint16_t
uint16_t Acc[8];	// sub-sample lead accumulators
int16_t Lead[8];	// final ECG leads

unsigned int sampleIndex; // used to index into the ECG buffer
unsigned int voiceIndex;	// used to index into the Voice buffer

float VBatt;

// Lead Fail Stuff
char LFBiasEnable;	// 1 = inject 250Hz bias, 0 = don't
int16_t LFBiasModel[2][8][4];	// two models (so we can ping-pong) for the eight channels, each model containing four samples
unsigned char M;	// selects which of the two LFBiasModels is ready for examination, 0 or 1
unsigned char LFBiasModelIndex; // used to index into the four subsamples of each LFBias model
int LFLo, LFHi;	// holds extents of LF swing for each channel
unsigned int LFMag, LFMax, LFMin;//, LFMaxFilt, LFMinFilt; // holds overall LF swing magnitude, the final indication of lead impedance

unsigned int LFMaxFilt[8];
unsigned int LFMinFilt[8];
unsigned char badLeads[8];

char QC;	// the final QC determination, used to drive 1/2/3 bars on the LED
unsigned int B1, B2, B3; // Thresholds (obtained from EEPROM) for 2/3 bars due to lead impedance

long AbandonTimer; // used to timeout pre-acquisition if the unit is abandoned
unsigned char LFReady; // used by the acquisition interrupt to tell the LF checker that data is ready

#ifdef BLUETOOTH
unsigned char BTEnable;	// 1 enables bluetooth transmission
unsigned char BTReady;	// 1 = a leadfail bias model is ready for decimation, 0 = not ready
unsigned char BTBufPhase; // 0 = decimation of LFBiasModel[0] loads the BTBuf, 1 = decimation of LFBiasModel[1] adds to BTBuf, after which /8 produces an average of both models
unsigned int BTBuf[8];	// holds all eight channels of data, decimated from 1KHz to 125Hz.
// unsigned char BTString[29]; // room to store the Bluetooth sample output string.
unsigned char BTStatus;
unsigned char haveFinalPacket;
#endif

unsigned int enableVoice = 0;

#ifdef BLUETOOTH
unsigned char erasedWhileFull = 0;
unsigned char waitingForLow = 0;
unsigned char paceSpike = 0;
volatile uint8_t pacerStatus = 0;
#ifdef WORKING
unsigned int msSinceLastSpike = 0;
#endif
#endif

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

	LFBiasEnable = 1;//EE_Rd(EE_LFBIAS_ENABLE);
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

//------------------------------------------------------------------------------------------
// this function is called by the SysTick interrupt
void AcqInterrupt(){
	//return; 
	
	//static int index;	// keeps track of where we are in buffer stuffing
	//int VoiceSample;
	//static int VoiceBaseline;	// centers VoiceSample around zero for good uLaw compression

	if(AcqIrqState != ACQ_IRQ_IDLE){ // only do something if there is something to do
	// The previous interrupt will have started conversions on the eight ECG leads or the microphone input
	// That'll be done by now, so we can drive straight into getting the conversion values
	// For ECG, during decimation (mode == DECIMATION), we transfer the decimated ECG/monitoring values into the output buffers
	// most times (mode >= ACCUMULATE and < ACQ_DECIMATE) we just accumulate new ECG measurements into the decimation accumulators
	// During voice acquisition, we transfer the voice sample into the output buffer

		if(ADC_Ch_Conversion_Complete(&ADCB.CH3)){	// the ADCs should be ready, no harm in checking

			//------------------------------------------------------------------------------------------
			// process a Voice sample, if the buffers are locked, we can still keep the baseline filter running
			if(AcqIrqState==ACQ_IRQ_ECG_DECIMATE){
				//#define UPPER_THRESH 62000
				//#define LOWER_THRESH 3200
				
				#define UPPER_THRESH 65534
				#define LOWER_THRESH 3200
				
				//#define UPPER_THRESH 100000
				//#define LOWER_THRESH -1
				//#define UPPER_THRESH 48000
				//#define LOWER_THRESH 6500
#ifndef BLUETOOTH
				if(LFBias()) {
#endif
					LFBiasModelIndex = (LFBiasModelIndex+1) & 3;
#ifndef BLUETOOTH 
				}
#endif
				
				// Check for a pacer spike
#ifdef BLUETOOTH
				pacerStatus = ((PORT_PACE_DETECT.IN & PACE_DETECT_L1) >> PIN3_bp);
				if((pacerStatus) && !waitingForLow) {
					//if(BTEnable && btSpikeIndex == 0xFF) {
					//	btSpikeIndex =  (M*4)+((LFBiasModelIndex));
					//}						
					//else if(BTEnable && btSpikeIndex2 == 0xFF) {
					//	btSpikeIndex2 = (M*4)+((LFBiasModelIndex));
					//}
					if(BTEnable && pacerSpikes[0] == 0xFF) {
						pacerSpikes[0] = samplesSinceLastPacket; //(M*4)+((LFBiasModelIndex));
					}						
					else if(BTEnable && pacerSpikes[1] == 0xFF) {
						pacerSpikes[1] = samplesSinceLastPacket; //(M*4)+((LFBiasModelIndex));
					}
					else if(BTEnable && pacerSpikes[2] == 0xFF) {
						pacerSpikes[2] = samplesSinceLastPacket; //(M*4)+((LFBiasModelIndex));
					}

					//if(BTEnable) {
					//	BTStatus |= 8;
						//print_bt("Spike detected! Last spike %d ms ago\r\n", msSinceLastSpike);
					//}						 
					waitingForLow = 1;
#ifdef WORKING
					msSinceLastSpike = 0;
#endif
				}
				else if(waitingForLow && (!pacerStatus)) {
					waitingForLow = 0;
				}
#endif

#ifdef BLUETOOTH
				samplesSinceLastPacket++;
#endif

				// rezero any lead that goes out of bounds
				if(Acc[V3]>UPPER_THRESH) {
					Acc[V3] = 65535;
					if (legacy_charger) {						
						PORT_V3_V6.OUTCLR = REZERO_V3;
						PORT_V3_V6.DIRSET = REZERO_V3;
					} else {
						PORT_V3_V4.OUTCLR = REZERO_V3;
						PORT_V3_V4.DIRSET = REZERO_V3;
					}
				}
				else if(Acc[V3] < LOWER_THRESH) {
					Acc[V3]=0;
					if (legacy_charger) {
						PORT_V3_V6.OUTSET = REZERO_V3;
						PORT_V3_V6.DIRSET = REZERO_V3;
					} else {
						PORT_V3_V4.OUTSET = REZERO_V3;
						PORT_V3_V4.DIRSET = REZERO_V3;
					}
				}
				else {
					if (legacy_charger) {
						PORT_V3_V6.DIRCLR = REZERO_V3;
					} else {
						PORT_V3_V4.DIRCLR = REZERO_V3;
					}
				}
				
				// convert the unsigned values to signed and transfer to the Leads
				Lead[V3] = Acc[V3]-0x8000;
				//------------------------------------

				if(Acc[V4]>UPPER_THRESH){
					Acc[V4] = 65535;
					if (legacy_charger) {
						PORT_V3_V6.OUTCLR = REZERO_V4;
						PORT_V3_V6.DIRSET = REZERO_V4;
					} else {
						PORT_V3_V4.OUTCLR = REZERO_V4;
						PORT_V3_V4.DIRSET = REZERO_V4;
					}
				}
				else if(Acc[V4] < LOWER_THRESH){
					Acc[V4]=0;
					if (legacy_charger) {
						PORT_V3_V6.OUTSET = REZERO_V4;
						PORT_V3_V6.DIRSET = REZERO_V4;
					} else {
						PORT_V3_V4.OUTSET = REZERO_V4;
						PORT_V3_V4.DIRSET = REZERO_V4;
					}
				}
				else {
					if (legacy_charger) {
						PORT_V3_V6.DIRCLR = REZERO_V4;
					} else {
						PORT_V3_V4.DIRCLR = REZERO_V4;
					}
				}
				
				Lead[V4] = Acc[V4]-0x8000;
				//------------------------------------

				if(Acc[V5]>UPPER_THRESH){
					Acc[V4] = 65535;
					if (legacy_charger) {
						PORT_V3_V6.OUTCLR = REZERO_V5;
						PORT_V3_V6.DIRSET = REZERO_V5;
					} else {
						PORT_V5.OUTCLR = REZERO_V5;
						PORT_V5.DIRSET = REZERO_V5;
					}
				}
				else if(Acc[V5] < LOWER_THRESH){
					Acc[V5]=0;
					if (legacy_charger) {
						PORT_V3_V6.OUTSET = REZERO_V5;
						PORT_V3_V6.DIRSET = REZERO_V5;
					} else {
						PORT_V5.OUTSET = REZERO_V5;
						PORT_V5.DIRSET = REZERO_V5;
					}
				}
				else {
					if (legacy_charger) {
						PORT_V3_V6.DIRCLR = REZERO_V5;
					} else {
						PORT_V5.DIRCLR = REZERO_V5;
					}
				}
				
				Lead[V5] = Acc[V5]-0x8000;
				//------------------------------------

				if(Acc[V6]>UPPER_THRESH){
					Acc[V6] = 65535;
					if (legacy_charger) {
						PORT_V3_V6.OUTCLR = REZERO_V6;
						PORT_V3_V6.DIRSET = REZERO_V6;
					} else {
						PORT_V6.OUTCLR = REZERO_V6_2200;
						PORT_V6.DIRSET = REZERO_V6_2200;
					}
				}
				else if(Acc[V6] < LOWER_THRESH){
					Acc[V6]=0;
					if (legacy_charger) {
						PORT_V3_V6.OUTSET = REZERO_V6;
						PORT_V3_V6.DIRSET = REZERO_V6;
					} else {
						PORT_V6.OUTSET = REZERO_V6_2200;
						PORT_V6.DIRSET = REZERO_V6_2200;
					}
				}
				else {
					if (legacy_charger) {
						PORT_V3_V6.DIRCLR = REZERO_V6;
					} else {
						PORT_V6.DIRCLR = REZERO_V6_2200;
					}
				}
				
				Lead[V6] = Acc[V6]-0x8000;
				//------------------------------------

				// reload the accumulators with fresh conversions
				Acc[V3] = ADCA.CH0.RES;
				Acc[V4] = ADCA.CH1.RES;
				Acc[V5] = ADCA.CH2.RES;
				Acc[V6] = ADCA.CH3.RES;

				// now that we've got the ADCA lead values, set ADCA up for RL/VBatt monitoring, which is not decimated
				//ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_RL_MON, 0);
				//ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_BATT, 0);
				//ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm |ADC_CH1START_bm));

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
				
				//print_usb("%d\r\n", Lead[V1]);

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
				//ADC_Ch_InputMux_Config(&ADCB.CH0, ADC_CH_RA_MON, 0);
				//ADC_Conversions_Start(&ADCB, ADC_CH0START_bm);

				//while(!ADC_Ch_Conversion_Complete(&ADCA.CH1));	// ADCA conversions should be done by now, but wait just in case
				//RL_Mon = ADCA.CH0.RES;
				//VBatt  = ADCA.CH1.RES;

#ifdef BLUETOOTH
				/*
				int voltage = ((RL_Mon-zeroVolts-delta)/(4095.0f-delta))*3.3f*1000;

				if((voltage >= 750 || voltage <= -750) && !BTLeadOff) {
					if(leadFailTimer != 500) {
						leadFailTimer++;
					}
					else if(!BTLeadOff) {
						//print_usb("Lead Off Condition!");
						BTLeadOff = 1;
						leadFailTimer = 0;
					}
				} else if((voltage < 750 && voltage >-750 && BTLeadOff)) {
					if(leadFailTimer != 500) {
						leadFailTimer++;
					} else {
						leadFailTimer = 0;
						BTLeadOff = 0;
					}
					//print_usb("Lead back on!");
				}
				*/
#endif

				//while(!ADC_Ch_Conversion_Complete(&ADCB.CH0));	// ADCB probably isn't done yet, wait for it
				//RA_Mon = ADCB.CH0.RES;

				// now we can restore the input multiplexing to the ECG leads (!!!!! should we reload all the multiplexers?)
				//ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_V3, 0); // restore mux, we just used this channel to measure RL_MON
				//ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_V4, 0); // restore mux, we just used this channel to measure BATT
				//ADC_Ch_InputMux_Config(&ADCB.CH0, ADC_CH_LA, 0); // restore mux, we just used this channel to measure RA_MON
				
				/*
						ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_V3, 0);		// set up for ECG measurements across PortA
						ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_V4, 0);
						ADC_Ch_InputMux_Config(&ADCA.CH2, ADC_CH_V5, 0);
						ADC_Ch_InputMux_Config(&ADCA.CH3, ADC_CH_V6, 0);
						ADC_Ch_InputMux_Config(&ADCB.CH0, ADC_CH_LA, 0);		// set up for ECG measurements across PortB
						ADC_Ch_InputMux_Config(&ADCB.CH1, ADC_CH_LL, 0);
						ADC_Ch_InputMux_Config(&ADCB.CH2, ADC_CH_V1, 0);
						ADC_Ch_InputMux_Config(&ADCB.CH3, ADC_CH_V2, 0);
				*/

#ifdef WORKING
				//if(capturePosition != FFT_N) {
				//	capture[capturePosition++] = Lead[currentFFTLead];
					//capture[1][capturePosition++] = Lead[currentFFTLead+1];
					
					//bfly_buff[0][capturePosition].i = Lead[currentFFTLead];
					//bfly_buff[0][capturePosition++].r = Lead[currentFFTLead];
				//}
#endif
				
						
				AcqIrqState = ACQ_IRQ_ECG; // back to accumulation
				

				// if LFBias is on, we build the bias signal models
				// LFBias is synchronous to sampling at 1/4 the rate. So there are four samples per cycle of the LFBias generator.
				#if BLUETOOTH != 1 // in bluetooth builds, we build the lf model even if lfbias isn't on, because we need to the model for transmission
				if(LFBias()){
				#endif
					//LFBiasModelIndex = (LFBiasModelIndex+1) & 3; // construct index into LFBias model, which is four 1KHz samples long
#ifdef WORKING
					msSinceLastSpike++;
#endif
					LFBiasModel[M][LA][LFBiasModelIndex]=Lead[LA];
					LFBiasModel[M][LL][LFBiasModelIndex]=Lead[LL];
					LFBiasModel[M][V1][LFBiasModelIndex]=Lead[V1];
					//print_usb("%d\r\n", Lead[LA]-Lead[LL]);
					
					LFBiasModel[M][V2][LFBiasModelIndex]=Lead[V2];
					LFBiasModel[M][V3][LFBiasModelIndex]=Lead[V3];
					LFBiasModel[M][V4][LFBiasModelIndex]=Lead[V4]; 
					LFBiasModel[M][V5][LFBiasModelIndex]=Lead[V5];
					LFBiasModel[M][V6][LFBiasModelIndex]=Lead[V6];
					
					
					//if(LFBiasEnable) {
						if(LFBiasModelIndex&1){ 
							LFBiasTog();
							//DPLEDTog();
							// Manually toggle. See what happens.
							//PORTC.OUTSET = PORTC.OUT ^ PIN4_bm;
							//PORTC.OUT ^= PIN4_bm;
						}
					//}
										
					
					if(LFBiasModelIndex==3){ // we just filled the model, so
						M ^= 1; //pingpong circulate
						
						if(!continuousMode) {
							AbandonTimer--;	// run down the pre-acquisition abandonment timer
						}						
						LFReady=1;			// tell the AcqTask the model is ready for analysis
#ifdef BLUETOOTH
						if(BTEnable) BTReady=1;		 	// tell BTTask that the model is ready for decimation
#endif
					} 
				#if BLUETOOTH != 1
				}
				else{
					LFBiasModelIndex=0;
				}
				#endif

				// an ECG sample is ready, figure out where to put it

				/*
				if(AcqBufferLock == UNLOCKED 
#ifdef BLUETOOTH
					&& (BTStatus & 4)
#endif
					){	// store samples when the task is ready

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
						//if(haveUsbSpike)
						//	print_bt("We have a USB pacer spike on sample %d out of 21\n", Buf1.Data.ECG.Flag1>>1);
						
						if(Buf1.Status == BUF_FILLING){
							// Pack in the spike index
							Buf1.Status = BUF_FULL;
						}
						else{
							// Pack in the spike index
							Buf2.Status = BUF_FULL;
						}
						index=0;	// next interrupt will start a new buffer
					}
				}
				else{ //AcqBufferLock == UNLOCKED
					index=0;
				}
				*/
			}

			//------------------------------------------------------------------------------------------
			// Accumulate sub-samples to build up the next sample set
			else if(AcqIrqState>=ACQ_IRQ_ECG){
				//if(AcqIrqState <= ACQ_IRQ_ECG+8) {
				Acc[V3] += ADCA.CH0.RES;	// Accumulate channel A measurements
				Acc[V4] += ADCA.CH1.RES;
				Acc[V5] += ADCA.CH2.RES;
				Acc[V6] += ADCA.CH3.RES;
				Acc[LA] += ADCB.CH0.RES;	// Accumulate channel B measurements
				Acc[LL] += ADCB.CH1.RES;
				Acc[V1] += ADCB.CH2.RES;
				Acc[V2] += ADCB.CH3.RES;
			//}

				AcqIrqState++;	// advance through accumulation on our way to the decimation state
			}
		}
		// Start both converters on all channels for the next Lead/Voice sampling interrupt
		ADC_Conversions_Start(&ADCA, (ADC_CH0START_bm |ADC_CH1START_bm | ADC_CH2START_bm | ADC_CH3START_bm) );
		ADC_Conversions_Start(&ADCB, (ADC_CH0START_bm |ADC_CH1START_bm | ADC_CH2START_bm | ADC_CH3START_bm) );
	}// if(mode)
	//else{ // we're idle
	//	index = 0;
	//}
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
#ifdef WORKING
osENABLE_TASK(TaskUSB);
#endif

	static unsigned int ButtonTimer;
	//static unsigned int ShutoffTimer;
	static unsigned char i,j;
	static float vBattDelta;
	static int vBattPercent;
	//static unsigned char preAcqButtonReleased = 0;

	#define SHUTOFF_TIME 5000	// milliseconds
	
#ifndef BLUETOOTH
	#define LONG_BUTTON_TIME 5000 // ms
#else
	#define LONG_BUTTON_TIME 1000
#endif
	#define SHORT_BUTTON_TIME 30 // ms

	MainPowerOn();		// keep ourselves on
	//MainPowerOn();
	//FlashPowerOn();
	AnalogPowerOn();
	GroundUnused();		// ground unused analog input

	AcqBufferLock = LOCKED;
	Flags.AcqActive = 1;
	
#ifdef BLUETOOTH
	 pacerSpikes[0] = 0xFF;
	 pacerSpikes[1] = 0xFF;
	 pacerSpikes[2] = 0xFF;
#endif

#ifdef CONTINUOUS_BT
	GlobalWatchTimer = 0;
#endif

	//print_usb("In taskAcq");
while(1){	// do something until we complete or abort the acquisition process any decision to erase and restart will bring us back here
		
		StartAcq(ACQ_IRQ_IDLE); // shut down any acquisition that's already running
		
		//AnalogPowerOn();
		
		acqState = FSM_START;
		
		//enableVoice = 1;// EE_Rd(EE_ENABLE_VOICE);
		
		// In case we broke out during BT transmission
#ifdef BLUETOOTH
		if(BTEnable) {
			osDISABLE_TASK(TaskBT);
			BTOff();
			BTEnable = 0;
		}
#endif
		
		// at the top of this loop, we should be empty. if we're not, we erase flash spinning the display
		// if the button is still pressed after erasing, we wait for it to be released
		while(ButtonPressed()) {
			//MainPowerOff();
			osWAIT(1);
			if(ButtonTimer++ >= LONG_BUTTON_TIME)
				break;
		}
		osWAIT(50); // Wait for button bounce

		VBatt = cc_get_batt_volt();	
		InitADCs();	
		acqState = FSM_EMPTY;

		GlobalWatchTimer = 0;
		AcqWatchTimer = 0;

#ifdef WORKING
		ButtonTimer = LONG_BUTTON_TIME;
#endif

		if(ButtonTimer < LONG_BUTTON_TIME) {
			Seg7LP('0'+vBattPercent);

			AbandonTimer = 3000;
			while(AbandonTimer-- && !ButtonPressed() && !USBPresent()) {
				osWAIT(1);
			}
			osWAIT(30); // Wait for button bounce

			MainPowerOff();
			while(1) {
				osWAIT(1);
			}
		}

		Seg7LP('P');	// tell the user we're empty

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
		LFBiasOn();
		
		if(enableVoice) {
			StartAcq(ACQ_IRQ_VOICE); // tell the acquisition interrupt to do voice, though we won't start buffering just yet
		}

		/*
		Buf1.Status = BUF_EMPTY; // make the buffers ready
		Buf2.Status = BUF_EMPTY;
		*/

		// The button is pressed again, see how long
		// If the button is pressed for more than VOICE_RECORD_BUTTON_TIME, start recording voice
		// else shut off
		ButtonTimer=0;
			
		//------------------------------------------------------------------------------------------
		// Prepare for Lead QC

		// make fast copies of the LFBias thresholds
		B1=EE_Rd(EE_B1);
		B2=EE_Rd(EE_B2);
		B3=EE_Rd(EE_B3);
		for(i = 0; i < 8; i++) {
			LFMaxFilt[i]=B3;
			LFMinFilt[i]=B1;
		}			

		// prepare for LFBiasModel generation by clearing the models and setting the subsample index
		LFBiasModelIndex=0;
		M=0;
		for(i=0;i<=15;i++){ // flush both models
			for(j=0;j<=3;j++){
				LFBiasModel[0][i][j]=0;
			}
		}

		LFBiasOn();
		//AbandonTimer = EE_Rd(EE_ABANDON_TIME)*250L;	// set the abandonment timer
		LFReady=0;

		StartAcq(ACQ_IRQ_ECG);	// switch over to ECG pre-acquisition (buffers will be locked, so nothing is stored)

#ifndef BLUETOOTH
		while(ButtonPressed()) {
			osWAIT(1);
		}
#endif

		// start pre-acquisition here so lead QC can get going, but don't start buffering until the button is pressed

		if(LFReady){
			LFReady=0;
		}
		
		acqState = FSM_PREACQ;
		
#ifdef BLUETOOTH
		osENABLE_TASK(TaskBT); // Make sure BT the task is running

		BTBufPhase = 0;
       BTStatus = 0;               // Added by PRV

	   BTOn(); // if BT got killed, turn it back on
#endif
		//Seg7LP(0x29+1);

	   //TaskFlashCmd = TF_ERASE_ALL;	// erase everything
//static unsigned int toggleTimer = 0;
//static unsigned char currentToggleSetting = CONNECTION_ANDROID;

	   lastCheckState = BTEnable;
		//print_usb("Hitting main loop\r\n");
		do {	// pre-acquire while the button is not pressed	
			
			if(RTC.INTFLAGS & RTC_OVFIF_bm){	// if the RTC overflowed
				RTC.INTFLAGS = RTC_OVFIF_bm;		// clear the flag
				if(ElapsedMinutes<0xffff){
					ElapsedMinutes++;							// and increment ElapsedMinutes
				}
				// If we're past the configured timeout minutes, turn off
				if(ElapsedMinutes >= EE_Rd(EE_TIMEOUT_MINUTES) && (BTEnable == 0 || waitingForFirstCommand == 1)) {
					break;
				}
			}
			
			if(deviceNeedsToTurnOff)
				break;
			
			//Seg7HP('0' + ElapsedMinutes);
		//while(!ButtonPressed() && acqState != FSM_ACQURING) {
			//osWAIT(1);
			//osYIELD;
			if(keepAliveTimer != -1) {
				keepAliveTimer+=1;
				if(keepAliveTimer >= 5000) {
					//print_usb("Timeout from lack of keep alives.\r\n");
					keepAliveTimer = -1;
					connectionNeedsReset = 1;
					//resetAllVariables();
					//connectionType = CONNECTION_ANDROID;
					//Seg7HP('r');
				}
			}
			
			/*
			if(!BTEnable && readyForCommands) {
				toggleTimer++;
				if(toggleTimer >= 10000) {
					// Change what we're trying to call
					if(currentToggleSetting == CONNECTION_APPLE) {
						//print_usb("Toggling to Android\r\n");
						currentToggleSetting = CONNECTION_ANDROID;
						print_bt("SET CONTROL AUTOCALL 72243468-25ee-4025-8fe7-1edd2cf13cd2 2000 RFCOMM\r"); //72243468-25ee-4025-8fe7-1edd2cf13cd2
						ignoreNextRFCOMM = 1;
					} else {
						//print_usb("Toggling to Apple\r\n");
						currentToggleSetting = CONNECTION_APPLE;
						//print_bt("SET CONTROL AUTOCALL 00000000-deca-fade-deca-deafdecacaff 2000 IAP\r");
						//print_bt("SET CONTROL AUTOCALL fecacade-afde-cade-defa-cade00000000 2000 IAP\r");
						print_bt("SET CONTROL AUTOCALL 1101 2000 IAP\r");
					}
					
					toggleTimer = 0;
				}
			}
			*/
			
			
			// Wait 2 seconds for a command before we decide whether or not the connection is legit
			// THIS IS A HACK
			// FOR SOME REASON APPLE CURRENTLY CONNECTS TO RFCOMM CALLS BUT NO DATA TRANSFER CAN BE MADE
			// THIS IS HOW WE TELL THE DIFFERENCE BETWEEN CLIENTS
			// NOT EXPECTED TO WORK FOREVER
			/*
			if(RFCOMMTimer != -1) {
				RFCOMMTimer++;
				if(RFCOMMTimer > 2000) {
					//print_usb("Timeout from lack of RFCOMM response.\r\n");
					// We need to reset but start in IAP
					RFCOMMTimer = -1;
					connectionNeedsReset = 1;
					connectionType = CONNECTION_APPLE;
					//Seg7HP('r');
				}
			}
			*/

			if(connectionNeedsReset) {
				debug_message("Connection lost. Resetting %d...\r\n", isBT122);
				// Reset BT
				//Seg7LP('r');
				DPLEDOff();
				
				ElapsedMinutes = 0;
				// Reset RTC
				RTC_Initialize( RTC_PERIOD, 0, RTC_PERIOD-1, RTC_PRESCALER_DIV1024_gc );

				if (!isBT122) {
					BTEnable = 0;
					BTOff();
					osDISABLE_TASK(TaskBT);
					osWAIT(250);
					osWAIT(250);
					acqState = FSM_PREACQ;
					readyForCommands = 0;
					BTOn();
				
				
					while(!readyForCommands) {
						osWAIT(1);
					}
					ignoreNextRFCOMM = 1;
					print_bt("SDP ADD 72243468-25ee-4025-8fe7-1edd2cf13cd2 SpauldingECGRFCOMM\r");
					osWAIT(50);
					// Make settings change
					if(connectionType == CONNECTION_APPLE) {
						print_bt("SET CONTROL AUTOCALL 1101 1000 IAP\r");
						//Seg7HP('9');
					}
					else {
						print_bt("SET CONTROL AUTOCALL\r");
					}				
					connectionNeedsReset = 0;
					waitingForFirstCommand = 1;				
					osENABLE_TASK(TaskBT);
				} else {
					debug_message("SKIPPING BT122... \r\n");
					connectionNeedsReset = 0;
					waitingForFirstCommand = 1;
				}
				VBatt = cc_get_batt_volt();
				InitADCs();
				StartAcq(ACQ_IRQ_ECG);
			}
			
			if(LFReady){	// this will happen every 8 milliseconds
				LFReady=0;
				LFMag=0;
				
				for(i=0;i<8;i++){
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
					//if(i==0){ // prime the min/max
					//	LFMax = LFMin = LFMag;
					//}
					//if(LFMag > LFMax)LFMax=LFMag;
					//if(LFMag < LFMin)LFMin=LFMag;
					
					if(LFMag>LFMaxFilt[i]){
						LFMaxFilt[i]++;
					}
					if(LFMag<LFMaxFilt[i]){
						LFMaxFilt[i]--;
					}
					if(LFMag>LFMinFilt[i]){
						LFMinFilt[i]++;
					}
					if(LFMag<LFMinFilt[i]){
						LFMinFilt[i]--;
					}
					
					if(LFMinFilt[i]<B1)badLeads[i]=1;
					else if(LFMaxFilt[i]<B3)badLeads[i]=0;
					else if(LFMaxFilt[i]<B2)badLeads[i]=0;
					else badLeads[i]=1;

					//DPLEDTog();
				}
			}				
				
				
				//DPLEDTog();
				//Seg7LP(0x29+QC);

			if(ButtonPressed()) {
				//Seg7HP('b');
				if(ButtonTimer++ >= LONG_BUTTON_TIME) {
					break;
				}
			} else if(!ButtonPressed() && ButtonTimer) {
				ButtonTimer = 0;
			}
			
			lastCheckState = BTEnable;


			osWAIT(1);
		} while(!USBPresent());

		BTEnable = 0;
		BTOff();
		osDISABLE_TASK(TaskBT);
			
			// We're keeping ACQ active because we can go right back to another acquisition process after this
			//break;
		
		osDISABLE_TASK(TaskAcq); // We're done!
		Flags.AcqActive = 0;
		AnalogPowerOff();
		FlashPowerOff();
		MainPowerOff();
		Seg7LP(' ');
		DPLEDOff();
		
		while(1){
			MainPowerOff();
			osWAIT(1);
		}				
	} // while(1) back to 'E'

END_TASK

#endif