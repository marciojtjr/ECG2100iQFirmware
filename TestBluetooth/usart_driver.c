#ifndef BOOTLOADER

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include "usart_driver.h"
#include "iq.h"
#include "wdt_driver.h"
#include "TaskUSB.h"
#include "taskBT.h"
#include "rtos.h"
#include <util/delay.h>

int written = 0;

static int readyBufferPos = 0;
char readyForCommands = 0;

const char * const readyTest = "READY";

//============================================================
void USART_InterruptDriver_Initialize(USART_desc_t *usart_desc, USART_t *usart, USART_DREINTLVL_t dreIntLevel){
	usart_desc->usart = usart;
	usart_desc->dreIntLevel = dreIntLevel;
	usart_desc->RxPut = 0;
	usart_desc->RxReady = 0;
	usart_desc->TxPut = 0;
	usart_desc->TxGet = 0;
	usart_desc->TxWaiting = 0;
	usart_desc->TxDelivered = 0;
	usart_desc->TxTotal = 0;
}

//static int put_usb( char c, FILE *stream){
//	uart_putchar(&USBPortDescriptor,c);
//	return(0);
//}

#ifdef WORKING
#define put_usb_byte(c) uart_putchar(&USBPortDescriptor,c)
#endif
//static int put_bt( char c, FILE *stream){
//	uart_putchar(&BTPortDescriptor,c);
//	return(0);
//}

#ifdef WORKING
#define put_bt_byte(c) uart_putchar(&BTPortDescriptor,c)
#endif

//============================================================
// Rx ready interrupt handler
void USART_RXComplete(USART_desc_t *usart_desc){

	uint8_t c = usart_desc->usart->DATA;	// get the byte
	
	// If we're recieving on BT, check to see if we've recieved "READY" yet
#ifdef BLUETOOTH

	if(usart_desc == &BTPortDescriptor) {
#ifdef WORKING
		//if(inBluetoothUpdateMode) {
		//	put_usb_byte(c);
		//}
#endif
		if(!readyForCommands) {	
			// Have we found the next consecutive character in "READY"?
			if(c == readyTest[readyBufferPos]) {
				// Keep going if we have
				if(++readyBufferPos == 5)
					readyForCommands = 1;
			}
			else
				// If not, we need to restart
				readyBufferPos = 0;
		}
	}
	else {
#ifdef WORKING
		//if(inBluetoothUpdateMode) {
		//	put_bt_byte(c);
		//}
#endif
	}
#endif
	
	if(usart_desc->RxReady == 0){ // only receive characters if the last receive buffer has been processed
		if(c=='\r'|| c=='\n'){
#ifdef BLUETOOTH
			if(usart_desc != &BTPortDescriptor 
#ifdef WORKING
//|| inBluetoothUpdateMode
#endif
			)
#endif
			uart_putchar(usart_desc,'\r');
			
			usart_desc->RxBuf[usart_desc->RxPut] = 0;
			usart_desc->RxPut = 0;
			usart_desc->RxReady = 1;
		}
		else {
#ifdef BLUETOOTH
			if(usart_desc != &BTPortDescriptor 
#ifdef WORKING
//			|| inBluetoothUpdateMode
#endif
			) 
#endif
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
	}
	
	//if(usart_desc->RxReady == 0){ // only receive characters if the last receive buffer has been processed
		//if(usart_desc->RxPut==255){
		//	uart_putchar(usart_desc,'\r');
		//	usart_desc->RxBuf[usart_desc->RxPut] = 0;
		//	usart_desc->RxPut = 0;
		//	usart_desc->RxReady = 1;
		//}
		//else {
	//}
//}
}

//============================================================
// send a character if possible, wait if not

void uart_putchar(USART_desc_t *usart_desc, uint8_t data){
	usart_desc->TxWaiting = 1;
	
	while( (( (usart_desc->TxPut)+1 ) & TX_BUF_MASK) == usart_desc->TxGet ); // wait for room

	usart_desc->TxBuf[usart_desc->TxPut]= data;	// put the byte in the buffer
	usart_desc->TxPut = (usart_desc->TxPut + 1) & TX_BUF_MASK;		// advance the put pointer
	
	usart_desc->TxDelivered = 0;	
	
	usart_desc->usart->CTRLA = (usart_desc->usart->CTRLA & ~USART_DREINTLVL_gm) | usart_desc->dreIntLevel;		// Enable DRE interrupt (might already be enabled)
	usart_desc->TxWaiting = 0;
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
		usart_desc->TxTotal++;
		usart_desc->TxGet = (usart_desc->TxGet + 1) & TX_BUF_MASK;	// advance the pointer, with wrap
	}
	else{	// else turn off DRE interrupts
		usart_desc->TxDelivered = 1;
		usart_desc->usart->CTRLA = (usart_desc->usart->CTRLA & ~USART_DREINTLVL_gm) | USART_DREINTLVL_OFF_gc; // Disable DRE interrupts
	}
}
#endif