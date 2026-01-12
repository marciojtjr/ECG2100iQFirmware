#ifndef BOOTLOADER

#include <avr/io.h>
#include "avr_compiler.h"
#include "rtos.h"
#include "iq.h"
#include "TaskWatchDog.h"
#include "taskUSB.h"

//------------------------------------------------------------------------------------------
// Maintain the watchdog here. We do it in a task rather than in the scheduler so that if
// any task hangs, this task will die.
// Hangs can occur because hardware (internal or external) hangs, or because program flow is interrupted by an electrical upset
// Some hangs cannot be detected. For example, failure to receive a USB command can be caused either
// by a manfunction in the UART or by the host PC simply not sending commands.

// In addition to detecting a task stall, we allow tasks to set their own WatchTimers. Any non-zero watchtimer will be
// decremented once per cycle through this task. If the timer was non-zero and is decremented to zero, this task will hang
// until the processor's watchdog timer forces a hardware reset.

// Any task that sets a watchtimer to monitor some process (like waiting for Flash to erase) should clear it to zero
// on completion of the monitored process. Zeroed watchtimers are not decremented or tested. A watchtimer should
// never be set to 1, as that will force an immediate reset when this task runs.

unsigned char FlashWatchTimer = 0;		// disabled, TaskFlash will set this
unsigned char USBWatchTimer = 0;			// disabled, will be set by TaskUSB
unsigned char AcqWatchTimer = 0;			// disabled, TaskAcq will set this
unsigned int GlobalWatchTimer = 900;	// set a maximum powerup time of 15 minutes, regardless of activity.

BEGIN_TASK(TaskWatchDog) 

	static unsigned char i;
	while(1){

		for(i=0;i<10;++i){	//this loop takes one second to run (10 x 100ms)
			
			if(USBPresent()) {
				DPLEDTog();		// toggle the LED decimal point to indicate we're alive
			}
			osWAIT(100);
			wdt_reset();
		}
	}
END_TASK
#endif