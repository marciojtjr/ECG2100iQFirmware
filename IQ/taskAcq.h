#ifndef BOOTLOADER

#ifndef taskacq_h
#define taskacq_h

#ifdef WORKING
#include "fft.h"
#endif

#define ACQ_USB_TEST 1;

void InitADCs(void);
void AcqInterrupt(void);
void PackECG(unsigned char *out, unsigned char *in);
void UnPackECG(signed int *out, unsigned char *in);

#define ECG_SAMPLES_PER_PAGE 21
#define ECG_SAMPLES_PER_MS 1
#define ECG_MS_PER_PAGE (ECG_SAMPLES_PER_PAGE / ECG_SAMPLES_PER_MS)

#define VOICE_SAMPLES_PER_PAGE	256	// samples
#define VOICE_SAMPLES_PER_MS 16
#define	VOICE_MS_PER_PAGE (VOICE_SAMPLES_PER_PAGE / VOICE_SAMPLES_PER_MS)

#define ECG_RECORD_LENGTH 	301.0 // seconds
#define VOICE_RECORD_LENGTH 32.7 // seconds

// ECG Lead assignments for PortA and PortB

// ADC channel assignments for Port A
#define	ADC_CH_UNUSED_0		ADC_CH_MUXPOS_PIN0_gc
#define ADC_CH_RL_MON 		ADC_CH_MUXPOS_PIN1_gc
#define ADC_CH_BATT			ADC_CH_MUXPOS_PIN2_gc
#define ADC_CH_V6			ADC_CH_MUXPOS_PIN4_gc
#define ADC_CH_V5		 	ADC_CH_MUXPOS_PIN5_gc
#define ADC_CH_V4			ADC_CH_MUXPOS_PIN6_gc
#define ADC_CH_V3			ADC_CH_MUXPOS_PIN7_gc

// ADC channel assignments for Port B
#define ADC_CH_VREF				ADC_CH_MUXPOS_PIN0_gc
#define ADC_CH_RA_MON			ADC_CH_MUXPOS_PIN1_gc
#define ADC_CH_MIC				ADC_CH_MUXPOS_PIN2_gc
#define ADC_CH_V2				ADC_CH_MUXPOS_PIN4_gc
#define ADC_CH_V1				ADC_CH_MUXPOS_PIN5_gc
#define ADC_CH_LL				ADC_CH_MUXPOS_PIN6_gc
#define ADC_CH_LA				ADC_CH_MUXPOS_PIN7_gc
//#define ADC_CH_VREF				ADC_CH_MUXPOS_PIN8_gc

#define PORT_LA_V2 PORTD
#define REZERO_LA	PIN3_bm
#define REZERO_LL	PIN2_bm
#define REZERO_V1	PIN1_bm
#define REZERO_V2	PIN0_bm

#define PORT_V3_V6 PORTC
#define PORT_V3_V4 PORTC
#define REZERO_V3	PIN3_bm
#define REZERO_V4	PIN2_bm
#define REZERO_V5	PIN1_bm
#define REZERO_V6	PIN0_bm

#define PORT_V6 PORTA
#define REZERO_V6_2200	PIN2_bm
#define PORT_V5 PORTB
#define REZERO_V5	PIN2_bm

#define PORT_PACE_DETECT PORTA
#define PACE_DETECT_L1 PIN3_bm

#define BT_STRING_LENGTH 110

extern float VBatt;
extern int16_t Lead[8];
extern uint16_t Acc[8];

#ifdef BLUETOOTH
 extern char btSpikeIndex;
 extern char btSpikeIndex2;
 extern char btSpikeIndex3;
 extern unsigned char badLeads[8];

 extern char pacerSpikes[3];
 extern unsigned char samplesSinceLastPacket;
#endif

// when AcqState == ACQ_IDLE, the acquisition interrupt (which is always running) does nothing. All internal pointers are reset
// when AcqState == ACQ_VOICE, the interrupt samples the microphone input and puts samples into the available buffers. If neither buffer is available
// a SystemError.BUF_OVERRUN is flagged
// when AcqState >= ACQ_ECG, the interrupt accumulates ECG sub-samples into accumulators in preparation for decimation (simple average of 16 sub-samples)
//  AcqState is incremented each interrupt to keep track of the accumulation
// when AcqState incrments to ACQ_DECIMATION, the sub-sample accumulators are scaled back to 12 bits and the eight leads are sent off to an available buffer
//   if neither buffer is available, SystemError.BUF_OVERRUN is flagged

enum {ACQ_IRQ_IDLE,  ACQ_IRQ_VOICE, ACQ_IRQ_ECG, ACQ_IRQ_ECG_DECIMATE=ACQ_IRQ_ECG+15};	// ACQ_IRQ_ECG increments into ACQ_IRQ_DECIMATE, then rolls back to ACQ_IRQ_ECG
enum {LA, LL, V1,  V2,  V3, V4, V5, V6, V7};	// offsets into Lead/Acc arrays
enum {FSM_START, FSM_EMPTY, FSM_VOICERECORDING, FSM_PREACQ, FSM_ACQURING, FSM_WAITINGFORPACKET, FSM_COMPARE, FSM_FULL, FSM_DONE, FSM_ERASING, FSM_FULL_ON, FSM_PREACQTIMEOUT};
extern unsigned char acqState;

extern char LFBiasEnable;
extern unsigned char haveFinalPacket;

#endif // taskacq_h
#endif