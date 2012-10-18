#ifndef TC_DRIVER_H
#define TC_DRIVER_H

#include "avr_compiler.h"

// Note: Channels C/D not available on TCx1.

// Macros

// Enable a one clock cycle delay of event sources for this TC.
// Mainly used for the high word TC when two TCs are cascaded to form a 32-bit TC.
#define TC_EnableEventDelay( _tc )  ( (_tc)->CTRLD |= TC0_EVDLY_bm )

//  Disable the event delay for this TC
#define TC_DisableEventDelay( _tc ) ( (_tc)->CTRLD &= ~TC0_EVDLY_bm )

// Lock automatic updates of compare registers from the corresponding buffer registers.
// To enable automatic updates again, use the TC_UnlockCompareUpdate macro. */
#define TC_LockCompareUpdate( _tc ) ( (_tc)->CTRLFSET = TC0_LUPD_bm )

// Unlock automatic updates of compare registers from the
// corresponding buffer registers. Note that the output compare registers will
// not be updated until an update event occurs, e.g. that the counter hits
// TOP or BOTTOM. To force an update of the compare register from the buffer
// register, use the TC_ForceUpdate macro
#define TC_UnlockCompareUpdate( _tc ) ( (_tc)->CTRLFCLR = TC0_LUPD_bm )


// Trigger a transfer from the output compare and period buffer registers ( CCxBUF and PERBUF )
// to the output compare and period registers ( CCx and PER ).
// Calling this macro has the same effect as an update condition
#define TC_ForceUpdate( _tc ) ( (_tc)->CTRLFSET = TC_CMD_UPDATE_gc )

// Restart the timer. The effect of running this command is:
//    - The CNT[H:L] register is cleared.
//    - The direction (DIR ) is reset. (Next clock cycle will increase the counter )
//    - All compare outputs are set to 0.
//    - If a DTI module is connected to the timer, it will be reset as well
#define TC_Restart( _tc ) ( (_tc)->CTRLFSET = TC_CMD_RESTART_gc )

// Write a new value to the CNT[H:L] register.
//  Note that the CNT[H:L] register is not double buffered, so it is recommended
//  that the timer is not running when this macro is used
#define TC_SetCount( _tc, _count ) ( (_tc)->CNT = (_count) )

// Set a new timer period. The period buffer register is not used,
// so the new period will be valid immediately after the 16-bit write is finished
#define TC_SetPeriod( _tc, _period ) ( (_tc)->PER = (_period) )

// Set a new timer period. The period buffer register is used,
// so the new period will be valid after the next update condition. */
#define TC_SetPeriodBuffered( _tc, _period ) ( (_tc)->PERBUF = (_period) )

// Sets/get a compare value for a channel.
// The buffered version ensures new compare values will be valid from the next update condition.
#define TC_SetCompareABuffered( _tc, _compareValue ) ( (_tc)->CCABUF = (_compareValue) )
#define TC_SetCompareA( _tc, _compareValue ) ( (_tc)->CCA = (_compareValue) )
#define TC_GetCompareA( _tc) ( (_tc)->CCA )

#define TC_SetCompareBBuffered( _tc, _compareValue ) ( (_tc)->CCBBUF = (_compareValue) )
#define TC_SetCompareB( _tc, _compareValue ) ( (_tc)->CCB = (_compareValue) )
#define TC_GetCompareB( _tc) ( (_tc)->CCB )

// Note: Channels C/D not available on TCx1.
#define TC_SetCompareCBuffered( _tc, _compareValue ) ( (_tc)->CCCBUF = (_compareValue) )
#define TC_SetCompareC( _tc, _compareValue ) ( (_tc)->CCC = (_compareValue) )
#define TC_GetCompareC( _tc) ( (_tc)->CCC )

#define TC_SetCompareDBuffered( _tc, _compareValue ) ( (_tc)->CCDBUF = (_compareValue) )
#define TC_SetCompareD( _tc, _compareValue ) ( (_tc)->CCD = (_compareValue) )
#define TC_GetCompareD( _tc) ( (_tc)->CCD )


//  Return Non-zero if overflow flag is set, zero otherwise.
#define TC_GetOverflowFlag( _tc ) ( (_tc)->INTFLAGS & TC0_OVFIF_bm )

// Clear the Timer/Counter overflow flag.
#define TC_ClearOverflowFlag( _tc ) ( (_tc)->INTFLAGS = TC0_OVFIF_bm )

// return Non-zero if the Timer error flag is set.
#define TC_GetErrorFlag( _tc ) ( (_tc)->INTFLAGS & TC0_ERRIF_bm )

// Clear the Timer/Counter error flag.
#define TC_ClearErrorFlag( _tc ) ( (_tc)->INTFLAGS = TC0_ERRIF_bm )

// When the TC is in Input Capture mode, the return value of this macro indicates
// whether there is an unread input capture value available for a capture channel

// When a TC is in any other mode, the return value of this macro indicates
// whether there has been a compare match between the channel and counter.
// Return Non-zero if Compare or capture has occured, zero otherwise.

#define TC_GetCCAFlag( _tc ) ( (_tc)->INTFLAGS & TC0_CCAIF_bm )
#define TC_GetCCBFlag( _tc ) ( (_tc)->INTFLAGS & TC0_CCBIF_bm )
#define TC_GetCCCFlag( _tc ) ( (_tc)->INTFLAGS & TC0_CCCIF_bm )
#define TC_GetCCDFlag( _tc ) ( (_tc)->INTFLAGS & TC0_CCDIF_bm )

// Clear the Timer/Counter compare or capture interrupt flag.
#define TC_ClearCCAFlag( _tc ) ( (_tc)->INTFLAGS = TC0_CCAIF_bm )
#define TC_ClearCCBFlag( _tc ) ( (_tc)->INTFLAGS = TC0_CCBIF_bm )
#define TC_ClearCCCFlag( _tc ) ( (_tc)->INTFLAGS = TC0_CCCIF_bm )
#define TC_ClearCCDFlag( _tc ) ( (_tc)->INTFLAGS = TC0_CCDIF_bm )

// Return the first available input value for capture channel
#define TC_GetCaptureA( _tc ) ( (_tc)->CCA )
#define TC_GetCaptureB( _tc ) ( (_tc)->CCB )
#define TC_GetCaptureC( _tc ) ( (_tc)->CCC )
#define TC_GetCaptureD( _tc ) ( (_tc)->CCD )


// Function Prototypes

void TC0_ConfigClockSource( volatile TC0_t * tc, TC_CLKSEL_t clockSelection );
void TC0_ConfigWGM( volatile TC0_t * tc, TC_WGMODE_t wgm );
void TC0_ConfigInputCapture( volatile TC0_t * tc, TC_EVSEL_t eventSource );
void TC0_EnableCCChannels( volatile TC0_t * tc, uint8_t enableMask );
void TC0_DisableCCChannels( volatile TC0_t * tc, uint8_t disableMask );
void TC0_SetOverflowIntLevel( volatile TC0_t * tc, TC_OVFINTLVL_t intLevel );
void TC0_SetErrorIntLevel( volatile TC0_t * tc, TC_ERRINTLVL_t intLevel );
void TC0_SetCCAIntLevel( volatile TC0_t * tc, TC_CCAINTLVL_t intLevel );
void TC0_SetCCBIntLevel( volatile TC0_t * tc, TC_CCBINTLVL_t intLevel );
void TC0_SetCCCIntLevel( volatile TC0_t * tc, TC_CCCINTLVL_t intLevel );
void TC0_SetCCDIntLevel( volatile TC0_t * tc, TC_CCDINTLVL_t intLevel );
void TC0_Reset( volatile TC0_t * tc );

void TC1_ConfigClockSource( volatile TC1_t * tc, TC_CLKSEL_t clockSelection );
void TC1_ConfigWGM( volatile TC1_t * tc, TC_WGMODE_t wgm );
void TC1_ConfigInputCapture( volatile TC1_t * tc, TC_EVSEL_t eventSource );
void TC1_EnableCCChannels( volatile TC1_t * tc, uint8_t enableMask );
void TC1_DisableCCChannels( volatile TC1_t * tc, uint8_t disableMask );
void TC1_SetOverflowIntLevel( volatile TC1_t * tc, TC_OVFINTLVL_t intLevel );
void TC1_SetErrorIntLevel( volatile TC1_t * tc, TC_ERRINTLVL_t intLevel );
void TC1_SetCCAIntLevel( volatile TC1_t * tc, TC_CCAINTLVL_t intLevel );
void TC1_SetCCBIntLevel( volatile TC1_t * tc, TC_CCBINTLVL_t intLevel );
void TC1_SetCCCIntLevel( volatile TC1_t * tc, TC_CCCINTLVL_t intLevel );
void TC1_SetCCDIntLevel( volatile TC1_t * tc, TC_CCDINTLVL_t intLevel );
void TC1_Reset( volatile TC1_t * tc );


#endif
