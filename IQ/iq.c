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
#include "taskAcq.h"
#include "seg7.h"

#include "ccpwrite.h"
//#include "sleepmgr.h"
#include "lowpower.h"
#include "rtc_driver.h"
#include "eeprom.h"
#include "TaskFlash.h"
#include "TaskWatchDog.h"

#include "taskBT.h"
#include "taskCC.h"
#include "taskUSB.h"

#define SYS_FREQ 16000	//Hz
#define POLL_FREQ 128000

unsigned char inBluetoothUpdateMode = 0;
unsigned char continuousMode = 0;
//unsigned short globalTimer = 0;

void (*resetVec)( void ) = 0x0000; // Set up function pointer to the RESET vector.

void InitUSBUART(void){
	PORTF.DIRSET = PIN3_bm;  PORTF.PIN3CTRL = PORT_OPC_WIREDAND_gc; // TXD is open collector output
	PORTF.DIRCLR = PIN2_bm;// RXD is input
}

void InitTWI(void){
	PORTC.DIRSET = PIN0_bm | PIN1_bm;  // SDA, SCL as outputs
	PORTC.PIN0CTRL = PORT_OPC_WIREDANDPULL_gc;  // SDA with pull-up
	PORTC.PIN1CTRL = PORT_OPC_WIREDANDPULL_gc;  // SCL with pull-up
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

SYS_ERR SystemError; 	// holds error flags to indicate various errors
FLAGS Flags;					// holds various flags

void SetDefaults(void){
	EE_Wr(EE_ABANDON_TIME,240);
 	EE_Wr(EE_LB,3700);
 	EE_Wr(EE_B1,1);
 	EE_Wr(EE_B2,80);
 	EE_Wr(EE_B3,40);
	EE_Wr(EE_TIMEOUT_MINUTES, 15);
 	//EE_Wr(EE_LAST_CONNECTION_TYPE, CONNECTION_UNKNOWN);
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
	//pulldown is set in iq.h
	//PORTB.PIN3CTRL = PORT_OPC_TOTEM_gc;	// fix glitch in bootstrap loaded that enabled pulldown on microphone input rather than USBDetect
	FastClock();
	HIGHPOWER_Init();
	InitSeg7();	// blank the 7 segment display
	DPLEDOff();
	AnalogPowerOff();
	FlashPowerOff();
	InitUSBDetect();
	InitButton();
	InitTWI();
	
	Flags.AcqActive = 0;

	CLK.RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;	// start the RTC (clocked at 1024Hz) so we can set registers
	while(RTC_Busy());	// Wait until RTC is ready.
	
	RTC_Initialize( RTC_PERIOD, 0, RTC_PERIOD-1, RTC_PRESCALER_DIV1024_gc );

	//------------------------------------------------------------------------------------------
	// Catch uninitialized EEPROM parameters
	// if the abandon time is a nonsense value, set all EEPROM coefficients to default
	if(EE_Rd(EE_ABANDON_TIME)>600){
		SetDefaults();
	}
	
	// ECR #116:
	//    As the timeout minutes were added as a firmware update,
	//    we need to set its default separately as the ABANDON_TIME
	//    has already been set to a sane value
	if(EE_Rd(EE_TIMEOUT_MINUTES) > 600) {
		EE_Wr(EE_TIMEOUT_MINUTES, 15);
	}

	while(1){

		//------------------------------------------------------------------------------------------
		// Set up the main system timer and interrupt
		TC_SetPeriod(&TCC1,F_CPU/SYS_FREQ-1);	// set the TC period
		TC1_ConfigClockSource(&TCC1, TC_CLKSEL_DIV1_gc );	// start timer by selecting a clock source 1x
		TC1_SetOverflowIntLevel(&TCC1, TC_OVFINTLVL_LO_gc );	// low level interrupt on overflow

		PMIC.CTRL |= PMIC_LOLVLEX_bm + PMIC_MEDLVLEX_bm + PMIC_HILVLEX_bm;

		StartScheduler();	// should never return

	  return 0;  // should never get here
	}
}

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

	//-------------------------------------------------------------------------------------------
	// First, we handle the power-up condition, where the button was pressed or we were plugged into USB while we were off
	if(!USBPresent()){ // if we're not on usb, the button powered us up
		osENABLE_TASK(TaskAcq);
	}

	else{ // USBPresent(), so it was a USB connection that powered us up. Enable TaskUSB and TaskFlash. If we have a buffer error condition, the PC will handle it.
		osENABLE_TASK(TaskUSB);
	}
	
	osENABLE_TASK(TaskCC);

	//-------------------------------------------------------------------------------------------
	// We've handled all the power up conditions. If we make it this far, it's because:
		// We're connected to USB (which keeps the power on)
		// TaskAcq has acquired an ECG (ElapsedMinutes will be one) and has disabled itself, leaving us to mark ElapsedMinutes
			// If TaskAcq is abandoned and times out, it will turn off power itself.
			// If ElapsedMinutes goes to one, TaskAcq has disabled itself and we must switch to low power operation and...
				// keep track of the elapsed time
				// watch for button presses, so we can display 'F' during the press
				// watch for USBPresent(), so we can come back up to high speed operation and enable TaskUSB and TaskFlash
				// watch for !USBPresent(), so we can switch back to low power operation, if ElapsedMinutes is still non-zero (meaning a host PC did not yet upload the ECG)


	// if ElapsedMinutes does go to zero, we can turn off. If we're still on USB, we stay at high speed with TaskUSB and TaskFlash running.
	//-------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------
	while(1){	// check forever (we'll eventually leave by powering down inside this loop)
		// The only way to exit this loop is by powering down and there are only two ways to do that and only one of those is proper.

		// The proper way to exit this loop:
		// 1) ElapsedMinutes is reset to zero by the host PC (which marks the end of the upload, along with both flash buffers going empty)
		//		AND we get disconnected from USB

		// The improper way to exit this loop:
		// 2) Some kind of system upset (low battery or glitch) shuts us down
		// 		There's no way to know where in the loop that happens, so we don't try to handle it

		// If we can't power down, we've got to figure out what to do while we're in this loop
		// If we're disconnected from USB and ElapsedMinutes has NOT been cleared, we'll slip into low power mode again, disabling TaskUSB and TaskFlash (once TaskFlash is idle?)
		// So long as we're connected to USB, we'll stay powered and running at high speed with TaskUSB and TaskFlash enabled
		// If ElapsedMinutes has been cleared, we'll shut off.

		// If we were powered-up by a button press and went into acquisition and that completed, ElapsedMinutes will be set to one and we'll slip into low power mode
		// If acquisition did not complete successfully, ElapsedMinutes will not be set to one and we'll power down.

		if(CLKSYS_IsReady(OSC_PLLRDY_bm)){ // if we're running at high speed, we can allow the scheduler to run
			osWAIT(1);
		}
		else{	// we're running too slow to allow use of the scheduler, so we handle the watchdog directly and do not release to the scheduler
			wdt_reset();
			DPLEDOff();	// in case it was left on by the watchdog task, which no longer runs
		}

		// Now we've got to handle the 7-segment display
		// If we're powered by USB, we can be 'F'ull, 'C'harging or 'E'mpty
		if(USBPresent()){
			if(BatteryIsCharging()){
#ifdef WORKING
				if(!inBluetoothUpdateMode) 
#endif
					Seg7LP('C');
			}
			if(BatteryIsCharged()){
#ifdef WORKING
				if(!inBluetoothUpdateMode) {
#endif
					Seg7LP('C');
					DPLEDOn();
#ifdef WORKING
				}
#endif
			}
		}
		// if we're not powered by USB and TaskAcq is not running, then show 'F'ull or 'E'mpty or "L" while the button is pressed and nothing otherwise
		// if TaskAcq is running, it will handle the button, the display, and power
		else if(!Flags.AcqActive){
			if(!ElapsedMinutes){
				MainPowerOff();	// TaskAcq isn't running and ElapsedMinutes is zero (so even if we've recorded something, we can't maintain it)... so turn off when the button is released
			}
			if(!ButtonPressed()){ // the button is released and we're still running, so blank the display (it draws a lot of power)
				Seg7LP(' ');
			}
		}
	} // while(1)

END_TASK

/*
ISR(TCE1_OVF_vect) {
	// Our own "interrupt" for BT USART
	if((BTPortDescriptor.usart->STATUS & USART_DREIF_bm) != 0 && BTRTS()) {
		USART_DataRegEmptyNotInterrupt(&BTPortDescriptor);
	}
	if((BTPortDescriptor.usart->STATUS & USART_RXCIF_bm) != 0) {
		USART_RXComplete(&BTPortDescriptor);
		//BTPortDescriptor.usart->STATUS &= ~USART_RXCIF_bm;
	}
}
*/

// 16KHz main interrupt. Drives acquisition and SysTick
ISR(TCC1_OVF_vect){
//	DPLEDOn();
	static char i;
	AcqInterrupt();
	
	
	if( (i++&0xf)==0){ 
		//++globalTimer;
		SysTick();
	}
//	DPLEDOff();
}

#endif