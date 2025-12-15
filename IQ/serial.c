#ifdef BOOTLOADER
#include "defines.h"
#include "peanut.h"
#include <avr/wdt.h>


#include <avr/io.h>
#include "avr_compiler.h"
#include "serial.h"

#define XON  0x11
#define XOFF 0x13

unsigned long timeout;

void initbootuart(unsigned char value, unsigned char scaleFactor){
	UART_PORT.DIRSET |= UART_TX_PIN;
	MY_UART.BAUDCTRLA = value;
	MY_UART.BAUDCTRLB = (scaleFactor << USART_BSCALE0_bp)|(value >> 8);
	MY_UART.CTRLB = (1 << ENABLE_RECEIVER_BIT) | (1 << ENABLE_TRANSMITTER_BIT); // enable receive and transmit
	timeout = 0;
}

void sendchar(unsigned char c){ 
    MY_UART.DATA = c; // prepare transmission
    while (!(MY_UART.STATUS & (1 << TRANSMIT_COMPLETE_BIT)));
    // wait until byte sendt
    MY_UART.STATUS |= (1 << TRANSMIT_COMPLETE_BIT); // delete TXCflag
		wdt_reset();
}

void sendStr(const char *s) {
	while(*s) sendchar(*s++);
}

unsigned char recchar(void){
	while(!(MY_UART.STATUS & (1 << RECEIVE_COMPLETE_BIT))){ // wait for data
		if(ButtonPressed()){	// only wait for the host here if the button is pressed,
			wdt_reset();
		}
	}
	return(MY_UART.DATA);
}

void send_xon(void){
	sendchar(XON);
}

void send_xoff(void){
	sendchar(XOFF);
}

void disconnect_uart(void){
	MY_UART.CTRLB = 0;  // Disable receiver and transmitter
	UART_PORT.DIRCLR = UART_TX_PIN;  // Release TX pin
}

void flush_uart(void){
	volatile uint8_t dummy;
	while(MY_UART.STATUS & (1 << RECEIVE_COMPLETE_BIT)) {
		dummy = MY_UART.DATA;  // Drain RX buffer
	}
	(void)dummy;
}
#endif