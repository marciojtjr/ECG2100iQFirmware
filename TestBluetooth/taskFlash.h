#ifndef BOOTLOADER

#ifndef taskflash_h
#define taskflash_h

#define FLASH_PAGE_SIZE 				256

// Flash commands
// These may be issued from any other task by checking that TaskFlashCmd == TF_IDLE before
// placing the appropriate command into that variable

enum {TF_IDLE, TF_ERASE_ALL, TF_ERASE_VOICE, TF_ERASE_ECG, TF_WRITE_BUF1, TF_WRITE_BUF2, TF_READ_BUF1, TF_READ_BUF2, TF_SLEEP, TF_WAKE};

// Flash Page Layout
// The flash is 4Mbytes, divided into 64 sectors (64KByte each), each divided into 256 pages of 256 bytes each
// ECG Samples are 12 bytes (and one millisecond) long (eight 12-bit samples, packed into 12 bytes)
// This is not a clean fit into 256 byte pages, so we do our best
// We can pack 21 12-byte samples into 252 bytes, leaving four bytes for status information for each sample
// 300 seconds of data at 21 samples (and milliseconds)/page requires 300/0.021 = 14,286 pages

// Voice is sampled at 16KHz for a maximum of 30 seconds, requiring 30*16K/256 = 1,875 pages

// We split the Flash into two regions, Voice and ECG, on a sector boundary
// The only sector boundary that leaves > 1,875 pages for voice and > 14,286 pages for ECG is at page 2048
// So we have:
//	2048 pages for voice, for a total recording time of 2048*16ms = 32.768s
//	14,336 pages for ECG, for a total recording time of 14,336 * 0.021 = 301.056s

// This information is laid out in Flash as follows:

// Pages 0-2047 = Voice (sectors 0-7)
// Pages 2048-16,383 = ECG (sectors 8-63)

#define FLASH_PAGES_PER_SECTOR		256UL
#define FLASH_ADDR_SIZE 					3 // size of the address field for commands, 24 bit address
#define FLASH_SIZE 								4	// megabytes
#define FLASH_SIZE_BYTES					(FLASH_SIZE * 1024UL * 1024UL)
#define FLASH_NUM_PAGES						(FLASH_SIZE_BYTES/FLASH_PAGE_SIZE)
#define FLASH_NUM_SECTORS					(FLASH_NUM_PAGES/FLASH_PAGES_PER_SECTOR)

#define FLASH_START_PAGE 					0
#define FLASH_END_PAGE 						(FLASH_NUM_PAGES-1)
#define FLASH_START_SECTOR				0
#define FLASH_END_SECTOR					(FLASH_NUM_SECTORS-1)

#define FLASH_VOICE_START_PAGE 		FLASH_START_PAGE
#define FLASH_VOICE_NUM_PAGES			2048
#define FLASH_VOICE_END_PAGE			(FLASH_VOICE_START_PAGE + FLASH_VOICE_NUM_PAGES - 1)

#define FLASH_VOICE_START_SECTOR 	0
#define FLASH_VOICE_NUM_SECTORS		(FLASH_VOICE_NUM_PAGES/FLASH_PAGES_PER_SECTOR)
#define FLASH_VOICE_END_SECTOR		(FLASH_VOICE_NUM_SECTORS-1)

#define FLASH_ECG_START_PAGE			(FLASH_VOICE_END_PAGE + 1)
#define FLASH_ECG_NUM_PAGES 			(FLASH_NUM_PAGES - FLASH_VOICE_NUM_PAGES)
#define FLASH_ECG_END_PAGE				(FLASH_ECG_START_PAGE + FLASH_ECG_NUM_PAGES - 1)

#define FLASH_ECG_START_SECTOR 		(FLASH_VOICE_END_SECTOR + 1)
#define FLASH_ECG_NUM_SECTORS			(FLASH_ECG_NUM_PAGES/FLASH_PAGES_PER_SECTOR)
#define FLASH_ECG_END_SECTOR			(FLASH_ECG_START_SECTOR + FLASH_ECG_NUM_SECTORS - 1)
//#define FLASH_ECG_END_SECTOR			63


#if FLASH_ECG_END_PAGE != FLASH_END_PAGE
	#error Error in Flash page calculations
#endif
#if FLASH_ECG_END_SECTOR != FLASH_END_SECTOR
	#error Error in Flash page calculations
#endif
#if FLASH_ECG_END_SECTOR !=63
	#error Flash End Sector error
#endif

// globals for inter-task communication
extern unsigned char TaskFlashCmd;		// holds the currently executing flash command
extern unsigned int FlashPageNumber;	// holds the currently active flash page number

#endif // taskflash_h
#endif