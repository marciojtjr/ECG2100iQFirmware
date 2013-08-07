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
#include "sleepmgr.h"
#include "lowpower.h"
#include "rtc_driver.h"
#include "eeprom.h"
#include "TaskFlash.h"
#include "TaskAcq.h"
#include "TaskWatchDog.h"

#include "taskBT.h"

#define SYS_FREQ 16000	//Hz
#define RTC_PERIOD	60 // Configure RTC wakeup period to one minute, it clocks at 1 (1024Hz from 32KHz internal oscillator / 1024 prescaler)

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

//! Global variable to keep track of which state the program is in.
uint8_t gState = 0;

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
	RTC_Initialize( RTC_PERIOD, 0, RTC_PERIOD-1, RTC_PRESCALER_DIV1024_gc );	// Configure RTC wakeup period, clock it at 1Hz

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

	ElapsedMinutes = 0;	// we are powering up, there's no way we can know what the elapsed time is, so zero it

	if(EE_Rd( (EE_VOICE_RECORD_LENGTH)!=0) && (EE_Rd(EE_ECG_RECORD_LENGTH)!=0) ){	// we're truly full
		// all we can do is display the "F" message until the button is released. This is also an error condition.
		// We should never power up full because we never turn off once full. We stay in low power operation tracking elapsed time.
		// If we power up in a full condition, maybe we were in that mode so long that the battery died or we had some kind of system upset?
		// Regardless of the reason we were off, it means we don't know the ElapsedMinutes. So powerup zeros it and we let it be.
		// There is also no reason to keep track of elapsed time once this happens, so we can power back down

		Seg7LP('F');
		acqState = FSM_START;
		
		//MainPowerOn();
	}
	//!!!!! USBSuspend seems to "flicker" at connection, will this cause a problem? probably not.

	if(!USBPresent()){ // if we're not on usb, the button powered us up

		if( (EE_Rd(EE_VOICE_RECORD_LENGTH)!=0) && (EE_Rd(EE_ECG_RECORD_LENGTH)!=0) ){	// we're truly full, there's nothing more to do
			
			do {
				MainPowerOff();
				osWAIT(1);
			}while(ButtonPressed());
			
			// If we get here, we were plugged into USB while the button was being held, now the button is released but USB will keep us powered
			// The easiest thing to do is probably to jump to the reset vector (letting the watchdog expire would take us to the bootstrap loader)
			resetVec();
		}

		else if( (EE_Rd(EE_VOICE_RECORD_LENGTH)==0) && (EE_Rd(EE_ECG_RECORD_LENGTH)==0) ){	// we're truly empty, enable acquisition
			osENABLE_TASK(TaskAcq);
			// TaskAcq is responsible for handling the button. It will also turn off power if acquisition is abandoned.
			// If acquisition completes successfully, TaskAcq will set ElapsedMinutes = 1 and disable itself
		}

		else{ // ERROR! Either Voice or ECG has data, but the other buffer is empty. We should never power up in this state. Erase everything, go back to empty, and start acquisition.
			MainPowerOn();	// keep ourselves on until erase is complete
			do{
				osWAIT(10);
			}while(TaskFlashCmd!=TF_IDLE);	// wait till Flash is ready to go

			TaskFlashCmd = TF_ERASE_ALL;	// erase everything

			do{
				osWAIT(100);
				Seg7HP(0x23);
				osWAIT(100);
				Seg7HP(0x24);
				osWAIT(100);
				Seg7HP(0x25);
				osWAIT(100);
				Seg7HP(0x26);
				osWAIT(100);
				Seg7HP(0x21);
				osWAIT(100);
				Seg7HP(0x22);
			}while(TaskFlashCmd!=TF_IDLE);		// spin the LED until everything has been erased
			Seg7HP(' ');
			osWAIT(250);
			osENABLE_TASK(TaskAcq);	// now we're empty and ready to acquire
			Flags.AcqActive = 1;
		}
	} // if(!USBPresent())

	else{ // USBPresent(), so it was a USB connection that powered us up. Enable TaskUSB and TaskFlash. If we have a buffer error condition, the PC will handle it.
		osENABLE_TASK(TaskUSB);
	}

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

	//ElapsedMinutes = 1; // test

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



		if(ElapsedMinutes){	// if ElapsedMinutes was set by a successfull acquisition, we must keep advancing it

			MainPowerOn();	// stay on to mark time
//			AnalogPowerOff();

			// While we are marking time, it's possible that we'll be pluggedinto/unplugged from USB. We have to adjust our clock speed to accommodate that

			if(!USBPresent() && CLKSYS_IsReady(OSC_PLLRDY_bm) ){	// if we're not connected to USB (which supplies power) and we're running fast
//			MainPowerOn();
				if(acqState != FSM_FULL_ON) {
					LOWPOWER_Init();	// enter lowest power operating mode to conserve battery energy
					SlowClock();
					AnalogPowerOff();
					FlashPowerOff();
					InitUSBDetect();
					InitButton();
				}
			}

			if( USBPresent() && !CLKSYS_IsReady(OSC_PLLRDY_bm) ){ // we're on USB power and not running at high speed, switch to high speed
				acqState = FSM_START;
				
				FastClock();
				FlashPowerOn();
				InitUSBUART();
			}

			// now that we're running at the right clock speed for the circumstances, we can take care of ElapsedMinutes
			if(RTC.INTFLAGS & RTC_OVFIF_bm){	// if the RTC overflowed
				RTC.INTFLAGS = RTC_OVFIF_bm;		// clear the flag
				if(ElapsedMinutes<0xffff){
					ElapsedMinutes++;							// and increment ElapsedMinutes
				}
			}
		}	// if(ElapsedMinutes())

		// Now we've got to handle the 7-segment display
		// If we're powered by USB, we can be 'F'ull, 'C'harging or 'E'mpty
		if(USBPresent()){
			if(EE_Rd( EE_VOICE_RECORD_LENGTH)!=0 || EE_Rd(EE_ECG_RECORD_LENGTH)!=0){
				Seg7LP('F');
			}
			if(EE_Rd( EE_VOICE_RECORD_LENGTH)==0 && EE_Rd(EE_ECG_RECORD_LENGTH)==0 && BatteryIsCharging() ){
				Seg7LP('C');
			}
			if(EE_Rd( EE_VOICE_RECORD_LENGTH)==0 && EE_Rd(EE_ECG_RECORD_LENGTH)==0 && BatteryIsCharged() ){
				Seg7LP('E');
			}
		}
		// if we're not powered by USB and TaskAcq is not running, then show 'F'ull or 'E'mpty or "L" while the button is pressed and nothing otherwise
		// if TaskAcq is running, it will handle the button, the display, and power
		else if(!Flags.AcqActive){
			if(!ElapsedMinutes){
				MainPowerOff();	// TaskAcq isn't running and ElapsedMinutes is zero (so even if we've recorded something, we can't maintain it)... so turn off when the button is released
			}
			if(ButtonPressed()){ // if the button is pressed and TaskAcq is running (and therefore not handling the button), we'll display E or L if we're able to record and F if not
				if( EE_Rd(EE_VOICE_RECORD_LENGTH)==0 && EE_Rd(EE_ECG_RECORD_LENGTH)==0 ){ // Flash is ready to acquire, see if the battery voltage is high enough to get through a recording
					Seg7LP('E');
					acqState = FSM_EMPTY;
				}
				else{	// Flash is full, we can't record
					Seg7LP('F');
					
					if(!CLKSYS_IsReady(OSC_PLLRDY_bm) && acqState != FSM_FULL_ON) {
						FastClock();
						FlashPowerOn();
						HIGHPOWER_Init();
						acqState = FSM_FULL_ON;
					}
					else if(ElapsedMinutes && acqState == FSM_FULL_ON) {
						AnalogPowerOn();
						
						osENABLE_TASK(TaskBT);
						
						readyForCommands = 1; // We know for sure the chip is ready
						BTOn(); // Just call BTOn since the BT chip is for sure set up
						
						buttonTimer = 0;
						while(ButtonPressed() && acqState != FSM_DONE) {
							osWAIT(1);
							if(++buttonTimer >= 5000) {
								BTOff();
								BTEnable = 0;
								unsigned char EraseInProgress = 0;
														
								MainPowerOn();			// just in case we lose USB
								EraseInProgress=1;	// keep auto power off from tripping if we lose USB
														
								if(TaskFlashCmd == TF_IDLE){
									TaskFlashCmd = TF_ERASE_ALL;
									do{
										osWAIT(100);
										Seg7HP(0x23);
										osWAIT(100);
										Seg7HP(0x24);
										osWAIT(100);
										Seg7HP(0x25);
										osWAIT(100);
										Seg7HP(0x26);
										osWAIT(100);
										Seg7HP(0x21);
										osWAIT(100);
										Seg7HP(0x22);
									}while(TaskFlashCmd != TF_IDLE);	// wait for erase to complete
									ElapsedMinutes = 0;
									Seg7HP(' ');
															
									EraseInProgress=0;	// all auto power off on loss of USB
									acqState = FSM_DONE;
								}
							}
						}
						
						buttonTimer = 0;
						while(!BTEnable && !ButtonPressed() && acqState != FSM_DONE) {
							osWAIT(1);
							if(++buttonTimer >= 30000)
								break;
						}

						// We made a connection while in Full
						if(BTEnable) {
							while(!ButtonPressed() && acqState != FSM_DONE && !USBPresent()) {
								osWAIT(1);
							}
						}
						// Wait for button bounce
						buttonTimer = 0;
						while(ButtonPressed() && !USBPresent()) {
							osWAIT(1);
							if(++buttonTimer >= 5000) {
								BTOff();
								BTEnable = 0;
								unsigned char EraseInProgress = 0;
														
								MainPowerOn();			// just in case we lose USB
								EraseInProgress=1;	// keep auto power off from tripping if we lose USB
														
								if(TaskFlashCmd == TF_IDLE){
									TaskFlashCmd = TF_ERASE_ALL;
									do{
										osWAIT(100);
										Seg7HP(0x23);
										osWAIT(100);
										Seg7HP(0x24);
										osWAIT(100);
										Seg7HP(0x25);
										osWAIT(100);
										Seg7HP(0x26);
										osWAIT(100);
										Seg7HP(0x21);
										osWAIT(100);
										Seg7HP(0x22);
									}while(TaskFlashCmd != TF_IDLE);	// wait for erase to complete
									ElapsedMinutes = 0;
									Seg7HP(' ');
															
									EraseInProgress=0;	// all auto power off on loss of USB
									acqState = FSM_DONE;
								}
							}
						}
						
						
						osDISABLE_TASK(TaskBT);
						
						BTOff();
						BTEnable = 0;
						
						transferingFullData = 0; // In case we were in the middle of a transmission
						transferingDataType = FULL_TRANSFER_NONE;
						
						acqState = FSM_DONE;
					}					
				}
			}
			else{ // the button is released and we're still running, so blank the display (it draws a lot of power)
				Seg7LP(' ');
			}
		}
	} // while(1)

END_TASK

// 16KHz main interrupt. Drives acquisition and SysTick
ISR(TCC1_OVF_vect){
//	DPLEDOn();
	static char i;
	AcqInterrupt();
	if( (i++&0xf)==0){
		SysTick();
	}
//	DPLEDOff();
}
