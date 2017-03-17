/*
 * apple.c
 *
 * Created: 15-Jan-14 16:35:25
 *  Author: snobaste
 */ 

#ifndef BOOTLOADER
#ifdef BLUETOOTH

#include "apple.h"
#include "taskUSB.h"
#include "taskBT.h"
#include "taskAcq.h"
#include "seg7.h"
#include "iq.h"
#include "usart_driver.h"
#include "eeprom.h"

 #define START_1 0xFF
 #define START_2 0x55

//const char * const lingoStrings[] = {"General", "Microphone", "Simple Remote", "Display Remote", "Extended Interface", "Accessory Power", "USB Host Mode", "RF Tuner", "Accessory Equalizer", "Sports", "Digital Audio", "Reserved", "Storage", "iPod Out", "Location"};

ApplePacket recievedPacket;
ApplePacket sentPacket;

int i;
char sendBuffer[31];
char sessionID[2];
unsigned short transID = 1;
unsigned char successfulAppleConnection = 0;
//char *oldPayload = sentPacket.payload;
volatile unsigned char waitingForAck = 0;
unsigned char haveCommandFromAppleDevice = 0;
char commandFromAppleDevice[10];
unsigned char canSendNextPacket = 0;
unsigned char hasError = 0;

void resetAppleVariables() {
	transID = 1;
	successfulAppleConnection = 0;
	//rot = 0;
	waitingForAck = 0;
	haveCommandFromAppleDevice = 0;
}

unsigned short getCurrentTransID() {
	return transID;
}
char *getSessionID() {
	return sessionID;
}

void getApplePacket(char *buffer) {
	//memset(recievedPacket.payload, 0, 25);

	if(buffer[0] == START_2) {
		//Seg7HP('A');
		recievedPacket.start = buffer[0];
		recievedPacket.length = buffer[1];
		recievedPacket.lingoID = buffer[2];
		recievedPacket.commandID = buffer[3];
		recievedPacket.transID = (buffer[4] << 8) | (buffer[5] & 0xFF);
		//recievedPacket.payload = 0;
		for(i = 6; i < 2+recievedPacket.length; i++) {
			recievedPacket.payload[i-6] = buffer[i];
		}
		recievedPacket.checksum = buffer[2+recievedPacket.length];
	} 

#ifdef WORKING
	else {
		/*
		print_usb("We have a weird packet that starts with %x\r\n", buffer[0]);
		Seg7HP('E');

		recievedPacket.start = buffer[0];
		recievedPacket.length = buffer[1];
		recievedPacket.lingoID = buffer[2];
		recievedPacket.commandID = buffer[3];
		recievedPacket.transID = (buffer[4] << 8) | (buffer[5] & 0xFF);
		//recievedPacket.payload = 0;
		for(i = 6; i < 2+recievedPacket.length; i++) {
			recievedPacket.payload[i-6] = buffer[i];
		}
		recievedPacket.checksum = buffer[2+recievedPacket.length];

		printApplePacket(recievedPacket);
		*/
	}
#endif
	//memset(buffer, 0, 25);
}

void printApplePacket(ApplePacket packet) {
	print_usb("Start = %x\r\nLength = %d\r\nLingo = %x\r\nCommandID = %x\r\nTransactionID = %d\r\nChecksum = %x\r\n", packet.start, packet.length, packet.lingoID, packet.commandID, packet.transID, packet.checksum);
	print_usb("Payload:\r\n\t");
	for(i = 0; i < 25; i++) {
		print_usb("%c", packet.payload[i]);
	}
	print_usb("\r\n");
}

void checkRecievedPacketAndSendNext() {
	//if(sentPacket.start != 0x)
	int needsToSend = 0;
	sentPacket.start = 0;

//	printApplePacket(recievedPacket);
	//Seg7HP('C');

	if(recievedPacket.lingoID == GENERAL) {
		switch(recievedPacket.commandID) {
			case IPODACK:
				break;

			case OPEN_DATA_SESSION_FOR_PROTOCOL:
			//Seg7HP('A');
				sessionID[0] = recievedPacket.payload[0];
				sessionID[1] = recievedPacket.payload[1];
				//print_usb("\tsessionID = %x\r\n\tprotocolIndex = %x\r\n", recievedPacket.payload[0] | (recievedPacket.payload[1] << 8), recievedPacket.payload[2]);
				sentPacket.start = START_2;
				sentPacket.length = 6;
				sentPacket.lingoID = 0;
				sentPacket.commandID = ACCESSORY_ACK;
				sentPacket.transID = recievedPacket.transID;
				sentPacket.payload[0] = 0x00;
				sentPacket.payload[1] = OPEN_DATA_SESSION_FOR_PROTOCOL;
				
				successfulAppleConnection = 1;
				waitingForFirstCommand = 1;
				
				needsToSend = 1;
				
				
				break;
				
			case IPOD_DATA_TRANSFER:
				haveCommandFromAppleDevice = 1;

				//print_usb("We have a command from the apple device: ");

				for(i = 2; i < recievedPacket.length-4; i++) {
					BTPortDescriptor.RxBuf[i-2] = recievedPacket.payload[i];
					//print_usb("%c", recievedPacket.payload[i]);
				}

				//print_usb("\r\n");

				// Package reply
				sentPacket.start = START_2;
				sentPacket.length = 6;
				sentPacket.lingoID = 0;
				sentPacket.commandID = ACCESSORY_ACK;
				sentPacket.transID = recievedPacket.transID;
				sentPacket.payload[0] = 0x00;
				sentPacket.payload[1] = IPOD_DATA_TRANSFER;

				needsToSend = 1;
				break;

			case CLOSE_DATA_SESSION:
				sentPacket.start = START_2;
				sentPacket.length = 6;
				sentPacket.lingoID = 0;
				sentPacket.commandID = ACCESSORY_ACK;
				sentPacket.transID = recievedPacket.transID;
				sentPacket.payload[0] = 0x00;
				sentPacket.payload[1] = CLOSE_DATA_SESSION;

				needsToSend = 1;
				break;
			default:
				//print_usb("WEIRD OPERATION\r\n");
				//printApplePacket(recievedPacket);
				//Seg7HP('O');
				break;
		}
		
//#ifdef WORKING
		if(recievedPacket.commandID == IPODACK) {
			//DPLEDTog();
			if(recievedPacket.payload[0] != 0x00) {
				//Seg7HP('E');
				//print_usb("ERROR ACK\r\n");
				//printApplePacket(recievedPacket);
				
				//for(int i = 0; i < RX_BUF_SIZE; i++)
				//	print_usb("%x ", BTPortDescriptor.RxBuf[i]);
												
											//}
											//memset(BTPortDescriptor.RxBuf, 0, RX_BUF_SIZE);
											
				//print_usb("\r\n\r\n");
				hasError = 1;
				canSendNextPacket = 1;
				//DPLEDTog();
			} 
			else {
				hasError = 0;
				canSendNextPacket = 1;
				//Seg7HP('A');
				//++transID;
				//if(transID == 100) {
				//	transID = 0;
				//}
			}
			waitingForAck = 0;
		}
//#endif 
		/*
		if(recievedPacket.commandID == 0x43) {
			haveCommandFromAppleDevice = 1;

			print_usb("We have a command from the apple device: ");

			for(i = 2; i < recievedPacket.length-4; i++) {
				commandFromAppleDevice[i-2] = recievedPacket.payload[i];
				print_usb("%c", recievedPacket.payload[i]);
			}

			print_usb("\r\n");
		}*/
	}
	
	if(sentPacket.start == START_2 && needsToSend) {
		//addChecksumToSentPacket();
		sendPacket();

		if(sentPacket.payload[1] == CLOSE_DATA_SESSION) {
			//print_usb("Death by CLOSE_DATA_SESSION\r\n");
			//BTEnable = 0;
			//BTOff();
			connectionNeedsReset = 1;
			
			if(acqState == FSM_FULL) {
				acqState = FSM_DONE;
			}				
			//connectionType = CONNECTION_ANDROID;
			successfulAppleConnection = 0;
			transID = 1;
			iniAPMode = 0;
		}
	}
}

void addChecksumToSentPacketIsECG(int isECG, unsigned short length) {
	sentPacket.checksum = 0;

	//sentPacket.checksum += sentPacket.length;
	sentPacket.checksum += sentPacket.lingoID;
	//sentPacket.checksum += sentPacket.transID;
	sentPacket.checksum += (sentPacket.transID >> 8);
	sentPacket.checksum += (sentPacket.transID & 0xFF);
	sentPacket.checksum += sentPacket.commandID;
	//sentPacket.checksum +=
	if(isECG) {
		sentPacket.checksum += ((6+length) >> 8);
		sentPacket.checksum += ((6+length) & 0xFF);
		sentPacket.checksum += sessionID[0];
		sentPacket.checksum += sessionID[1];

		//for(i = 0; i < length; i++) {
		//	sentPacket.checksum += AppleBTString[i];
		//}
		sentPacket.checksum += appleCrc;
	}
	else {
		sentPacket.checksum += sentPacket.length;
		for(i = 0; i < sentPacket.length - 4; i++) {
			sentPacket.checksum += sentPacket.payload[i];
		}
	}

	sentPacket.checksum = -(sentPacket.checksum & 0xFF);
}
void addChecksumToSentPacket() {
	addChecksumToSentPacketIsECG(0, 0);
}

void sendPacket() {
	//Seg7HP('A');
	addChecksumToSentPacket();

	uart_putchar(&BTPortDescriptor, START_1);
	uart_putchar(&BTPortDescriptor,sentPacket.start);
	uart_putchar(&BTPortDescriptor,sentPacket.length);
	uart_putchar(&BTPortDescriptor,sentPacket.lingoID);
	uart_putchar(&BTPortDescriptor,sentPacket.commandID);
	uart_putchar(&BTPortDescriptor,(sentPacket.transID >> 8));
	uart_putchar(&BTPortDescriptor,(sentPacket.transID & 0xFF));
	//uart_putchar(&BTPortDescriptor, sessionID[0]);
	//uart_putchar(&BTPortDescriptor, sessionID[1]);

	for(i = 0; i < sentPacket.length-4; i++) {
		//sendBuffer[i+6] = sentPacket.payload[i];
		uart_putchar(&BTPortDescriptor, sentPacket.payload[i]);
	}
	uart_putchar(&BTPortDescriptor, (sentPacket.checksum & 0xFF));
	//sendBuffer[7+sendPacket.length-4] = 0;
	++transID;
}

void startStreaming() {
	sentPacket.start = START_2;
	sentPacket.lingoID = 0;
	sentPacket.commandID = ACCESSORY_DATA_TRANSFER;
}

/*
void sinkAndRead(uint8_t *byte, size_t *sunk) {
	int ret = heatshrink_encoder_sink(&encoder, byte, APPLE_BT_STRING_SIZE, sunk);
	if(ret == HSER_SINK_OK) { Seg7HP('S');  }
	//heatshrink_encoder_poll(&encoder, byte, 1, sunk);
}
*/

void sendECGPacketWithLength(unsigned short modifiedLength) {
	//canSendNextPacket = 0;

	sentPacket.start = START_2;
	//sentPacket.length = 6+APPLE_BT_STRING_SIZE;
	sentPacket.lingoID = 0;
	sentPacket.commandID = ACCESSORY_DATA_TRANSFER;
	sentPacket.transID = transID;
	//sentPacket.payload = BTString;

	addChecksumToSentPacketIsECG(1, modifiedLength);

	uart_putchar(&BTPortDescriptor, START_1);
	uart_putchar(&BTPortDescriptor, START_2);
	uart_putchar(&BTPortDescriptor, 0x00);
	uart_putchar(&BTPortDescriptor, ((6+modifiedLength) >> 8));
	uart_putchar(&BTPortDescriptor, ((6+modifiedLength) & 0xFF));
	uart_putchar(&BTPortDescriptor, 0);
	uart_putchar(&BTPortDescriptor, ACCESSORY_DATA_TRANSFER);
	uart_putchar(&BTPortDescriptor, (transID >> 8));
	uart_putchar(&BTPortDescriptor, (transID & 0xFF));
	uart_putchar(&BTPortDescriptor, sessionID[0]);
	uart_putchar(&BTPortDescriptor, sessionID[1]);

	fwrite(AppleBTString, modifiedLength, 1, bt);
	//fwrite(AppleBTString, modifiedLength, 1, usb);

	uart_putchar(&BTPortDescriptor, (unsigned char)(sentPacket.checksum & 0xFF));

	++transID;
}
void sendECGPacket() {
	sendECGPacket(SMALL_BT_SIZE);
}

#endif
#endif