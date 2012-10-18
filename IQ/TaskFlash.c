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
static SPI_Descriptor_t spiDescriptor;			// spi operations descriptor
static char status;

unsigned char TaskFlashCmd = TF_IDLE;	// used by acquisition task to tell us what to do
unsigned int FlashPageNumber;					// set by others to tell us which page to read/write
//===========================================================================================
// Power up the flash memory and make sure it's there
BEGIN_TASK(TaskFlash)

	static unsigned char sector, endSector;
	static unsigned int i;

	DPLEDOn();
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
			Buf1.Data.Byte[i]=SPI_TransceiveByte(&spiDescriptor, 0);
		}
		//!!! check for memory ID error here
		SPI_SS_Hi(&spiDescriptor);
	}while(Buf1.Data.Byte[0]!=0x20);
	DPLEDOff();

	//===========================================================================================
	// endlessly watch for commands from AcqTask (program) or USBTask (read, erase)

	while(1){
		osWAIT(1); // let other tasks run

		FlashWatchTimer = FLASH_WATCH_TIME;

		if(TaskFlashCmd != TF_IDLE){ // if there is anything to do, do it

			//-----------------------------------------------------------------------------------------
			// Bulk erase entire Flash
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

			//-----------------------------------------------------------------------------------------
			// Erase all sectors in either the Voice or ECG sections of Flash
			else if(TaskFlashCmd == TF_ERASE_VOICE || TaskFlashCmd == TF_ERASE_ECG){
				if(TaskFlashCmd == TF_ERASE_VOICE){
					sector = FLASH_VOICE_START_SECTOR;
					endSector = FLASH_VOICE_END_SECTOR;
				}
				else{
					sector = FLASH_ECG_START_SECTOR;
					endSector = FLASH_ECG_END_SECTOR;
				}
				for(;sector<=endSector; sector++){
					// enable programming
					SPI_SS_Lo(&spiDescriptor);
					SPI_TransceiveByte(&spiDescriptor, FCMD_WRITE_ENABLE);
					SPI_SS_Hi(&spiDescriptor);

					// erase a sector
					SPI_SS_Lo(&spiDescriptor);
					SPI_TransceiveByte(&spiDescriptor, FCMD_SECTOR_ERASE);
					SPI_TransceiveByte(&spiDescriptor, sector);
					SPI_TransceiveByte(&spiDescriptor, 0);
					SPI_TransceiveByte(&spiDescriptor, 0);
					SPI_SS_Hi(&spiDescriptor);

					do{	// check status of chip erase until it's done
						osWAIT(100); // no hurry, sector erase takes 600ms typ
						SPI_SS_Lo(&spiDescriptor);
						SPI_TransceiveByte(&spiDescriptor, FCMD_READ_STATUS);
						status = SPI_TransceiveByte(&spiDescriptor, 0) & FSTS_BUSY;
						SPI_SS_Hi(&spiDescriptor);
					}while(status==FSTS_BUSY);
				}
			}

			//-----------------------------------------------------------------------------------------
			// Write the bytes in Buf1 or Buf2 to the page specified by FlashPageNumber
			else if(TaskFlashCmd == TF_WRITE_BUF1 || TaskFlashCmd == TF_WRITE_BUF2){
				// enable programming
				SPI_SS_Lo(&spiDescriptor);
				SPI_TransceiveByte(&spiDescriptor, FCMD_WRITE_ENABLE);
				SPI_SS_Hi(&spiDescriptor);

				SPI_SS_Lo(&spiDescriptor);	// select
				SPI_TransceiveByte(&spiDescriptor, FCMD_PAGE_PROGRAM); // issue the write command and send the write address
				SPI_TransceiveByte(&spiDescriptor, FlashPageNumber>>8);
				SPI_TransceiveByte(&spiDescriptor, FlashPageNumber);
				SPI_TransceiveByte(&spiDescriptor, 0);

				if(TaskFlashCmd == TF_WRITE_BUF1){
					for(i=0;i<FLASH_PAGE_SIZE;i++){	// write the bytes
						SPI_TransceiveByte(&spiDescriptor, Buf1.Data.Byte[i]);
					}
				}
				else{
					for(i=0;i<FLASH_PAGE_SIZE;i++){	// write the bytes
						SPI_TransceiveByte(&spiDescriptor, Buf2.Data.Byte[i]);
					}
				}
				SPI_SS_Hi(&spiDescriptor); // deselect
				// we'll wait for completion at the bottom of the task
			}
		
			//-----------------------------------------------------------------------------------------
			// Read a page (pointed to by FlashPageNumber) of bytes from Flash into either Buf1 or Buf2
			else if(TaskFlashCmd == TF_READ_BUF1 || TaskFlashCmd == TF_READ_BUF2){

				SPI_SS_Lo(&spiDescriptor);	// select
				SPI_TransceiveByte(&spiDescriptor, FCMD_READ_DATA); // issue the read command and send the read address
				SPI_TransceiveByte(&spiDescriptor, FlashPageNumber>>8);
				SPI_TransceiveByte(&spiDescriptor, FlashPageNumber);
				SPI_TransceiveByte(&spiDescriptor, 0);

				if(TaskFlashCmd == TF_READ_BUF1){
					for(i=0;i<FLASH_PAGE_SIZE;i++){	// read the bytes
						Buf1.Data.Byte[i]=SPI_TransceiveByte(&spiDescriptor, 0);
					}
				}
				else{
					for(i=0;i<FLASH_PAGE_SIZE;i++){	// read the bytes
						Buf2.Data.Byte[i]=SPI_TransceiveByte(&spiDescriptor, 0);
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
				SPI_SS_Lo(&spiDescriptor);
				SPI_TransceiveByte(&spiDescriptor, FCMD_READ_STATUS);
				status = SPI_TransceiveByte(&spiDescriptor, 0) & FSTS_BUSY;
				SPI_SS_Hi(&spiDescriptor);
				osYIELD;
			}while(status==FSTS_BUSY);
//			}while(1);
			// flush the appropriate record lengths if we erased flash buffers
			if(TaskFlashCmd == TF_ERASE_ALL){
				EE_Wr(EE_VOICE_RECORD_LENGTH,0);
				EE_Wr(EE_ECG_RECORD_LENGTH,0);
			}
			else if(TaskFlashCmd == TF_ERASE_VOICE){
				EE_Wr(EE_VOICE_RECORD_LENGTH,0);
			}
			else if(TaskFlashCmd == TF_ERASE_ECG){
				EE_Wr(EE_ECG_RECORD_LENGTH,0);
			}
			TaskFlashCmd = TF_IDLE;
		}
	}

END_TASK




