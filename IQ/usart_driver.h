#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include "avr_compiler.h"

// Transmit and Receive buffers
#define RX_BUF_SIZE 255	// can be any size up to 255
#define TX_BUF_SIZE 128	// must be a power of 2 no larger than 128
#define TX_BUF_MASK ( TX_BUF_SIZE - 1 )

extern int written;

#if ( TX_BUF_SIZE & TX_BUF_MASK )
#error TX_BUF_SIZE is not a power of 2
#endif

// a structure to hold everything of importance for using the USART
typedef struct USART_Descriptor{
	USART_t *usart;// pointer to USART module base address
	USART_DREINTLVL_t dreIntLevel;// data register empty interrupt level
	volatile uint8_t RxPut;
	volatile uint8_t RxReady;	// 0 = program is not ready to receive, 0 = it is
	volatile uint8_t TxPut;
	volatile uint8_t TxGet;
	volatile uint8_t RxBuf[RX_BUF_SIZE];
	volatile uint8_t TxBuf[TX_BUF_SIZE];
	volatile uint8_t TxWaiting;
	volatile uint8_t TxDelivered;
	volatile uint32_t TxTotal;
} USART_desc_t;

//============================================
// Macros
#define USART_Format_Set(_usart, _charSize, _parityMode, _twoStopBits) (_usart)->CTRLC = (uint8_t) _charSize | _parityMode | (_twoStopBits ? USART_SBMODE_bm : 0)

// Set the USART's baud rate register.
#define USART_Baudrate_Set(_usart, _bselValue, _bScaleFactor) (_usart)->BAUDCTRLA =(uint8_t)_bselValue; (_usart)->BAUDCTRLB =(_bScaleFactor << USART_BSCALE0_bp)|(_bselValue >> 8)

#define USART_Rx_Enable(_usart) ((_usart)->CTRLB |= USART_RXEN_bm)
#define USART_Rx_Disable(_usart) ((_usart)->CTRLB &= ~USART_RXEN_bm)
#define USART_Tx_Enable(_usart)	((_usart)->CTRLB |= USART_TXEN_bm)
#define USART_Tx_Disable(_usart) ((_usart)->CTRLB &= ~USART_TXEN_bm)

//  Set interrupt levels for the three USART interrupt sources
#define USART_RxdInterruptLevel_Set(_usart, _rxdIntLevel) ((_usart)->CTRLA = ((_usart)->CTRLA & ~USART_RXCINTLVL_gm) | _rxdIntLevel)
#define USART_TxdInterruptLevel_Set(_usart, _txdIntLevel) ((_usart)->CTRLA = ((_usart)->CTRLA & ~USART_TXCINTLVL_gm) | _txdIntLevel)
#define USART_DreInterruptLevel_Set(_usart, _dreIntLevel) ((_usart)->CTRLA = ((_usart)->CTRLA & ~USART_DREINTLVL_gm) | _dreIntLevel)

// Set the USART operating mode. The default mode is asynchronous mode.
#define USART_SetMode(_usart, _usartMode)   ((_usart)->CTRLC = ((_usart)->CTRLC & (~USART_CMODE_gm)) | _usartMode)

//============================================
// Functions
void USART_InterruptDriver_Initialize(USART_desc_t *usart_desc, USART_t *usart, USART_DREINTLVL_t dreIntLevel );
void USART_RXComplete(USART_desc_t *usart_desc);
void USART_DataRegEmpty(USART_desc_t *usart_desc);
void uart_putchar(USART_desc_t *usart_desc, uint8_t data);

#endif
