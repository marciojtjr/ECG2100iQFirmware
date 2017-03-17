#ifndef BOOTLOADER

#ifndef TASKBT_H
#define TASKBT_H

#define BITS 12
#define SAMPLING_RATE 500

#include <stdio.h>
#include "usart_driver.h"
#include "taskAcq.h"

#define BT_BAUD_RATE 115200

#define BT_STATE_SIZE 9

/* FOR PACE DETECTION */


    #define BT_PREAMBLE   0
    //#define BT_TYPE       6
	#define BT_PACESPIKE2 6
    #define BT_STATUS     7
    #define BT_LEADOFF    8
	#define BT_PACESPIKE  9
    #define BT_FRMINDEX   10
    #define BT_DATA0     12
    #define BT_DATA1     60
#if BITS == 12
    #define BT_CRC      108
	#define BT_SIZE     110
#else
	#define BT_CRC      140
    #define BT_SIZE     142
#endif

#define SMALL_BT_SIZE BT_SIZE-BT_PACESPIKE2-(BT_SIZE-BT_CRC)


/* FOR NOT PACE DETECTION */

/*
	#define BT_PREAMBLE   0
	#define BT_TYPE       6
	#define BT_STATUS     7
	#define BT_LEADOFF    8
	#define BT_FRMINDEX   9
	#define BT_DATA0     11
	#define BT_DATA1     59
	#define BT_CRC      107

	#define BT_SIZE     109
*/

#define APPLE_BT_STRING_SIZE ((SMALL_BT_SIZE)*3)
extern char AppleBTString[APPLE_BT_STRING_SIZE];

enum { CONNECTION_UNKNOWN, CONNECTION_ANDROID, CONNECTION_APPLE };

extern FILE *bt;

extern unsigned char Verbose;

extern unsigned char BTEnable;
extern unsigned char BTReady;	// 1 = a leadfail bias model is ready for decimation, 0 = not ready
extern unsigned char BTBufPhase; // 0 = decimation of LFBiasModel[0] loads the BTBuf, 1 = decimation of LFBiasModel[1] adds to BTBuf, after which /8 produces an average of both models
extern unsigned int BTBuf[];	// holds all eight channels of data, decimated from 1KHz to 125Hz.
extern char BTLeadOff;
extern unsigned char BTString[]; // room to store the Bluetooth sample output string.
extern unsigned char M;	// selects which of the two LFBiasModels is ready for examination, 0 or 1
extern int16_t LFBiasModel[2][8][4];	// two models (so we can ping-pong) for the eight channels, each model containing four samples
extern unsigned char BTStatus;
extern char BTStateString[BT_STATE_SIZE];
extern USART_desc_t BTPortDescriptor;
extern unsigned long long appleCrc;

void BTOn(void);
void BTOff(void);

// macro to make sending stuff a little easier
#define print_bt(fmt,...)  fprintf_P(bt, PSTR(fmt), ## __VA_ARGS__)
#define print_bt_debug(fmt,...) if(Debug) fprintf_P(bt, PSTR(fmt), ## __VA_ARGS__)
#define print_bt_verbose(fmt,...) if(Verbose) fprintf_P(bt, PSTR(fmt), ## __VA_ARGS__)

#define BT_USART			USARTE0
#define BT_USART_PORT PORTE

extern unsigned char transferingFullData;
enum {FULL_TRANSFER_NONE, FULL_TRANSFER_VOICE, FULL_TRANSFER_ECG};
extern unsigned char transferingDataType;

extern USART_desc_t BTPortDescriptor;	// a descriptor for the USART connected to the Bluetooth transceiver
extern FILE bt_str;

extern unsigned char connectionType;
extern unsigned char waitingForFirstCommand;
extern unsigned char connectionNeedsReset;
extern int16_t keepAliveTimer;

extern unsigned char isThereAnActiveConnection;

#endif
#endif