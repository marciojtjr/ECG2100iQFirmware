#ifndef BOOTLOADER

#include <avr/io.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <math.h>
#include <avr/sleep.h>

#include "rtos.h"

#include "iq.h"
#include "avr_compiler.h"
#include "TC_driver.h"
#include "clksys_driver.h"
#include "wdt_driver.h"
#include "usart_driver.h"
#include "eeprom_driver.h"
#include "adc_driver.h"
#include "seg7.h"

#include "ccpwrite.h"
#include "sleepmgr.h"
#include "lowpower.h"
#include "rtc_driver.h"
#include "eeprom.h"
#include "TaskWatchDog.h"

#include <avr/io.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <math.h>
#include <avr/sleep.h>

#include "rtos.h"

#include "iq.h"
#include "avr_compiler.h"
#include "TC_driver.h"
#include "clksys_driver.h"
#include "wdt_driver.h"
#include "usart_driver.h"
#include "eeprom_driver.h"
#include "adc_driver.h"
#include "buffers.h"
#include "eeprom.h"
#include "seg7.h"

#include "lowpower.h"

#include "crc.h"

unsigned char BTBufReady;
unsigned int BTBuf[8];
unsigned char transferingFullData = 0;
char BTStateString[BT_STATE_SIZE];

enum { FORMAT_DECIMAL, FORMAT_BINARY }; // various file output formats
static unsigned char format = FORMAT_BINARY;

//##########################################################################################
// define and allocate globals for the bluetooth task

USART_desc_t BTPortDescriptor;	// a descriptor for the USART connected to the Bluetooth transceiver
static int put_bt( char c, FILE *stream){
	uart_putchar(&BTPortDescriptor,c);
	return(0);
}

#define put_byte(c) uart_putchar(&BTPortDescriptor,c)

FILE bt_str = FDEV_SETUP_STREAM(put_bt, 0, _FDEV_SETUP_WRITE);
FILE *bt;

void BTOn(void){
	PORTF.DIRSET = PIN0_bm;
	PORTF.OUTSET = PIN0_bm;
	PORTF.PIN0CTRL = PORT_OPC_TOTEM_gc;
}

void BTOff(void){
	PORTF.DIRSET = PIN0_bm;
	PORTF.OUTCLR = PIN0_bm;
	PORTF.PIN0CTRL = PORT_OPC_TOTEM_gc;
}

const unsigned char Hex[16] PROGMEM = {"0123456789ABCDEF"};
char bin2Hex(unsigned char c){
	return(pgm_read_byte(&Hex[c & 0xf]));
}

short
CRCCalc(short crc, short x)
{
	x = ((crc>>8) ^ x) & 0xff;
	x ^= x>>4;
	return (crc << 8) ^ (x << 12) ^ (x <<5) ^ x;
}

//------------------------------------------------------------------------------------------
// Bluetooth USART Interrupts (low level)
ISR(USARTE0_RXC_vect){
	USART_RXComplete(&BTPortDescriptor);
}

ISR(USARTE0_DRE_vect){
	USART_DataRegEmpty(&BTPortDescriptor);
}

#define SYS_FREQ 16000	//Hz

#ifdef __WORKING
#define RTC_PERIOD	3840 // Configure RTC wakeup period to one minute, it clocks at 1 (1024Hz from 32KHz internal oscillator / 1024 prescaler)
#else
#define RTC_PERIOD 60
#endif

#ifdef WORKING
unsigned char inBluetoothUpdateMode = 0;
#endif

void (*resetVec)( void ) = 0x0000; // Set up function pointer to the RESET vector.

void InitUSBUART(void){
	PORTF.DIRSET = PIN3_bm;  PORTF.PIN3CTRL = PORT_OPC_WIREDAND_gc; // TXD is open collector output
	PORTF.DIRCLR = PIN2_bm;// RXD is input
}

void SlowClock(void){
	CLKSYS_Enable( OSC_RC32KEN_bm );
	CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc );
	while ( CLKSYS_IsReady( OSC_RC32KRDY_bm ) == 0 );
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32K_gc );
	CLKSYS_Disable( OSC_XOSCEN_bm | OSC_RC2MEN_bm | OSC_RC32MEN_bm | OSC_PLLEN_bm ); // other oscillators draw power, shut them off
	CLKSYS_Disable( OSC_XOSCEN_bm | OSC_RC2MEN_bm | OSC_RC32MEN_bm | OSC_PLLEN_bm ); // other oscillators draw power, shut them off
	CLKSYS_Disable( OSC_XOSCEN_bm | OSC_RC2MEN_bm | OSC_RC32MEN_bm | OSC_PLLEN_bm ); // other oscillators draw power, shut them off
}

void FastClock(void){
	//------------------------------------------------------------------------------------------
	// Set the CPU clock to 32MHz using an external 16MHz crystal and the PLL set to x2
	CLKSYS_XOSC_Config( OSC_FRQRANGE_12TO16_gc, false, OSC_XOSCSEL_XTAL_16KCLK_gc );	// setup for 16MHz external crystal
	CLKSYS_Enable( OSC_XOSCEN_bm );	// enable the external crystal oscillator
	while ( CLKSYS_IsReady( OSC_XOSCRDY_bm ) == 0 );	// wait till it's stable for use as the PLL input
	CLKSYS_PLL_Config( OSC_PLLSRC_XOSC_gc, 2 );	// drive the PLL from the external crystal, set multiplication to 2x
	CLKSYS_Enable( OSC_PLLEN_bm );	// enable the PLL
	CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc );	// set up the prescalers
	while ( CLKSYS_IsReady( OSC_PLLRDY_bm ) == 0 );	// wait for PLL to stabilize
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_PLL_gc );	// start clocking the system from it
	CLKSYS_Disable( OSC_RC2MEN_bm | OSC_RC32MEN_bm); // turn off oscillators we don't use
	CLKSYS_Enable( OSC_RC32KEN_bm );	// we need the low power 32KHz clock for the RTC
}

void RC32MClock(void){
	//------------------------------------------------------------------------------------------
	// Set the CPU clock to 32MHz
	CLKSYS_Enable( OSC_RC32MEN_bm );
	while ( CLKSYS_IsReady( OSC_RC32MRDY_bm ) == 0 );
	CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc );
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32M_gc );
	CLKSYS_Disable( OSC_RC2MEN_bm | OSC_XOSCEN_bm | OSC_RC32KEN_bm );
}

unsigned int ElapsedMinutes;	// keeps track of time since the ECG was recorded, in minutes.
unsigned int buttonTimer = 0;

#ifdef WORKING
unsigned long int currentTimer = 0;
#endif

#ifdef WORKING
unsigned char lpState = LP_OFF;
#endif

SYS_ERR SystemError; 	// holds error flags to indicate various errors
FLAGS Flags;					// holds various flags

void SetDefaults(void){
	EE_Wr(EE_ABANDON_TIME,240);
 	EE_Wr(EE_LB,3700);
 	EE_Wr(EE_B1,1);
 	EE_Wr(EE_B2,80);
 	EE_Wr(EE_B3,40);
}

// Flash an error code in the LED for 5 seconds, then reset the CPU
void Error(unsigned char c){
	Seg7HP(c+'0');	// show the error code
	while(1);
}

//==========================================================================================
//main

int main(void){

	WDT_EnableAndSetTimeout( WDT_PER_2KCLK_gc );
	PORTB.PIN3CTRL = PORT_OPC_TOTEM_gc;	// fix glitch in bootstrap loaded that enabled pulldown on microphone input rather than USBDetect
	FastClock();
	InitSeg7();	// blank the 7 segment display
	DPLEDOff();
	AnalogPowerOff();
	FlashPowerOff();
	//Seg7HP('P');
	InitUSBDetect();
	InitButton();
	Flags.AcqActive = 0;

	CLK.RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;	// start the RTC (clocked at 1024Hz) so we can set registers
	while(RTC_Busy());	// Wait until RTC is ready.
	
	RTC_Initialize( RTC_PERIOD, 0, RTC_PERIOD-1, RTC_PRESCALER_DIV1_gc );	// Configure RTC wakeup period, clock it at 1Hz

	//------------------------------------------------------------------------------------------
	// Catch uninitialized EEPROM parameters
	// if the abandon time is a nonsense value, set all EEPROM coefficients to default
	if(EE_Rd(EE_ABANDON_TIME)>600){
		SetDefaults();
	}
	if(EE_Rd(EE_ENABLE_VOICE)>1) {
		EE_Wr(EE_ENABLE_VOICE, 1);
	}

	while(1){

		//------------------------------------------------------------------------------------------
		// Set up the main system timer and interrupt
		TC_SetPeriod(&TCC1, F_CPU/SYS_FREQ-1);	// set the TC period
		TC1_ConfigClockSource(&TCC1, TC_CLKSEL_DIV1_gc );	// start timer by selecting a clock source 1x
		TC1_SetOverflowIntLevel(&TCC1, TC_OVFINTLVL_LO_gc );	// low level interrupt on overflow

		PMIC.CTRL |= PMIC_LOLVLEX_bm + PMIC_MEDLVLEX_bm + PMIC_HILVLEX_bm;

		StartScheduler();	// should never return

	  return 0;  // should never get here
	}
}

#ifdef WORKING
void resetLowPowerStatus() {
	lpState = LP_OFF;
	Seg7LP(' ');
	currentTimer = getCurrentLowPowerClockValue();
}
#endif

unsigned char BTEnable = 0;	// 1 enables bluetooth transmission
unsigned char BTReady;	// 1 = a leadfail bias model is ready for decimation, 0 = not ready
unsigned char BTBufPhase; // 0 = decimation of LFBiasModel[0] loads the BTBuf, 1 = decimation of LFBiasModel[1] adds to BTBuf, after which /8 produces an average of both models
unsigned int BTBuf[8];	// holds all eight channels of data, decimated from 1KHz to 125Hz.
// unsigned char BTString[29]; // room to store the Bluetooth sample output string.
unsigned char BTString[6+1+1+1+2+8*12+2]; // Bigger now.
unsigned char BTStatus;

//------------------------------------------------------------------------------------------
// handle powerups and elapsed time tracking
// If we're powered up by USB, enable TaskUSB and TaskFlash
// If we're powered up by the button and we're full, display "F" until the button is released
// If we're powered up by the button and we're empty, enable TaskAcq and TaskFlash
// If TaskAcq sets ElapsedMinutes to 1 (we're full), we don't actually power down, but enter a low power
// mode where we keep track of time since we went full.
// We stay in lower power mode so long as ElapsedMinutes!=0
// The only thing which sets ElapsedMinutes to zero is a read of ElapsedMinutes by the host PC, or a power on reset.
// While ElapsedMinutes!=0, if we're connected to USB, we bring ourselves up to full speed and enable TaskUSB and TaskFlash
// if we are not connected, or become disconnected, we re-enter the low power mode

// This is the only task enabled to run after reset, so we needn't worry about what other tasks are doing until we enable them.

BEGIN_TASK(TaskMain)
	static int counter = 0;
	
	LFBiasOn();
	Seg7LP('b');
	
	/*
	while(1) {
		MainPowerOn();
		wdt_reset();
		
		osWAIT(2);
		if((++counter) == 50) {
			counter = 0;
			DPLEDTog();
		}
		LFBiasTog();
		//DPLEDTog();
	}
	*/

	//------------------------------------------------------------------------------------------
	// prepare the Bluetooth USART
#ifdef BAUDRATECHANGER
	#define BT_BAUD_RATE 115200
#else
	#define BT_BAUD_RATE 921600
#endif

	bt = &bt_str;
	BT_USART_PORT.DIRSET = PIN3_bm;// TXD is output
	BT_USART_PORT.DIRCLR = PIN2_bm;// RXD is input

	USART_InterruptDriver_Initialize(&BTPortDescriptor, &BT_USART, USART_DREINTLVL_LO_gc); // setup USART, DRE interrupt priority is LOW
	USART_RxdInterruptLevel_Set(BTPortDescriptor.usart, USART_RXCINTLVL_LO_gc); // receive interrupt priority is LOW
#ifdef BAUDRATECHANGER
	USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N1
	USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/BT_BAUD_RATE-1), 0);
#else
	USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, true);	//8N1
	USART_Baudrate_Set(BTPortDescriptor.usart, 19, -4); //921600 bps
#endif
	
	//USART_Format_Set(BTPortDescriptor.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_EVEN_gc, false);	//8E1
	//USART_Baudrate_Set(BTPortDescriptor.usart, 33, -1); //921600 bps
	//USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/115200-1), 0);
	
	//USART_Baudrate_Set(BTPortDescriptor.usart, (F_CPU/16/BT_BAUD_RATE-1), 0);  // Does not work for all Baud Rates (BAUD_RATE = 921600 for example)
	USART_Rx_Enable(BTPortDescriptor.usart);
	USART_Tx_Enable(BTPortDescriptor.usart);
  
    unsigned char ix = 0;
    static unsigned int FrameIndex = 0;
//#ifdef COMPARE_VERSION
//    short crc;
//#else
	static unsigned int crc;
//#endif
	static uint32_t waiting = 0;
	static uint32_t beforeSend = 0;

    BTString[ix++] = 0xAA;  // Preamble byte 1
    BTString[ix++] = 0x55;  // Preamble byte 2
    BTString[ix++] = 0xB3;  // Preamble byte 3
    BTString[ix++] = 0x3B;  // Preamble byte 4
    BTString[ix++] = 0xAF;  // Preamble byte 5
    BTString[ix++] = 0xFA;  // Preamble byte 6
	
	BTStateString[0] = 0xFA; // Status preamble byte 1
	BTStateString[1] = 0xAF; // Status preamble byte 2
	BTStateString[2] = 0x3B; // Status preamble byte 3
	BTStateString[3] = 0xB3; // Status preamble byte 4
	BTStateString[4] = 0x55; // Status preamble byte 5
	BTStateString[5] = 0xAA; // Status preamble byte 6
	
/* FOR PACE DETECTION */


    #define BT_PREAMBLE   0
    #define BT_TYPE       6
    #define BT_STATUS     7
    #define BT_LEADOFF    8
	#define BT_PACESPIKE  9
    #define BT_FRMINDEX   10
    #define BT_DATA0     12
    #define BT_DATA1     60
    #define BT_CRC      108

    #define BT_SIZE     110
	
//#ifdef COMPARE_VERSION
	static unsigned int i;
//#endif

	static char acqState = FSM_PREACQ;
	static int btSpikeIndex = 0xFF;
	static int btSpikeIndex2 = 0xFF;
	static char haveFinalPacket = 0;
	
			BTBufPhase = 0;
			BTStatus = 0;               // Added by PRV

	//==========================================================================================
	// Bluetooth command parser

//	if(!USBPresent() ){	// we can run Bluetooth only on non rev 1 hardware (which has unprogrammed JTAG UID)
		// Wait until we're in preacq before turning on the radio
		// Or if we're full, let it go through as well
		while(acqState < FSM_PREACQ && acqState != FSM_FULL) {
			osWAIT(1);
		}			
		
		BTOn();
		
		// Wait until we've recieved "READY" from the BT chip
		while(readyForCommands == 0) {
			osWAIT(1);
		}			
		
		// Set PIO5 as the CD pin
		print_bt("SET CONTROL CD 20 1\r");
		
		// Make sure our name is our serial number
		print_bt("SET BT NAME SpauCliQ%02d%02d%04d\r",(int)((EE_Rd(EE_SERIAL_NUM)/1000000)%100), (int)((EE_Rd(EE_SERIAL_NUM)/10000)%100), (int)(EE_Rd(EE_SERIAL_NUM)%10000));
		
		// Make sure our UART settings are correct
		print_bt("SET CONTROL BAUD 921600,8n1\r");	
		
		// Add entry for iOS devices
		//print_bt("SDP ADD 00000000-deca-fade-deca-deafdecacaff Wireless iAP\r");
		
		BTReady = 1;
		while(1) {	
			MainPowerOn();
			wdt_reset();
			if(!BTEnable) {
				
				// Wait until the CD pin goes high (which happens when we enter data mode)
				if(PORTF.IN & PIN7_bm) {
					BTEnable = 1;
					
					// Clear any status bits that have been set
					BTStatus = 0;
					
					// Write out our current state
					BTStateString[6] = FSM_PREACQ;
					fwrite(BTStateString, BT_STATE_SIZE, 1, bt);
				}
				
			}	
			else if(BTEnable) {
				LPWAIT(8);
				BTReady = 1;
				if(BTReady) {// we have a lead fail model to decimate
					BTReady=0;

						BTString[BT_TYPE] = 1;     // Record type
						BTString[BT_STATUS] = 0;     // Status Flags
						BTString[BT_LEADOFF] = 0;     // Lead Off Indicator Bits
						BTString[BT_PACESPIKE] = 0;		// Pacer spike index bits
						//if(btSpikeIndex2 != 0xFF && (BTStatus & 8)) BTString[BT_PACESPIKE] |= (btSpikeIndex2 << 4);
						
						for(i = BT_DATA0; i < BT_CRC-BT_DATA0; i++) {
							BTString[i] = 0;
						}													
						
						//if(BTStatus & 8) {
							//print_bt("Pacer spike detected at index: %d, %d, sample index: %d\r\n", btSpikeIndex, btSpikeIndex2, FrameIndex);
							//print_bt("Pacer spike index 1: %d\r", btSpikeIndex);
							//print_bt("Pacer Spike index 2: %d\r", btSpikeIndex2);
							//print_bt("We have a pacer spike: "BYTETOBINARYPATTERN"\r", BYTETOBINARY(BTString[BT_PACESPIKE]));
						//	BTStatus &= ~8; // Clear pacer bit 
						//	btSpikeIndex = 0xFF; // Clear spike indecies
						//	btSpikeIndex2 = 0xFF;
						//}					
							
						BTString[BT_FRMINDEX] = FrameIndex >> 8;     // Lead Off Indicator Bits
						BTString[BT_FRMINDEX+1] = FrameIndex;     // Lead Off Indicator Bits

						crc = 0;
						for (unsigned char jx = BT_TYPE; jx < BT_CRC; jx++)
							crc = CRCCalc(crc, BTString[jx]);

						BTString[BT_CRC] = crc >> 8;
						BTString[BT_CRC+1] = crc;
						/*
						for (jx = 0; jx < ix; jx++)
							print_bt("%02X",BTString[jx]);
						print_bt("\n");
						*/
						
						fwrite(BTString, BT_SIZE, 1, bt);
						
						if(acqState == FSM_WAITINGFORPACKET)
							haveFinalPacket = 1;

						FrameIndex++;
					

					}
					//osYIELD;
				}				
			    //osYIELD;	// let other stuff run
		    }	

END_TASK

// 16KHz main interrupt. Drives acquisition and SysTick
ISR(TCC1_OVF_vect){
//	DPLEDOn();
	static char i;
	//AcqInterrupt();
	if( (i++&0xf)==0){
		SysTick();
	}
//	DPLEDOff();
}

#endif