#ifndef BOOTLOADER

#include "avr_compiler.h"
#include "lowpower.h"

#include "rtc_driver.h"

//! Convenience macro for enabling pull-ups on specified pins on any port.
#define __PORT_PULLUP(port, mask) { \
	PORTCFG.MPCMASK = mask ; \
	port.PIN0CTRL = PORT_OPC_PULLUP_gc; \
}

//! Convenience macro for enabling pull-downs on specified pins on any port.
#define __PORT_PULLDOWN(port, mask) { \
	PORTCFG.MPCMASK = mask ; \
	port.PIN0CTRL = PORT_OPC_PULLDOWN_gc; \
}

#define ENABLE_PULLUP( ) { \
	__PORT_PULLUP(PORTA, 0xFF); \
	__PORT_PULLUP(PORTB, 0xFF); \
	__PORT_PULLUP(PORTC, 0xFF); \
	__PORT_PULLUP(PORTD, 0xFF); \
	__PORT_PULLUP(PORTE, 0xFF); \
	__PORT_PULLUP(PORTF, 0xFF); \
	__PORT_PULLUP(PORTR, 0x03); \
}

#define ENABLE_PULLDOWN( ) { \
	__PORT_PULLDOWN(PORTA, 0xFF); \
	__PORT_PULLDOWN(PORTB, 0xFF); \
	__PORT_PULLDOWN(PORTC, 0xFF); \
	__PORT_PULLDOWN(PORTD, 0xFF); \
	__PORT_PULLDOWN(PORTE, 0xFF); \
	__PORT_PULLDOWN(PORTF, 0xFF); \
	__PORT_PULLDOWN(PORTR, 0x03); \
}

/*
void LOWPOWER_Init( void ){

// Disable encryption, DMA and event system
//PR.PRGEN |= PR_AES_bm | PR_DMA_bm | PR_EVSYS_bm | PR_RTC_bm;
	PR.PRGEN |= PR_AES_bm | PR_DMA_bm | PR_EVSYS_bm;

// Disable all timers except Port C
//PR.PRPC |= PR_HIRES_bm | PR_TC0_bm | PR_TC1_bm;
	PR.PRPD |= PR_HIRES_bm | PR_TC0_bm | PR_TC1_bm;
	PR.PRPE |= PR_HIRES_bm | PR_TC0_bm | PR_TC1_bm;
	PR.PRPF |= PR_HIRES_bm | PR_TC0_bm;

// Disable all comm ports except USART on PORTF and SPI on PORTD
	PR.PRPC |= PR_SPI_bm | PR_TWI_bm | PR_USART0_bm | PR_USART1_bm;
	PR.PRPD |= PR_USART0_bm | PR_USART1_bm;
//PR.PRPD |= PR_SPI_bm | PR_USART0_bm | PR_USART1_bm;
	PR.PRPE |= PR_SPI_bm | PR_TWI_bm | PR_USART0_bm | PR_USART1_bm;
//PR.PRPF |= PR_USART0_bm;

// Disable analog ports
	PR.PRPA |= PR_AC_bm | PR_ADC_bm;
	PR.PRPB |= PR_AC_bm | PR_ADC_bm | PR_DAC_bm;

//!!!!!	CCPWrite(&NVM.CTRLB, NVM_EPRM_bm | NVM_FPRM_bm); // Enable EEPROM and Flash power reduction mode.

//	ENABLE_PULLUP();
//	ENABLE_PULLDOWN();
}
*/

void HIGHPOWER_Init( void ) {
// Enable encryption, DMA and event system
	PR.PRGEN = 0;

// Enable all timers except Port C
	PR.PRPD = 0;
	PR.PRPE = 0;
	PR.PRPF = 0;

// Enable all comm ports except USART on PORTF and SPI on PORTD
	PR.PRPC = 0;
	PR.PRPD = 0;
	PR.PRPE = 0;

// Enable analog ports
	PR.PRPA = 0;
	PR.PRPB = 0;
}

#endif