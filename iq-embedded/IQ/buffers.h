#ifndef BUFFERS_H
#define BUFFERS_H

enum {BUF_EMPTY, BUF_FILLING, BUF_FULL, BUF_WRITING};

struct ECGBuffer {
	unsigned char Sample[21][12];
	unsigned char Flag1;
	unsigned char Flag2;
	unsigned char Flag3;
	unsigned char Flag4;
};

union DATA{
	struct ECGBuffer ECG;
	unsigned char Voice[256];
	unsigned char Byte[256];
};

struct BUFFER {
	unsigned char Status;
	union DATA Data;
};


extern struct BUFFER Buf1;
extern struct BUFFER Buf2;

#endif
