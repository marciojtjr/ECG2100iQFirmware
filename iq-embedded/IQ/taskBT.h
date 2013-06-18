#ifndef TASKBT_H
#define TASKBT_H

#include <stdio.h>

extern FILE *bt;

extern unsigned char Verbose;

extern unsigned char BTEnable;
extern unsigned char BTReady;	// 1 = a leadfail bias model is ready for decimation, 0 = not ready
extern unsigned char BTBufPhase; // 0 = decimation of LFBiasModel[0] loads the BTBuf, 1 = decimation of LFBiasModel[1] adds to BTBuf, after which /8 produces an average of both models
extern unsigned int BTBuf[];	// holds all eight channels of data, decimated from 1KHz to 125Hz.
extern unsigned char BTString[]; // room to store the Bluetooth sample output string.
extern unsigned char M;	// selects which of the two LFBiasModels is ready for examination, 0 or 1
extern int LFBiasModel[2][8][4];	// two models (so we can ping-pong) for the eight channels, each model containing four samples
extern unsigned char BTIn[252];
extern unsigned char BTStatus;

void BTOn(void);
void BTOff(void);

// macro to make sending stuff a little easier
#define print_bt(fmt,...)  fprintf_P(bt, PSTR(fmt), ## __VA_ARGS__)
#define print_bt_debug(fmt,...) if(Debug) fprintf_P(bt, PSTR(fmt), ## __VA_ARGS__)
#define print_bt_verbose(fmt,...) if(Verbose) fprintf_P(bt, PSTR(fmt), ## __VA_ARGS__)

#endif
