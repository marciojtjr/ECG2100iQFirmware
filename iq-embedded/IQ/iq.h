#ifndef iq_h
#define iq_h

#define ENABLE_DEBUG 0	// set non-zero if you want the debug code included
//#define COMPARE_VERSION
//#define LOGGING_VERSION

#define VERSION 1.05

// 1.05 - Built for bluetooth (no change to original prototype)
// 1.04 - unit no longer powers off if an erase command is launched via USB and the unit is unplugged
//				USB ev and ee commands have been removed, they were not used
//				EE_Wr() now checks for valid offsets. They must be long aligned and not include EE_APP_CRC.
// 1.03 - bluetooth fork added
// 1.02 - internal version
// 1.01 - first update
// 1.00 - initial release

#define AnalogPowerOn() 	PORTC.DIRSET = PIN6_bm; PORTC.OUTSET = PIN6_bm
#define AnalogPowerOff() 	PORTC.DIRSET = PIN6_bm; PORTC.OUTCLR = PIN6_bm

#define FlashPowerOn()
#define FlashPowerOff()

#define MainPowerOn() 	PORTF.DIRSET = PIN6_bm; PORTF.OUTSET = PIN6_bm; PORTF.PIN6CTRL = PORT_OPC_TOTEM_gc
#define MainPowerOff()	{ PORTF.DIRSET = PIN6_bm; PORTF.OUTCLR = PIN6_bm; PORTF.PIN6CTRL = PORT_OPC_TOTEM_gc; FlashPowerOff(); AnalogPowerOff(); }

#define BatteryIsCharged() 	(PORTC.IN & PIN5_bm)
#define BatteryIsCharging()	(!(PORTC.IN & PIN5_bm))

#define DPLEDOn()			PORTE.DIRSET = PIN7_bm; PORTE.OUTCLR = PIN7_bm; PORTE.PIN7CTRL = PORT_OPC_TOTEM_gc
#define DPLEDOff()		PORTE.DIRSET = PIN7_bm; PORTE.OUTSET = PIN7_bm; PORTE.PIN7CTRL = PORT_OPC_TOTEM_gc
#define DPLEDTog()		PORTE.DIRSET = PIN7_bm; PORTE.OUTTGL = PIN7_bm; PORTE.PIN7CTRL = PORT_OPC_TOTEM_gc

#define InitButton()	PORTC.DIRCLR = PIN7_bm; PORTC.PIN7CTRL = PORT_OPC_PULLDOWN_gc
#define ButtonPressed()	(PORTC.IN & PIN7_bm)

#define InitUSBDetect()	PORTB.DIRCLR = PIN3_bm; PORTB.PIN3CTRL = PORT_OPC_PULLDOWN_gc	// set USB sense pin to input with pulldown
#define USBPresent()	 	(PORTB.IN & PIN3_bm)		// pin is high if USB is present and awake
//#define USBPresent()	 	0

#define GroundUnused() PORTA.DIRSET = PIN0_bm | PIN3_bm; PORTA.OUTCLR = PIN0_bm | PIN3_bm;

#define InitRTS() PORTF.DIRCLR = PIN1_bm
#define RTS() ((PORTF.IN & PIN1_bm) == 0)

#define LFBiasOff()	PORTC.DIRCLR = PIN4_bm;
#define LFBiasOn()	if(LFBiasEnable){PORTC.DIRSET = PIN4_bm;}
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

extern unsigned int DCOffset;
extern SYS_ERR SystemError;
extern FLAGS Flags;

void SetDefaults(void);
void Error(unsigned char);

// ElapsedMinutes keeps track of time since ECG was recorded, in minutes. This is reset to zero on powerup and doesn't
// start incrementing until the end of an ECG recording.
extern unsigned int ElapsedMinutes;

#endif
