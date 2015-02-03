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
#include "Seg7.h"
#include "eeprom.h"
#include <util/delay.h>
#include <ctype.h>
#include <string.h>
//#include "apple.h"

int written = 0;

unsigned char ignoreNextRFCOMM = 0;
unsigned char RFCOMMConnectionEstablished = 0;

static unsigned char readyBufferPos = 0;

static unsigned char iapBufferPos = 0;
static int remainingLengthBytes = -1;
const char * const iapTest PROGMEM = "IAP 5 OK";
//const char * const iapTest2 PROGMEM = "IAP 1 OK";

static unsigned char rfcommBufferPos = 0;
const char * const rfcommTest PROGMEM = "RFCOMM";


unsigned char iniAPMode = 0;
unsigned char waitingForiAPPacket = 1;
int remainingiAPBytes = -1;

unsigned char readyForiAPMode = 0;

char readyForCommands = 0;
char lastC;

const char * const readyTest PROGMEM = "READY";

// Check for error out
const char * const badKeyTest PROGMEM = "RFC_CONNECTION_KEY_MISSING";
static unsigned char badKeyTestPos = 0;
unsigned char needToDropPairing = 0;

unsigned char ignoreNextRxReady = 0;

void resetUsartAppleVariables() {
	iapBufferPos = 0;
	remainingLengthBytes = -1;
	iniAPMode = 0;
	waitingForiAPPacket = 1;
	remainingiAPBytes = -1;
}

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

#ifdef BLUETOOTH
#define put_usb_byte(c) uart_putchar(&USBPortDescriptor,c)
#endif
//static int put_bt( char c, FILE *stream){
	//	uart_putchar(&BTPortDescriptor,c);
	//	return(0);
//}

#ifdef BLUETOOTH
#define put_bt_byte(c) uart_putchar(&BTPortDescriptor,c)
#endif

//============================================================
// Rx ready interrupt handler
void USART_RXComplete(USART_desc_t *usart_desc){

	uint8_t c = usart_desc->usart->DATA;	// get the byte
	
	// If we're recieving on BT, check to see if we've recieved "READY" yet
	#ifdef BLUETOOTH

	if(usart_desc == &BTPortDescriptor) {
		if(inBluetoothUpdateMode) {
			put_usb_byte(c);
		}

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

		if(iniAPMode) {
			//Seg7HP('A');
			// We have a command packet
			if(waitingForiAPPacket == 0) {
				if(remainingiAPBytes == -1) {
					remainingiAPBytes = c+1;
				}
				else {
					--remainingiAPBytes;
					usart_desc->RxBuf[usart_desc->RxPut++]=c;
					if(remainingiAPBytes != 0) {
						return;
					}
				}
			}
			if(c == 0x55 && waitingForiAPPacket) {
				usart_desc->RxPut = 0;
				waitingForiAPPacket = 0;
				//Seg7HP('A');
				//usart_desc->RxBuf[usart_desc->RxPut++]=c;
				//return;
			}
		}
		else { 
			if(c == iapTest[iapBufferPos] || (iapBufferPos == 4 && isdigit(iapTest[iapBufferPos]))) {
				if(++iapBufferPos == 8) {
					//inBluetoothUpdateMode = 0;
					//print_usb("We are in IAP mode!");
					//inBluetoothUpdateMode = 0;
					//iniAPMode = 1;
					//Seg7HP('O');
					
					readyForiAPMode = 1;
					//memset(usart_desc->RxBuf, 0, RX_BUF_SIZE);
					iapBufferPos = 0;

					connectionType = CONNECTION_APPLE;
					//DPLEDOn();
					//EE_Wr(EE_LAST_CONNECTION_TYPE, CONNECTION_APPLE);

					//print_usb("iAP Connection\r\n");
					//Seg7HP('O');
				}
			}
			else
				iapBufferPos = 0;
		}

/*
		if(waitingForFirstCommand) {
			if(connectionType == CONNECTION_ANDROID) {
				if(c == rfcommTest[rfcommBufferPos]) {
					if(++rfcommBufferPos == 6) {
						//connectionType = CONNECTION_ANDROID;
						rfcommBufferPos = 0;
						//print_usb("RFCOMM Connection\r\n");
						//Seg7HP('A');
						//EE_Wr(EE_LAST_CONNECTION_TYPE, CONNECTION_ANDROID);
						ignoreNextRxReady = 1;
						//Seg7HP('C');
						if(!ignoreNextRFCOMM) {
					
							RFCOMMConnectionEstablished = 1;
							RFCOMMTimer = 0;
						
						} else {
							ignoreNextRFCOMM = 0;
						}	
					}
				
				}else
						rfcommBufferPos = 0;
			}
		}
*/	
		
					// Check to see if we now have a bad key
					if(c == badKeyTest[badKeyTestPos]) {
						if(++badKeyTestPos == 26) {
							needToDropPairing = 1;
							badKeyTestPos = 0;
						}
					} else badKeyTestPos = 0;	
	}
	else {
		if(inBluetoothUpdateMode) {
			put_bt_byte(c);
			//put_usb_byte(c);
		}
	}
	#endif
	
	if(usart_desc->RxReady == 0){ // only receive characters if the last receive buffer has been processed
	if((
	!iniAPMode &&
	(c=='\r'|| c=='\n'))
	|| (iniAPMode && remainingiAPBytes == 0)
	){
		#ifdef BLUETOOTH
		if(usart_desc != &BTPortDescriptor
		//|| inBluetoothUpdateMode
		)
		#endif
		if(!inBluetoothUpdateMode) {
			uart_putchar(usart_desc,'\r');
		}
		
		if(!iniAPMode) {
			usart_desc->RxBuf[usart_desc->RxPut] = 0;
		}
		usart_desc->RxPut = 0;
		
		usart_desc->RxReady = 1;
		/*
		if(!ignoreNextRxReady) {
			usart_desc->RxReady = 1;
		}			
		else {
			 ignoreNextRxReady = 0;	
			 usart_desc->RxReady = 0;
		}
		*/			 
		//usart_desc->RxReady = 1;	
		
		if(readyForiAPMode) {
			iniAPMode = 1;

			//Seg7HP('A');


			readyForiAPMode = 0;
			
			// Flush this. No one cares
			usart_desc->RxReady = 0;
			memset((void*)usart_desc->RxBuf, 0, RX_BUF_SIZE);
		}
		if(iniAPMode) {
			remainingiAPBytes = -1;
			waitingForiAPPacket = 1;
		}
	}
	else {
		#ifdef BLUETOOTH
		if(usart_desc != &BTPortDescriptor
		//|| inBluetoothUpdateMode
		)
		#endif
		if(!inBluetoothUpdateMode) {
			uart_putchar(usart_desc,c);
		}
		
		//if(iniAPMode) {

		//}
		if(c=='\b'
		&& !iniAPMode && !inBluetoothUpdateMode
		){	// backspace
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