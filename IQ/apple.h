/*
 * apple.h
 *
 * Created: 15-Jan-14 16:16:01
 *  Author: snobaste
 */ 

#ifndef BOOTLOADER
#ifdef BLUETOOTH

#ifndef APPLE_H_
#define APPLE_H_

enum LINGOS {
	GENERAL,
	MICROPHONE,
	SIMPLE_REMOTE,
	DISPLAY_REMOTE,
	EXTENDED_INTERFACE,
	ACCESSORY_POWER,
	USB_HOST_MODE,
	RF_TUNER,
	ACCESSORY_EQUALIZER,
	SPORTS,
	DIGITAL_AUDIO,
	RESERVED,
	STORAGE,
	IPOD_OUT,
	LOCATION
};

enum COMMANDS {
	REQUEST_IDENTIFY,
	IDENTIFY,
	IPODACK,
	REQUEST_EXTENDED_INTERFACE_MODE,
	ENTER_EXTENDED_INTERFACE_MODE,
	EXIT_EXTENDED_INTERFACE_MODE,
	REQUEST_IPOD_NAME,
	RETURN_IPOD_NAME,
	REQUEST_IPOD_SOFTWARE_VERSION,
	RETURN_IPOD_SOFTWARE_VERSION,
	REQUEST_IPOD_SERIAL_NUM,
	RETURN_IPOD_SERIAL_NUM,
	REQUEST_IPOD_MODEL_NUM,
	RETURN_IPOD_MODEL_NUM,
	REQUEST_LINGO_PROTOCOL_VERSION,
	RETURN_LINGO_PROTOCOL_VERSION,
	REQUEST_TRANSPORT_MAX_PAYLOAD_SIZE,
	RETURN_TRANSPORT_MAX_PAYLOAD_SIZE,
	IDENTIFY_DEVICE_LINGOES,
	GET_ACCESSORY_AUTHENTICATION_INFO,
	RET_ACCESSORY_AUTHENTICATION_INFO,
	ACK_ACCESSORY_AUTHENTICATION_INFO,
	GET_ACCESSORY_AUTHENTICATION_SIGNATURE,
	RET_ACCESSORY_AUTHENTICATION_SIGNATURE,
	ACK_ACCESSORY_AUTHENTICATION_STATUS,
	GET_IPOD_AUTHENTICATION_INFO,
	RET_IPOD_AUTHENTICATION_INFO,
	ACK_IPOD_AUTHENTICATION_INFO,
	GET_IPOD_AUTHENTICATION_SIGNATURE,
	RET_IPOD_AUTHENTICATION_SIGNATURE,
	ACK_IPOD_AUTHENTICATION_STATUS,
	RESERVED1,
	RESERVED2,
	RESERVED3,
	NOTIFY_IPOD_STATUS_CHANGE,
	GET_IPOD_OPTIONS,
	RET_IPOD_OPTIONS,
	RESERVED4,
	GET_ACCESSORY_INFO,
	RET_ACCESSORY_INFO,
	GET_IPOD_PREFERENCES,
	RET_IPOD_PREFERENCES,
	SET_IPOD_REFERENCES,
	GET_UI_MODE = 0x35,
	RET_UI_MODE,
	SET_UI_MODE,
	START_IDPS,
	SET_FID_TOKEN_VALUES,
	ACK_FID_TOKEN_VALUES,
	END_IDPS,
	IDPS_STATUS,
	OPEN_DATA_SESSION_FOR_PROTOCOL = 0x3F,
	CLOSE_DATA_SESSION,
	ACCESSORY_ACK,
	ACCESSORY_DATA_TRANSFER,
	IPOD_DATA_TRANSFER,
	SET_ACCESSORY_STATUS_NOTIFICATION = 0x46,
	RET_ACCESSORY_STATUS_NOTIFICATION
};	
	
typedef struct iAPPacket {
	unsigned char start;
	unsigned char length;
	unsigned char lingoID;
	unsigned char commandID;
	unsigned short transID;
	unsigned char payload[25];
	//unsigned char checksum;
	unsigned long long checksum;
} ApplePacket;

extern const char * const lingoStrings [];

extern unsigned char successfulAppleConnection;
extern volatile unsigned char waitingForAck;
extern unsigned char haveCommandFromAppleDevice;
extern unsigned char canSendNextPacket;
extern unsigned char hasError;

extern char commandFromAppleDevice[10];

extern ApplePacket recievedPacket;
extern ApplePacket sentPacket;
extern char sendBuffer[31];

void getApplePacket(char *buffer);
void printApplePacket(ApplePacket packet);
void addChecksumToSentPacketIsECG(int isECG, unsigned short length);
void addChecksumToSentPacket();
void checkRecievedPacketAndSendNext();
void sendPacket();
void sendECGPacket();
void sendECGPacketWithLength(unsigned short modifiedLength);
void resetAppleVariables();
unsigned short getCurrentTransID();
char *getSessionID();
void startStreaming();

#endif /* BLUETOOTH */

#endif /* APPLE_H_ */
#endif