#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include "usart_driver.h"
#include "iq.h"
#include "wdt_driver.h"
#include "TaskUSB.h"
#include <util/delay.h>

//============================================================
void USART_InterruptDriver_Initialize(USART_desc_t *usart_desc, USART_t *usart, USART_DREINTLVL_t dreIntLevel){
	usart_desc->usart = usart;
	usart_desc->dreIntLevel = dreIntLevel;
	usart_desc->RxPut = 0;
	usart_desc->RxReady = 0;
	usart_desc->TxPut = 0;
	usart_desc->TxGet = 0;
}


//============================================================
// Rx ready interrupt handler
void USART_RXComplete(USART_desc_t *usart_desc){

	uint8_t c = usart_desc->usart->DATA;	// get the byte

	//if(usart_desc->RxReady == 0){ // only receive characters if the last receive buffer has been processed
		if(c=='\r'|| c=='\n'){
			uart_putchar(usart_desc,'\r');
			usart_desc->RxBuf[usart_desc->RxPut] = 0;
			usart_desc->RxPut = 0;
			usart_desc->RxReady = 1;
		}
		else {
			uart_putchar(usart_desc,c);
			if(c=='\b'){	// backspace
				uart_putchar(usart_desc,' ');
				uart_putchar(usart_desc,'\b');
				if(usart_desc->RxPut)usart_desc->RxPut--;
			}
			else {
				usart_desc->RxBuf[usart_desc->RxPut++]=c;
			}
		}
	//}
}

//============================================================
// send a character if possible, wait if not

void uart_putchar(USART_desc_t *usart_desc, uint8_t data){

	while( (( (usart_desc->TxPut)+1 ) & TX_BUF_MASK) == usart_desc->TxGet ); // wait for room

	usart_desc->TxBuf[usart_desc->TxPut]= data;	// put the byte in the buffer
	usart_desc->TxPut = (usart_desc->TxPut + 1) & TX_BUF_MASK;		// advance the put pointer
	usart_desc->usart->CTRLA = (usart_desc->usart->CTRLA & ~USART_DREINTLVL_gm) | usart_desc->dreIntLevel;		// Enable DRE interrupt (might already be enabled)
}

//============================================================
// Data Register Empty Interrupt Service Routine

void USART_DataRegEmpty(USART_desc_t *usart_desc){

#if ENABLE_DEBUG
	if(FlowControl){
		while(!RTS()){
			wdt_reset();
		}
	}
#endif

	if (usart_desc->TxPut != usart_desc->TxGet){	// if there is something in the transmit buffer, send it
		usart_desc->usart->DATA = usart_desc->TxBuf[usart_desc->TxGet];
		usart_desc->TxGet = (usart_desc->TxGet + 1) & TX_BUF_MASK;	// advance the pointer, with wrap
	}
	else{	// else turn off DRE interrupts
		usart_desc->usart->CTRLA = (usart_desc->usart->CTRLA & ~USART_DREINTLVL_gm) | USART_DREINTLVL_OFF_gc; // Disable DRE interrupts
	}
}


