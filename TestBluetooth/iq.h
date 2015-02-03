#ifndef BOOTLOADER
#ifndef iq_h
#define iq_h

#define ENABLE_DEBUG 0	// set non-zero if you want the debug code included

#ifdef BLUETOOTH
#define VERSION 2.00
#else
#define VERSION 1.06
#endif

// -----------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------2.0 Production Changelist-----------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------
// 2.00 - Initial release of Bluetooth iQ
// -----------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------1.0 Production Changelist-----------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------
// 1.06 - 1.0 Production update, with Full mode, ECG Acquisition timer, Single Button presses, and all applicable changes for 2.0
// 1.05 - Built for bluetooth (no change to original prototype)
// 1.04 - unit no longer powers off if an erase command is launched via USB and the unit is unplugged
//				USB ev and ee commands have been removed, they were not used
//				EE_Wr() now checks for valid offsets. They must be long aligned and not include EE_APP_CRC.
// 1.03 - bluetooth fork added
// 1.02 - internal version
// 1.01 - first update
// 1.00 - initial release
// -----------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

/***************************************************************
 *** INTERNAL BUILDS
 ***************************************************************
 *** 13.8.22b1 - Possible fix for Shaun's CRC issue
 ***************************************************************/

#define AnalogPowerOn() 	PORTC.DIRSET = PIN6_bm; PORTC.OUTSET = PIN6_bm
#define AnalogPowerOff() 	PORTC.DIRSET = PIN6_bm; PORTC.OUTCLR = PIN6_bm

#define FlashPowerOn()
#define FlashPowerOff()

#define MainPowerOn() 	PORTF.DIRSET = PIN6_bm; PORTF.OUTSET = PIN6_bm; PORTF.PIN6CTRL = PORT_OPC_TOTEM_gc
#define MainPowerOff()	{ PORTF.DIRSET = PIN6_bm; PORTF.OUTCLR = PIN6_bm; PORTF.PIN6CTRL = PORT_OPC_TOTEM_gc; FlashPowerOff(); AnalogPowerOff(); }

#define BatteryIsCharged() 	(PORTC.IN & PIN5_bm)
#define BatteryIsCharging()	(!(PORTC.IN & PIN5_bm))

#define DPLEDOn()	   PORTE.DIRSET = PIN7_bm; PORTE.OUTCLR = PIN7_bm; PORTE.PIN7CTRL = PORT_OPC_TOTEM_gc
#define DPLEDOff()		PORTE.DIRSET = PIN7_bm; PORTE.OUTSET = PIN7_bm; PORTE.PIN7CTRL = PORT_OPC_TOTEM_gc
#define DPLEDTog()		PORTE.DIRSET = PIN7_bm; PORTE.OUTTGL = PIN7_bm; PORTE.PIN7CTRL = PORT_OPC_TOTEM_gc

#define InitButton()	PORTC.DIRCLR = PIN7_bm; PORTC.PIN7CTRL = PORT_OPC_PULLDOWN_gc
#define ButtonPressed()	(PORTC.IN & PIN7_bm)

#define InitUSBDetect()	PORTB.DIRCLR = PIN3_bm; PORTB.PIN3CTRL = PORT_OPC_PULLDOWN_gc	// set USB sense pin to input with pulldown
#define USBPresent()	 	(PORTB.IN & PIN3_bm)		// pin is high if USB is present and awake
//#define USBPresent()	 	0

#define GroundUnused() PORTA.DIRSET = PIN0_bm; PORTA.OUTCLR = PIN0_bm;

#define InitRTS() PORTF.DIRCLR = PIN1_bm
#define RTS() ((PORTF.IN & PIN1_bm) == 0)

#define LFBiasOff()	PORTC.DIRCLR = PIN4_bm;
#define LFBiasOn()	{PORTC.DIRSET = PIN4_bm;}
#define LFBiasTog() PORTC.OUTTGL = PIN4_bm;
#define LFBias()		(PORTC.DIR && PIN4_bm)

#define BIT_FIELD_SIZE 8

typedef struct {
	unsigned char BUF_OVERRUN				:BIT_FIELD_SIZE;	// set when the acquisition interrupt can't find a free buffer
} SYS_ERR;

typedef struct {
	unsigned char AcqActive					:BIT_FIELD_SIZE;	// set when the acquisition task is running
	unsigned char PreAcq						:BIT_FIELD_SIZE;	// set when we are pre-acquiring, during which we rapidly rezero leads
} FLAGS;

void SetDefaults(void);
void Error(unsigned char);

#include <stdio.h>
#include "usart_driver.h"

#define BT_STATE_SIZE 7

extern FILE *bt;

extern unsigned char Verbose;

extern unsigned char BTEnable;
extern unsigned char BTReady;	// 1 = a leadfail bias model is ready for decimation, 0 = not ready
extern unsigned char BTBufPhase; // 0 = decimation of LFBiasModel[0] loads the BTBuf, 1 = decimation of LFBiasModel[1] adds to BTBuf, after which /8 produces an average of both models
extern unsigned int BTBuf[];	// holds all eight channels of data, decimated from 1KHz to 125Hz.
extern unsigned char BTString[]; // room to store the Bluetooth sample output string.
extern unsigned char M;	// selects which of the two LFBiasModels is ready for examination, 0 or 1
extern int LFBiasModel[2][8][4];	// two models (so we can ping-pong) for the eight channels, each model containing four samples
extern unsigned char BTStatus;
extern char BTStateString[BT_STATE_SIZE];
extern USART_desc_t BTPortDescriptor;

void BTOn(void);
void BTOff(void);

// macro to make sending stuff a little easier
#define print_bt(fmt,...)  fprintf_P(bt, PSTR(fmt), ## __VA_ARGS__)
#define print_bt_debug(fmt,...) if(Debug) fprintf_P(bt, PSTR(fmt), ## __VA_ARGS__)
#define print_bt_verbose(fmt,...) if(Verbose) fprintf_P(bt, PSTR(fmt), ## __VA_ARGS__)

#define BT_USART			USARTE0
#define BT_USART_PORT PORTE

extern USART_desc_t BTPortDescriptor;	// a descriptor for the USART connected to the Bluetooth transceiver
extern FILE bt_str;

enum {FSM_START, FSM_EMPTY, FSM_VOICERECORDING, FSM_PREACQ, FSM_ACQURING, FSM_WAITINGFORPACKET, FSM_COMPARE, FSM_FULL, FSM_DONE, FSM_ERASING, FSM_FULL_ON, FSM_PREACQTIMEOUT};

#endif
#endif