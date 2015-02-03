#ifndef BOOTLOADER

#include "avr_compiler.h"
#include "TC_driver.h"

// Configure clock source for the Timer/Counter
void TC0_ConfigClockSource( volatile TC0_t * tc, TC_CLKSEL_t clockSelection ){
	tc->CTRLA = ( tc->CTRLA & ~TC0_CLKSEL_gm ) | clockSelection;
}

void TC1_ConfigClockSource( volatile TC1_t * tc, TC_CLKSEL_t clockSelection ){
	tc->CTRLA = ( tc->CTRLA & ~TC1_CLKSEL_gm ) | clockSelection;
}


// Sets a new WGM setting according to the wgm parameter.
void TC0_ConfigWGM( volatile TC0_t * tc, TC_WGMODE_t wgm ){
	tc->CTRLB = ( tc->CTRLB & ~TC0_WGMODE_gm ) | wgm;
}

void TC1_ConfigWGM( volatile TC1_t * tc, TC_WGMODE_t wgm ){
	tc->CTRLB = ( tc->CTRLB & ~TC1_WGMODE_gm ) | wgm;
}


// Sets the TC for input capture mode and selects the event lines that trigger the individual input channels.
// NOTE Output compare operation is disabled when input capture operation is enabled.
void TC0_ConfigInputCapture( volatile TC0_t * tc, TC_EVSEL_t eventSource ){
	tc->CTRLD = ( tc->CTRLD & ~( TC0_EVSEL_gm | TC0_EVACT_gm ) ) | eventSource | TC_EVACT_CAPT_gc;
}

void TC1_ConfigInputCapture( volatile TC1_t * tc, TC_EVSEL_t eventSource ){
	tc->CTRLD = ( tc->CTRLD & ~( TC1_EVSEL_gm | TC1_EVACT_gm ) ) | eventSource | TC_EVACT_CAPT_gc;
}


/*! \brief Enables compare/capture channels for Timer/Counter 0.
 *
 *  This function enables compare/capture channels according to the supplied
 *  enableMask. The enableMask can be obtained by ORing together the symbols
 *    - TC0_CCAEN_bm
 *    - TC0_CCBEN_bm
 *    - TC0_CCCEN_bm
 *    - TC0_CCDEN_bm
 *
 *  Example: TC0_EnableCCChannels( &TCC0, TC0_CCAEN_bm | TC0_CCCEN_bm ) will
 *  enable channels A and C.
 *
 *  \note No capture/compare channels are disabled by calling this function.
 *  Disabling is done by calling \ref TC0_DisableCCChannels.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param enableMask       Mask of channels to enable.
 */
void TC0_EnableCCChannels( volatile TC0_t * tc, uint8_t enableMask )
{
	/* Make sure only CCxEN bits are set in enableMask. */
	enableMask &= ( TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm );

	/* Enable channels. */
	tc->CTRLB |= enableMask;
}

/*! \brief Enables compare/capture channels for Timer/Counter 1.
 *
 *  This function enables compare/capture channels according to the supplied
 *  enableMask. The enableMask can be obtained by ORing together the symbols
 *    - TC1_CCAEN_bm
 *    - TC1_CCBEN_bm
 *
 *  Example: TC1_EnableCCChannels( &TCC1, TC1_CCAEN_bm | TC1_CCBEN_bm ) will
 *  enable channels A and B.
 *
 *  \note No capture/compare channels are disabled by calling this function.
 *  Disabling is done by calling \ref TC1_DisableCCChannels.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param enableMask       Mask of channels to enable.
 */
void TC1_EnableCCChannels( volatile TC1_t * tc, uint8_t enableMask )
{
	/* Make sure only CCxEN bits are set in enableMask. */
	enableMask &= ( TC1_CCAEN_bm | TC1_CCBEN_bm );

	/* Enable channels. */
	tc->CTRLB |= enableMask;
}


/*! \brief Disables compare/capture channels on Timer/Counter 0.
 *
 *  This function disables compare/capture channels according to the supplied
 *  disableMask. The disableMask can be obtained by ORing together the symbols
 *    - TC0_CCAEN_bm
 *    - TC0_CCBEN_bm
 *    - TC0_CCCEN_bm
 *    - TC0_CCDEN_bm
 *
 *  Example: TC0_DisableCCChannels( &TCC0, TC0_CCAEN_bm | TC0_CCCEN_bm ) will
 *  disable channels A and C.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param disableMask      Mask of channels to disable.
 */
void TC0_DisableCCChannels( volatile TC0_t * tc, uint8_t disableMask )
{
	/* Make sure only CCxEN bits are set in disableMask. */
	disableMask &= ( TC0_CCAEN_bm | TC0_CCBEN_bm | TC0_CCCEN_bm | TC0_CCDEN_bm );

	/* Disable channels. */
	tc->CTRLB &= ~disableMask;
}


/*! \brief Disables compare/capture channels on Timer/Counter 1.
 *
 *  This function disables compare/capture channels according to the supplied
 *  disableMask. The disableMask can be obtained by ORing together the symbols
 *    - TC1_CCAEN_bm
 *    - TC1_CCBEN_bm
 *
 *  Example: TC1_DisableCCChannels( &TCC1, TC1_CCAEN_bm | TC1_CCBEN_bm ) will
 *  disable channels A and B.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param disableMask      Mask of channels to disable.
 */
void TC1_DisableCCChannels( volatile TC1_t * tc, uint8_t disableMask )
{
	/* Make sure only CCxEN bits are set in disableMask. */
	disableMask &= ( TC1_CCAEN_bm | TC1_CCBEN_bm );

	/* Disable channels. */
	tc->CTRLB &= ~disableMask;
}

/*! \brief Sets the overflow interrupt level.
 *
 *  This function sets the overflow interrupt level of this Timer/Counter 0.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param intLevel         New overflow interrupt level.
 */
void TC0_SetOverflowIntLevel( volatile TC0_t * tc, TC_OVFINTLVL_t intLevel )
{
	tc->INTCTRLA = ( tc->INTCTRLA & ~TC0_OVFINTLVL_gm ) | intLevel;
}


/*! \brief Sets the overflow interrupt level.
 *
 *  This function sets the overflow interrupt level of this Timer/Counter 1.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param intLevel         New overflow interrupt level.
 */
void TC1_SetOverflowIntLevel( volatile TC1_t * tc, TC_OVFINTLVL_t intLevel )
{
	tc->INTCTRLA = ( tc->INTCTRLA & ~TC1_OVFINTLVL_gm ) | intLevel;
}


/*! \brief Sets the Error interrupt level.
 *
 *  This function sets the overflow interrupt level of this Timer/Counter 0.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param intLevel         New error interrupt level.
 */
void TC0_SetErrorIntLevel( volatile TC0_t * tc, TC_ERRINTLVL_t intLevel )
{
	tc->INTCTRLA = ( tc->INTCTRLA & ~TC0_ERRINTLVL_gm ) | intLevel;
}


/*! \brief Sets the Error interrupt level.
 *
 *  This function sets the overflow interrupt level of this Timer/Counter 1.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param intLevel         New error interrupt level.
 */
void TC1_SetErrorIntLevel( volatile TC1_t * tc, TC_ERRINTLVL_t intLevel )
{
	tc->INTCTRLA = ( tc->INTCTRLA & ~TC1_ERRINTLVL_gm ) | intLevel;
}


/*! \brief Sets the interrupt level for compare/capture channel A interrupt.
 *
 *  This function sets the interrupt level for compare/capture channel A
 *  interrupt in Timer/Counter 0.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param intLevel         New compare/capture channel A interrupt level.
 */
void TC0_SetCCAIntLevel( volatile TC0_t * tc, TC_CCAINTLVL_t intLevel )
{
	tc->INTCTRLB = ( tc->INTCTRLB & ~TC0_CCAINTLVL_gm ) | intLevel;
}


/*! \brief Sets the interrupt level for compare/capture channel A interrupt.
 *
 *  This function sets the interrupt level for compare/capture channel A
 *  interrupt in Timer/Counter 1.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param intLevel         New compare/capture channel A interrupt level.
 */
void TC1_SetCCAIntLevel( volatile TC1_t * tc, TC_CCAINTLVL_t intLevel )
{
	tc->INTCTRLB = ( tc->INTCTRLB & ~TC1_CCAINTLVL_gm ) | intLevel;
}


/*! \brief Sets the interrupt level for compare/capture channel B interrupt.
 *
 *  This function sets the interrupt level for compare/capture channel B
 *  interrupt in Timer/Counter 0.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param intLevel         New compare/capture channel B interrupt level.
 */
void TC0_SetCCBIntLevel( volatile TC0_t * tc, TC_CCBINTLVL_t intLevel )
{
	tc->INTCTRLB = ( tc->INTCTRLB & ~TC0_CCBINTLVL_gm ) | intLevel;
}


/*! \brief Sets the interrupt level for compare/capture channel B interrupt.
 *
 *  This function sets the interrupt level for compare/capture channel B
 *  interrupt in Timer/Counter 1.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param intLevel         New compare/capture channel B interrupt level.
 */
void TC1_SetCCBIntLevel( volatile TC1_t * tc, TC_CCBINTLVL_t intLevel )
{
	tc->INTCTRLB = ( tc->INTCTRLB & ~TC1_CCBINTLVL_gm ) | intLevel;
}


/*! \brief Sets the interrupt level for compare/capture channel C interrupt.
 *
 *  This function sets the interrupt level for compare/capture channel C
 *  interrupt in Timer/Counter 0.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param intLevel         New compare/capture channel A interrupt level.
 */
void TC0_SetCCCIntLevel( volatile TC0_t * tc, TC_CCCINTLVL_t intLevel )
{
	tc->INTCTRLB = ( tc->INTCTRLB & ~TC0_CCCINTLVL_gm ) | intLevel;
}


/*! \brief Sets the interrupt level for compare/capture channel D interrupt.
 *
 *  This function sets the interrupt level for compare/capture channel D
 *  interrupt in Timer/Counter 0.
 *
 *  \param tc               Timer/Counter module instance.
 *  \param intLevel         New compare/capture channel A interrupt level.
 */
void TC0_SetCCDIntLevel( volatile TC0_t * tc, TC_CCDINTLVL_t intLevel )
{
	tc->INTCTRLB = ( tc->INTCTRLB & ~TC0_CCDINTLVL_gm ) | intLevel;
}


/*! \brief Resets the Timer/Counter 0.
 *
 *  This function will reset the Timer/Counter. After calling this function,
 *  the Timer/Counter will be in the same state as it would after a full
 *  reset of the device.
 *
 *  \param tc  Timer/Counter 0 module instance.
 */
void TC0_Reset( volatile TC0_t * tc )
{
	/* TC must be turned off before a Reset command. */
	tc->CTRLA = ( tc->CTRLA & ~TC0_CLKSEL_gm ) | TC_CLKSEL_OFF_gc;

	/* Issue Reset command. */
	tc->CTRLFSET = TC_CMD_RESET_gc;
}


/*! \brief Resets the Timer/Counter 1.
 *
 *  This function will reset the Timer/Counter. After calling this function,
 *  the Timer/Counter will be in the same state as it would after a full
 *  reset of the device.
 *
 *  \param tc  Timer/Counter 1 module instance.
 */
void TC1_Reset( volatile TC1_t * tc )
{
	/* TC must be turned off before a Reset command. */
	tc->CTRLA = ( tc->CTRLA & ~TC1_CLKSEL_gm ) | TC_CLKSEL_OFF_gc;

	/* Issue Reset command. */
	tc->CTRLFSET = TC_CMD_RESET_gc;
}
#endif