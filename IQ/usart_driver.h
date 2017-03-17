#ifndef BOOTLOADER

#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include "avr_compiler.h"

// Transmit and Receive buffers
#define SMALL_RX_BUF_SIZE 128
#define SMALL_TX_BUF_SIZE 128

#define LARGE_RX_BUF_SIZE 255
#define LARGE_TX_BUF_SIZE 256

#define SMALL_TX_BUF_MASK ( SMALL_TX_BUF_SIZE - 1)
#define LARGE_TX_BUF_MASK ( LARGE_TX_BUF_SIZE - 1)

//#define RX_BUF_SIZE 128	// can be any size up to 255
//#define TX_BUF_SIZE 1024	// must be a power of 2 no larger than 128
//#define TX_BUF_MASK ( TX_BUF_SIZE - 1 )

extern int written;
extern char readyForCommands;

extern uint8_t iniAPMode ;
extern uint8_t waitingForiAPPacket ;
extern int16_t remainingiAPBytes ;
extern uint8_t needToDropPairing;
extern uint8_t ignoreNextRFCOMM;
extern uint8_t RFCOMMConnectionEstablished;
extern uint8_t ignoreNextRxReady;

//#if ( (SMALL_TX_BUF_SIZE & SMALL_TX_BUF_MASK) || (LARGE_TX_BUF_SIZE & LARGE_TX_BUF_MASK) )
//#error TX_BUF_SIZE is not a power of 2
//#endif


// a structure to hold everything of importance for using the USART
typedef struct USART_Small_Descriptor{
	USART_t *usart;// pointer to USART module base address
	USART_DREINTLVL_t dreIntLevel;// data register empty interrupt level
	volatile uint8_t RxPut;
	volatile uint8_t RxReady;	// 0 = program is not ready to receive, 0 = it is
	volatile uint8_t TxPut;
	volatile uint8_t TxGet;
	volatile uint8_t *RxBuf;//RxBuf[RX_BUF_SIZE];
	volatile uint8_t *TxBuf;//TxBuf[TX_BUF_SIZE];
	//volatile uint8_t RxBuf[SMALL_RX_BUF_SIZE];
	//volatile uint8_t TxBuf[SMALL_TX_BUF_SIZE];
	volatile uint8_t TxWaiting;
	volatile uint8_t TxDelivered;

	uint16_t TxBufSize;
	uint16_t RxBufSize;
	uint16_t TxBufMask;
	//volatile uint32_t TxTotal;
} USART_desc_t;

extern uint8_t smallRxBuf[SMALL_RX_BUF_SIZE];
extern uint8_t smallTxBuf[SMALL_TX_BUF_SIZE];

extern uint8_t largeRxBuf[LARGE_RX_BUF_SIZE];
extern uint8_t largeTxBuf[LARGE_TX_BUF_SIZE];

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


/*
void USART_Large_InterruptDriver_Initialize(USART_large_desc_t *usart_desc, USART_t *usart, USART_DREINTLVL_t dreIntLevel );
void USART_Large_RXComplete(USART_large_desc_t *usart_desc);
void USART_Large_DataRegEmpty(USART_large_desc_t *usart_desc);
void uart_large_putchar(USART_large_desc_t *usart_desc, uint8_t data);
*/

//void USART_DataRegEmptyNotInterrupt(USART_desc_t *usart_desc);




void resetUsartAppleVariables();

#endif
#endif