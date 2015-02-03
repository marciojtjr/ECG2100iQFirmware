
#ifndef BOOTLOADER
#ifndef WDT_DRIVER_H
#define WDT_DRIVER_H


#include "avr_compiler.h"


/* Definition of macros */

/*! \brief Check if Synchronization busy flag is set. */
#define WDT_IsSyncBusy() ( WDT.STATUS & WDT_SYNCBUSY_bm )

/*! \brief This macro resets the Watchdog Timer. */
#define WDT_Reset()	asm("wdr") //( watchdog_reset( ) )


/* Prototypes of funtions. */

void WDT_Enable( void );
void WDT_EnableAndSetTimeout( WDT_PER_t period );
void WDT_Disable( void );
bool WDT_IsWindowModeEnabled( void );
bool WDT_EnableWindowMode( void );
bool WDT_EnableWindowModeAndSetTimeout( WDT_WPER_t period );
void WDT_DisableWindowMode( void );

#endif
#endif