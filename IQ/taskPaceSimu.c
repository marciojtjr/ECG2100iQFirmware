/*
 * taskPaceSimu.c
 *
 * Created: 13-Sep-13 10:40:28
 *  Author: snobaste
 */ 
#ifndef BOOTLOADER

#include <avr/io.h>

#include "taskPaceSimu.h"
#include "taskBT.h"
#include "avr_compiler.h"
#include "spi_driver.h"
#include "rtos.h"
#include "iq.h"
#include "taskAcq.h"
#include "taskFlash.h"
#include "buffers.h"
#include "eeprom.h"
#include "taskUSB.h"
#include "TaskWatchDog.h"

#ifdef WORKING
BEGIN_TASK(TaskPaceSimu)
	
	
	PORT_PACE_DETECT.DIR |= PACE_DETECT_L1;
	PORT_PACE_DETECT.PIN3CTRL = 0;
	while(1) {
		PORT_PACE_DETECT.DIRSET = PACE_DETECT_L1; PORT_PACE_DETECT.OUTSET = PACE_DETECT_L1; PORT_PACE_DETECT.PIN3CTRL = PORT_OPC_TOTEM_gc;
		//PORT_PACE_DETECT.OUT |= PACE_DETECT_L2;
		//paceSpike = 1;
		osWAIT(4);
		//PORT_PACE_DETECT.OUT &= ~PACE_DETECT_L2;
		PORT_PACE_DETECT.DIRSET = PACE_DETECT_L1; PORT_PACE_DETECT.OUTCLR = PACE_DETECT_L1; PORT_PACE_DETECT.PIN3CTRL = PORT_OPC_TOTEM_gc;
		//paceSpike = 0;
		osWAIT(6);
		//paceSpike = 1;
		PORT_PACE_DETECT.DIRSET = PACE_DETECT_L1; PORT_PACE_DETECT.OUTSET = PACE_DETECT_L1; PORT_PACE_DETECT.PIN3CTRL = PORT_OPC_TOTEM_gc;
		osWAIT(4);
		//paceSpike = 0;
		PORT_PACE_DETECT.DIRSET = PACE_DETECT_L1; PORT_PACE_DETECT.OUTCLR = PACE_DETECT_L1; PORT_PACE_DETECT.PIN3CTRL = PORT_OPC_TOTEM_gc;
		osWAIT(250);
		osWAIT(250);
		osWAIT(250);
		osWAIT(250);
	}
	
END_TASK
#endif
#endif