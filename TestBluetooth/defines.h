#ifdef BOOTLOADER
#ifndef	DEFINES

#include <avr/io.h>	
#include "avr_compiler.h"

//#define LARGE_MEMORY


// baud rate register value calculation		
#define	BPS9600	 12 // 9600 bps at 2MHz
//#define	BRREG_VALUE	 51 // 38400 bps at 32MHz
//#define	BRREG_VALUE	 25 // 76800 bps at 32MHz

// definitions for UART control		
#define UART_PORT				    		PORTF
#define MY_UART									USARTF0
#define UART_TX_PIN				    	PIN3_bm
#define	ENABLE_TRANSMITTER_BIT	USART_TXEN_bp
#define	ENABLE_RECEIVER_BIT			USART_RXEN_bp
#define	TRANSMIT_COMPLETE_BIT		USART_TXCIF_bp
#define	DATA_REG_EMPTY_BIT		  USART_DREIF_bp
#define	RECEIVE_COMPLETE_BIT		USART_RXCIF_bp

// definitions for SPM control
#define	SPMCR_REG	NVM.CTRLB
#define	PAGESIZE	PROGMEM_PAGE_SIZE
#define	APP_END		(APP_SECTION_START+APP_SECTION_SIZE)

// EEPROM definitions
#define EEPROM_NO_PAGES 					EEPROM_SIZE/EEPROM_PAGE_SIZE
#define EEPROM_BYTES_IN_PAGE 			EEPROM_PAGE_SIZE
#define EEPROM_BYTE_ADDRESS_MASK (EEPROM_PAGE_SIZE-1)// Masking out the byte address in a page

#define	DEFINES
#endif	
#endif