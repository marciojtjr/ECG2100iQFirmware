/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief  XMEGA Watchdog driver source file.
 *
 *      This file contains the function implementations for the XMEGA Watchdog driver.
 *
 *      The driver is not intended for size and/or speed critical code, since
 *      most functions are just a few lines of code, and the function call
 *      overhead would decrease code performance. The driver is intended for
 *      rapid prototyping and documentation purposes for getting started with
 *      the XMEGA Watchdog module.
 *
 *      For size and/or speed critical code, it is recommended to copy the
 *      function contents directly into your application instead of making
 *      a function call. Another way is to define all the functions as macros.
 *
 * \par Application note:
 *      AVR1310: Using the XMEGA Watchdog
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * $Revision: 2303 $
 * $Date: 2009-04-16 14:47:58 +0200 (to, 16 apr 2009) $  \n
 *****************************************************************************/
#include "wdt_driver.h"



/*! \brief Enable Watchdog without changing prescaler settings.
 *
 *  This function enables the Watchdog without changing prescaler settings.
 *  The Watchdog will be reset automatically.
 *
 *  The function writes the correct signature to the Configuration
 *  Change Protection register before writing the CTRL register. Interrupts are
 *  automatically ignored during the change enable period. The function will
 *  wait for the watchdog to be synchronized to the other clock domains. before
 *  proceeding
 */
void WDT_Enable( void )
{
	uint8_t temp = WDT.CTRL | WDT_ENABLE_bm | WDT_CEN_bm;
	CCP = CCP_IOREG_gc;
	WDT.CTRL = temp;

	/* Wait for WD to synchronize with new settings. */
	while(WDT_IsSyncBusy()){

	}
}



/*! \brief Enable Watchdog and set prescaler.
 *
 *  This function enables the Watchdog and applies prescaler settings.
 *  The Watchdog will be reset automatically.
 *
 *  The function writes the correct signature to the Configuration
 *  Change Protection register before writing the CTRL register. Interrupts are
 *  automatically ignored during the change enable period. TThe function will
 *  wait for the watchdog to be synchronized to the other clock domains. before
 *  proceeding
 *
 *  \param  period  Watchdog Timer timeout period
 */
void WDT_EnableAndSetTimeout( WDT_PER_t period )
{
	uint8_t temp = WDT_ENABLE_bm | WDT_CEN_bm | period;
	CCP = CCP_IOREG_gc;
	WDT.CTRL = temp;

	/* Wait for WD to synchronize with new settings. */
	while(WDT_IsSyncBusy()){

	}
}



/*! \brief Disable Watchdog without changing prescaler settings.
 *
 *  This function disables the Watchdog without changing the prescaler settings.
 *  If it is enabled at a later time, the prescaler setting can be reused.
 *  Note that the Watchdog can only be disabled in Safety Level 1.
 *
 *  The function writes the correct signature to the Configuration
 *  Change Protection register before writing the CTRL register. Interrupts are
 *  automatically ignored during the change enable period. Disable functions
 *  operate asynchronously with immidiate effect.
 */
void WDT_Disable( void )
{
	uint8_t temp = (WDT.CTRL & ~WDT_ENABLE_bm) | WDT_CEN_bm;
	CCP = CCP_IOREG_gc;
	WDT.CTRL = temp;
}


/*! \brief Return status of window mode enable bit.
 *
 *  \retval  true   The WD Window Mode is enabled.
 *  \retval  false  The WD Eindow Mode is not enabled.
 */
bool WDT_IsWindowModeEnabled( void )
{
	uint8_t wdwm_enabled = (WDT.WINCTRL & WDT_WEN_bm);
	return wdwm_enabled;
}

/*! \brief Enable window mode without changing prescaler settings.
 *
 *  This function enables window mode without changing prescaler settings.
 *  The WD must be enabled before enabling window mode and a true value is
 *  returned if this condition is met.
 *
 *  The function writes the correct signature to the Configuration
 *  Change Protection register before writing the WINCTRL register. Interrupts are
 *  automatically ignored during the change enable period. The function will
 *  wait for the watchdog to be synchronized to the other clock domains. before
 *  proceeding.
 *
 *  \retval  true   The WD is enabled before enabling window mode
 *  \retval  false  The WD is not enabled before enabling window mode.
 */
bool WDT_EnableWindowMode( void )
{
	uint8_t wd_enable = WDT.CTRL & WDT_ENABLE_bm;
	uint8_t temp = WDT.WINCTRL | WDT_WEN_bm | WDT_WCEN_bm;
	CCP = CCP_IOREG_gc;
	WDT.WINCTRL = temp;
        
	/* Wait for WD to synchronize with new settings. */
	while(WDT_IsSyncBusy()){

	}
        
	return wd_enable;
}



/*! \brief Enable window mode and set prescaler.
 *
 *  This function enables window mode and applies prescaler settings.
 *  The WD must be enabled before enabling window mode and a true value is
 *  returned if this condition is met.
 *
 *  The function writes the correct signature to the Configuration
 *  Change Protection register before writing the WINCTRL register. Interrupts are
 *  automatically ignored during the change enable period. The function will
 *  wait for the watchdog to be synchronized to the other clock domains. before
 *  proceeding
 *
 *  \param  period  Window mode "closed" timeout period
 *
 *  \retval  true   The WD is enabled before enabling window mode
 *  \retval  false  The WD is not enabled before enabling window mode.
 */
bool WDT_EnableWindowModeAndSetTimeout( WDT_WPER_t period )
{
	uint8_t wd_enable = WDT.CTRL & WDT_ENABLE_bm;
	uint8_t temp = WDT_WEN_bm | WDT_WCEN_bm | period;
	CCP = CCP_IOREG_gc;
	WDT.WINCTRL = temp;
        
	/* Wait for WD to synchronize with new settings. */
	while(WDT_IsSyncBusy()){

	}
        
	return wd_enable;
}


/*! \brief Disable window mode without changing prescaler settings.
 *
 *  This function disables window mode without changing the prescaler settings.
 *  If it is enabled at a later time, the prescaler setting can be reused.
 *  Note that window mode can be disabled in Safety Level 1 and 2.
 *
 *  The function writes the correct signature to the Configuration
 *  Change Protection register before writing the WINCTRL register. Interrupts are
 *  automatically ignored during the change enable period. Disable functions
 *  operate asynchronously with immidiate effect.
 */
void WDT_DisableWindowMode( void )
{
	uint8_t temp = (WDT.WINCTRL & ~WDT_WEN_bm) | WDT_WCEN_bm;
	CCP = CCP_IOREG_gc;
	WDT.WINCTRL = temp;
}
