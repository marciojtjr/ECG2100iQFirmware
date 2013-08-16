#ifndef taskacq_h
#define taskacq_h

void InitADCs(void);
void AcqInterrupt(void);
void PackECG(unsigned char *out, unsigned char *in);
void UnPackECG(signed int *out, unsigned char *in);

#define ECG_SAMPLES_PER_PAGE 21
#define ECG_SAMPLES_PER_MS 1
#define ECG_MS_PER_PAGE (ECG_SAMPLES_PER_PAGE / ECG_SAMPLES_PER_MS)

#define VOICE_SAMPLES_PER_PAGE	256	// samples
#define VOICE_SAMPLES_PER_MS 16
#define	VOICE_MS_PER_PAGE (VOICE_SAMPLES_PER_PAGE / VOICE_SAMPLES_PER_MS)

#define ECG_RECORD_LENGTH 	301.0 // seconds
#define VOICE_RECORD_LENGTH 32.7 // seconds

extern float VBatt;

enum {FSM_START, FSM_EMPTY, FSM_VOICERECORDING, FSM_PREACQ, FSM_ACQURING, FSM_WAITINGFORPACKET, FSM_COMPARE, FSM_FULL, FSM_DONE, FSM_ERASING, FSM_FULL_ON};
extern unsigned char acqState;
extern unsigned char btSpikeIndex;
extern unsigned char usbSpikeIndex;
extern unsigned char haveUsbSpike;

extern unsigned char haveFinalPacket;

#endif // taskacq_h
