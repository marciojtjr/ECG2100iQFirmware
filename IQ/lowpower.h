#ifndef LOWPOWER_H
#define LOWPOWER_H


/*============================= CONFIG =======================================*/
//! Disable JTAG interface. (Comment out to leave it enabled.)
//#define NO_JTAG


/*============================ PROTOTYPES ====================================*/
#ifdef __cplusplus
extern "C" {
#endif
  
void LOWPOWER_Init( void );
void HIGHPOWER_Init( void );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LOWPOWER_H
