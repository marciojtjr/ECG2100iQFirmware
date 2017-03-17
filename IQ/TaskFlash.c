#ifndef BOOTLOADER

#include <avr/io.h>
#include "avr_compiler.h"
#include "spi_driver.h"
#include "rtos.h"
#include "iq.h"
#include "taskAcq.h"
#include "taskFlash.h"
#include "buffers.h"
#include "eeprom.h"
#include "taskUSB.h"
#include "taskBT.h"
#include "TaskWatchDog.h"

// Flash Commands
#define FCMD_WRITE_ENABLE		0x06
#define FCMD_WRITE_DISABLE	0x04
#define FCMD_READ_ID				0x9F
#define FCMD_READ_STATUS		0x05
#define FCMD_WRITE_STATUS		0x01
#define FCMD_READ_DATA			0x03
#define FCMD_PAGE_PROGRAM		0x02
#define FCMD_SECTOR_ERASE		0xD8
#define FCMD_CHIP_ERASE			0xC7
#define FCMD_POWERDOWN			0xB9
#define FCMD_POWERUP				0xAB

// Flash Status
#define FSTS_HARDWARE_WP		0x80	// 1 = WP pin is low (can't happen in our design)
#define FSTS_SOFTWARE_WE		0x02	// 1 = Write enable latch is set and programming is allowed
#define FSTS_BUSY						0x01	// 1 = busy writing (WIP/Write In Place)

// Globals
//static SPI_Descriptor_t spiDescriptor;			// spi operations descriptor
//static volatile char status;

//unsigned char TaskFlashCmd = TF_IDLE;	// used by acquisition task to tell us what to do
//unsigned int FlashPageNumber;					// set by others to tell us which page to read/write

/*
uint8_t FlashReadBuffer[512];
uint8_t FlashWriteBuffer[512];

uint8_t OkayToWriteToWriteBuffer = 1;
uint8_t OkayToReadFromReadBuffer = 0;

unsigned int currentReadBufferPos = 0;
unsigned int currentReadBufferSize = 0;

static volatile unsigned int currentFlashReadPage = 0, currentFlashWritePage = 0;
static unsigned int flashByte;
*/

//heatshrink_encoder encoder;

//===========================================================================================
// Power up the flash memory and make sure it's there
BEGIN_TASK(TaskFlash)

/*
static int8_t encoderRet;
static uint8_t encoderRetBytes[APPLE_BT_STRING_SIZE];
static size_t encoderRetCount;
// We're apprehending taskflash to do compression testing
while(1) {
	encoderRet = heatshrink_encoder_poll(&encoder, &encoderRetBytes[0], APPLE_BT_STRING_SIZE, &encoderRetCount);
	//if(encoderRet == HSER_POLL_NOT_READY) DPLEDTog();
	if(encoderRet != HSER_POLL_NOT_READY) { Seg7HP('r'); }
	osWAIT(1);
	//osYIELD;
}
*/

/*
	memset(FlashReadBuffer, 0, 512);
	memset(FlashWriteBuffer, 0, 512);


	//static unsigned char sector, endSector;

	//DPLEDOn();
	FlashPowerOn();
	osWAIT(10);

	// Initialize descriptor for SPI on port D
	SPI_Init(&spiDescriptor,					// pointer to descriptor to init
           &SPID,										// we're using SPID
           false,										// MSB goes first
           SPI_MODE_0_gc,						// SPI MODE 0
           true,										// use double speed
           SPI_PRESCALER_DIV4_gc);	// that's clk/2 or 16mbps

	do{	// check memory ID until it's correct
		osWAIT(1);
		SPI_SS_Lo(&spiDescriptor);
		SPI_TransceiveByte(&spiDescriptor,FCMD_READ_ID);
		for(unsigned char i=0; i<20; i++) {
			FlashReadBuffer[i]=SPI_TransceiveByte(&spiDescriptor, 0);
		}
		//!!! check for memory ID error here
		SPI_SS_Hi(&spiDescriptor);
	}while(FlashReadBuffer[0]!=0x20);
	//Seg7HP('F');

	//===========================================================================================
	// endlessly watch for commands from AcqTask (program) or USBTask (read, erase)

	while(1){
		osWAIT(1); // let other tasks run

		FlashWatchTimer = FLASH_WATCH_TIME;

		if(TaskFlashCmd != TF_IDLE){ // if there is anything to do, do it


			//-----------------------------------------------------------------------------------------
			// Write the bytes in Buf1 or Buf2 to the page specified by FlashPageNumber
			if(TaskFlashCmd == TF_ERASE_ALL){
				// read status
				SPI_SS_Lo(&spiDescriptor);
				SPI_TransceiveByte(&spiDescriptor, FCMD_READ_STATUS);
				status = SPI_TransceiveByte(&spiDescriptor, 0);
				SPI_SS_Hi(&spiDescriptor);

				// enable programming
				SPI_SS_Lo(&spiDescriptor);
				SPI_TransceiveByte(&spiDescriptor, FCMD_WRITE_ENABLE);
				SPI_SS_Hi(&spiDescriptor);

				// erase all of memory
				SPI_SS_Lo(&spiDescriptor);
				SPI_TransceiveByte(&spiDescriptor, FCMD_CHIP_ERASE);
				SPI_SS_Hi(&spiDescriptor);

				// we'll wait for completion at the bottom of the task
			}
			else if(TaskFlashCmd == TF_WRITE_BUF) {
				OkayToWriteToWriteBuffer = 0;
				TaskFlashCmd = TF_WRITE_BUF_STAGE_1;
			} else if (TaskFlashCmd == TF_READ_BUF) {
				TaskFlashCmd = TF_READ_BUF_STAGE_1;
				OkayToWriteToWriteBuffer = 0;
			}

			if(TaskFlashCmd == TF_WRITE_BUF_STAGE_1 || TaskFlashCmd == TF_WRITE_BUF_STAGE_2) {

				// enable programming
				SPI_SS_Lo(&spiDescriptor);
				SPI_TransceiveByte(&spiDescriptor, FCMD_WRITE_ENABLE);
				SPI_SS_Hi(&spiDescriptor);

				SPI_SS_Lo(&spiDescriptor);	// select
				SPI_TransceiveByte(&spiDescriptor, FCMD_PAGE_PROGRAM); // issue the write command and send the write address
				SPI_TransceiveByte(&spiDescriptor, currentFlashWritePage>>8);
				SPI_TransceiveByte(&spiDescriptor, currentFlashWritePage);
				SPI_TransceiveByte(&spiDescriptor, 0);

				if(TaskFlashCmd == TF_WRITE_BUF_STAGE_1){
					for(flashByte=0;flashByte<FLASH_PAGE_SIZE;flashByte++){	// write the bytes
						SPI_TransceiveByte(&spiDescriptor, FlashWriteBuffer[flashByte]);
					}
				}
				else{
					for(flashByte=FLASH_PAGE_SIZE;flashByte<FLASH_PAGE_SIZE*2;flashByte++){	// write the bytes
						SPI_TransceiveByte(&spiDescriptor, FlashWriteBuffer[flashByte]);
					}
				}

				SPI_SS_Hi(&spiDescriptor); // deselect

				// we'll wait for completion at the bottom of the task
			}
		
			//-----------------------------------------------------------------------------------------
			// Read a page (pointed to by FlashPageNumber) of bytes from Flash into either Buf1 or Buf2
			else if(TaskFlashCmd == TF_READ_BUF_STAGE_1 || TaskFlashCmd == TF_READ_BUF_STAGE_2){

				SPI_SS_Lo(&spiDescriptor);	// select
				SPI_TransceiveByte(&spiDescriptor, FCMD_READ_DATA); // issue the read command and send the read address
				SPI_TransceiveByte(&spiDescriptor, currentFlashReadPage>>8);
				SPI_TransceiveByte(&spiDescriptor, currentFlashReadPage);
				SPI_TransceiveByte(&spiDescriptor, 0);


				if(TaskFlashCmd == TF_READ_BUF_STAGE_1){
					for(flashByte=0;flashByte<FLASH_PAGE_SIZE;flashByte++){	// read the bytes
						FlashReadBuffer[flashByte]=SPI_TransceiveByte(&spiDescriptor, 0);
					}
				}
				else{
					for(flashByte=FLASH_PAGE_SIZE;flashByte<FLASH_PAGE_SIZE*2;flashByte++){	// read the bytes
						FlashReadBuffer[flashByte]=SPI_TransceiveByte(&spiDescriptor, 0);
					}
				}

				SPI_SS_Hi(&spiDescriptor); // deselect
			}
			//-----------------------------------------------------------------------------------------
			// Put Flash into deep powerdown
			else if(TaskFlashCmd == TF_SLEEP){
				SPI_SS_Lo(&spiDescriptor);	// select
				SPI_TransceiveByte(&spiDescriptor, FCMD_POWERDOWN); // issue the deep powerdown command
				SPI_SS_Hi(&spiDescriptor); // deselect
			}

			//-----------------------------------------------------------------------------------------
			// Wake Flash from deep powerdown
			else if(TaskFlashCmd == TF_WAKE){
				SPI_SS_Lo(&spiDescriptor);	// select
				SPI_TransceiveByte(&spiDescriptor, FCMD_POWERUP); // issue the deep powerdown command
				SPI_TransceiveByte(&spiDescriptor, 0);	// send three dummy bytes
				SPI_TransceiveByte(&spiDescriptor, 0);
				SPI_TransceiveByte(&spiDescriptor, 0);
				SPI_TransceiveByte(&spiDescriptor, 0);	// ID is returned here
				SPI_SS_Hi(&spiDescriptor); // deselect
			}
			//-----------------------------------------------------------------------------------------
			// Throw an error for commands we don't know
			else{
				// SystemError(ERR_BAD_TASKFLASH_CMD);
			}

			//-----------------------------------------------------------------------------------------
			// Spin until Flash is not busy before we return to idle
			do{	// check status of chip until it's not busy
				//Seg7HP('S');
				//Seg7HP('0');
				SPI_SS_Lo(&spiDescriptor);
				SPI_TransceiveByte(&spiDescriptor, FCMD_READ_STATUS);
				status = SPI_TransceiveByte(&spiDescriptor, 0) & FSTS_BUSY;
				SPI_SS_Hi(&spiDescriptor);
				osYIELD;
				//Seg7HP('1');
			}while(status==FSTS_BUSY);
			//DPLEDTog();
			//Seg7HP('S');
			// flush the appropriate record lengths if we erased flash buffers
			if(TaskFlashCmd == TF_WRITE_BUF_STAGE_1) {
				++currentFlashWritePage;
				TaskFlashCmd = TF_WRITE_BUF_STAGE_2;
				//Seg7HP('0');
			} else if(TaskFlashCmd == TF_WRITE_BUF_STAGE_2) {
				//Seg7HP('r');
				//Seg7HP('1');
				OkayToWriteToWriteBuffer = 1;
				++currentFlashWritePage;

				if(currentFlashWritePage == FLASH_NUM_PAGES) {
					currentFlashWritePage = 0;
				}
				TaskFlashCmd = TF_IDLE;
			} else if(TaskFlashCmd == TF_READ_BUF_STAGE_1) {
				Seg7HP('0');
				++currentFlashReadPage;
				TaskFlashCmd = TF_READ_BUF_STAGE_2;
			} else if(TaskFlashCmd == TF_READ_BUF_STAGE_2) {
				//Seg7HP('r');
				Seg7HP('1');
				currentReadBufferPos = 2;
				OkayToWriteToWriteBuffer = 1;
				currentReadBufferSize = ((((uint16_t)(FlashReadBuffer[0])) << 8) | (((uint16_t)(FlashReadBuffer[1])) & 0xff));
				++currentFlashReadPage;

				if(currentFlashReadPage == FLASH_NUM_PAGES) {
					currentFlashReadPage = 0;
				}

				OkayToReadFromReadBuffer = 1;
				TaskFlashCmd = TF_IDLE;
			} else {
				TaskFlashCmd = TF_IDLE;
			}

		// If we're idle and need to read a page
		} else if(!OkayToReadFromReadBuffer && currentFlashWritePage != currentFlashReadPage) {
			TaskFlashCmd = TF_READ_BUF;
			//Seg7HP('r');
		}
	}
*/
END_TASK

#endif