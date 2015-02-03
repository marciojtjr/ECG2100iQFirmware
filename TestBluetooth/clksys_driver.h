#ifndef CLKSYS_DRIVER_H
#define CLKSYS_DRIVER_H

#include "avr_compiler.h"


/* Definitions of macros. */

/*! \brief This macro enables the selected oscillator.
 *
 *  \note Note that the oscillator cannot be used as a main system clock
 *        source without being enabled and stable first. Check the ready flag
 *        before using the clock. The function CLKSYS_IsReady( _oscSel )
 *        can be used to check this.
 *
 *  \param  _oscSel Bitmask of selected clock. Can be one of the following
 *                  OSC_RC2MEN_bm, OSC_RC32MEN_bm, OSC_RC32KEN_bm, OSC_XOSCEN_bm,
 *                  OSC_PLLEN_bm.
 */
#define CLKSYS_Enable( _oscSel ) ( OSC.CTRL |= (_oscSel) )

/*! \brief This macro check if selected oscillator is ready.
 *
 *  This macro will return non-zero if is is running, regardless if it is
 *  used as a main clock source or not.
 *
 *  \param _oscSel Bitmask of selected clock. Can be one of the following
 *                 OSC_RC2MEN_bm, OSC_RC32MEN_bm, OSC_RC32KEN_bm, OSC_XOSCEN_bm,
 *                 OSC_PLLEN_bm.
 *
 *  \return  Non-zero if oscillator is ready and running.
 */
#define CLKSYS_IsReady( _oscSel ) ( OSC.STATUS & (_oscSel) )

/*! \brief This macro disables routing of clock signals to the Real-Time
 *         Counter (RTC).
 *
 *  Disabling the RTC saves power if the RTC is not in use.
 */
#define CLKSYS_RTC_ClockSource_Disable() ( CLK.RTCCTRL &= ~CLK_RTCEN_bm )

/*! \brief This macro disables the automatic calibration of the selected
 *         internal oscillator.
 *
 *  \param _clk  Clock source calibration to disable, either DFLLRC2M or DFLLRC32M.
 */
#define CLKSYS_AutoCalibration_Disable( _clk ) ( (_clk).CTRL &= ~DFLL_ENABLE_bm )


/* Prototyping of function. Detailed information is found in source file. */
void CLKSYS_XOSC_Config( OSC_FRQRANGE_t freqRange,
                         bool lowPower32kHz,
                         OSC_XOSCSEL_t xoscModeSelection );
void CLKSYS_PLL_Config( OSC_PLLSRC_t clockSource, uint8_t factor );
uint8_t CLKSYS_Disable( uint8_t oscSel );
void CLKSYS_Prescalers_Config( CLK_PSADIV_t PSAfactor,
                               CLK_PSBCDIV_t PSBCfactor );
uint8_t CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_t clockSource );
void CLKSYS_RTC_ClockSource_Enable( CLK_RTCSRC_t clockSource );
void CLKSYS_AutoCalibration_Enable( uint8_t clkSource, bool extReference );
void CLKSYS_XOSC_FailureDetection_Enable( void );
void CLKSYS_Configuration_Lock( void );


#endif
