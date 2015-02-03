#ifndef BOOTLOADER
#include "buffers.h"

//------------------------------------------------------------------------------------------
// Two (ping-pong) Flash memory page buffers
// During acquisition of either voice or ECG, the acquisition interrupt fills one of these buffers
// When the buffer is filled, its status is set to "FULL" and the interrupt switches to the other buffer
// The flash manager then writes the buffer to flash and sets the buffer status back to empty
// Although it's sufficient to keep track of only FULL/EMPTY, we also have intermediate states of
// FILLING/WRITING to aid in debugging

struct BUFFER Buf1;
struct BUFFER Buf2;
#endif