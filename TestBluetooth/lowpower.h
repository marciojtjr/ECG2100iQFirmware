#ifndef BOOTLOADER

#ifndef LOWPOWER_H
#define LOWPOWER_H


/*============================= CONFIG =======================================*/
//! Disable JTAG interface. (Comment out to leave it enabled.)
//#define NO_JTAG


/*============================ PROTOTYPES ====================================*/
#ifdef __cplusplus
extern "C" {
#endif

enum {LP_OFF, LP_LOW, LP_FULL, LP_LOWFULL}; 
static unsigned long int lowPowerTimerOffset = 0;

unsigned long int getCurrentLowPowerClockValue();
void LPWAIT(unsigned char ms);

void LOWPOWER_Init( void );
void HIGHPOWER_Init( void );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LOWPOWER_H
#endif