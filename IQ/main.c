#ifdef BOOTLOADER

#include <avr/io.h>
#include <avr/wdt.h>
#include "avr_compiler.h"
#include "peanut.h"
#include "defines.h"
#include "serial.h"
#include "sp_driver.h"
#include "eeprom_driver.h"
#include "clksys_driver.h"
#include "seg7.h"
#include "eeprom.h"

#define BOOT_VERSION 3	// the first version was actually 0xffffffff

/*If the code space is critical, user can uncomment the following lines*/
//#define REMOVE_AVRPROG_SUPPORT
//#define REMOVE_BLOCK_SUPPORT

#define LARGE_MEMORY

#ifdef LARGE_MEMORY
#  define ADDR_T unsigned long
#else  /* !LARGE_MEMORY */
#  define ADDR_T unsigned int
#endif /* LARGE_MEMORY */

long unsigned int AppCRC; // holds the CRC of the application flash after loading, will be programmed it EEPROM upon successful completion of an update

//-------------------------------------------------------------------------
void (*appVec)( void ) = 0x0000; // Set up function pointer to RESET vector.

void gotoApp(void){
	SP_WaitForSPM();
	SP_LockSPM();
	EIND = 0x00;
	appVec();
}

void FastClock(void){
	//------------------------------------------------------------------------------------------
	// Set the CPU clock to 32MHz using an external 16MHz crystal and the PLL set to x2
	CLKSYS_XOSC_Config( OSC_FRQRANGE_12TO16_gc, false, OSC_XOSCSEL_XTAL_16KCLK_gc );	// setup for 16MHz external crystal
	CLKSYS_Enable( OSC_XOSCEN_bm );	// enable the external crystal oscillator
	while ( CLKSYS_IsReady( OSC_XOSCRDY_bm ) == 0 );	// wait till it's stable for use as the PLL input
	CLKSYS_PLL_Config( OSC_PLLSRC_XOSC_gc, 2 );	// drive the PLL from the external crystal, set multiplication to 2x
	CLKSYS_Enable( OSC_PLLEN_bm );	// enable the PLL
	CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc );	// set up the prescalers
	while ( CLKSYS_IsReady( OSC_PLLRDY_bm ) == 0 );	// wait for PLL to stabilize
	CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_PLL_gc );	// start clocking the system from it
	CLKSYS_Disable( OSC_RC2MEN_bm | OSC_RC32KEN_bm);
}

//-------------------------------------------------------------------------

#define BLOCKSIZE PAGESIZE // BLOCKSIZE should be chosen so that the following holds: BLOCKSIZE*n = PAGESIZE,  where n=1,2,3...

unsigned char BlockLoad(unsigned int size, unsigned char mem, ADDR_T *address){   
  unsigned int data;    
  ADDR_T tempaddress;
        	
	if(mem=='F'){  // Flash memory type  NOTE: For flash programming, 'address' is given in words.
    (*address) <<= 1; // Convert address to bytes temporarily.
    tempaddress = (*address);  // Store address in page.
    do{
      data = recchar();
      data |= (recchar() << 8);
      SP_LoadFlashWord(*address, data);
      (*address)+=2; // Select next word in memory.            
      size -= 2; // Reduce number of bytes to write by two.
    } while(size); // Loop until all bytes written.
    SP_WriteApplicationPage(tempaddress);

    SP_WaitForSPM();
    (*address) >>= 1; // Convert address back to Flash words again.
    return '\r'; // Report programming OK
  }
  else{ // Invalid memory type?
    return '?';
  }
}

void BlockRead(unsigned int size, unsigned char mem, ADDR_T *address){
	if(mem=='F'){    // Flash memory type.
    (*address) <<= 1; // Convert address to bytes temporarily.
    do{
      sendchar( SP_ReadByte( *address) );
      sendchar( SP_ReadByte( (*address)+1) );
      (*address) += 2;    // Select next word in memory.
      size -= 2;          // Subtract two bytes from number of bytes to read
    } while (size);         // Repeat until all block has been read
    (*address) >>= 1;       // Convert address back to Flash words again.
  }
}

//==============================================================================================================
//==============================================================================================================
//==============================================================================================================
int main(void){

	ADDR_T address = 0;
  volatile unsigned int temp_int = 0;
	volatile unsigned char cmd = 0;

	if(EE_Rd(EE_BOOT_VERSION)!= BOOT_VERSION){
		EE_Wr(EE_BOOT_VERSION, BOOT_VERSION);
	}

	// The bootstrap loader is entered after every reset because the reset vector fuse is set to BOOTSTRAP
	// Resets can be caused by a power up, watchdog timeout, brown-out, software reset or hardware reset
	// A powerup (and hence power-up reset) can be caused by either a press of the button or insertion of the USB cable, both normal occurances
	// A brown-out reset would be caused by a low battery. While this is not expected, it's not an indication of a firmware problem
	// A software reset should never occur, as the firmware doesn't make use of it
	// An external hardware reset should never occur because the reset line (part of the PDI interface) is not connected to anything

	// A watchdog reset is an indication of either a firmware problem, a glitch or an intentional watchdog timeout in the application from the "up"date USB command

	// If we don't see USB power, we could not have gotten a USB "up" command, so we go directly to the application

	// If examination of the reset status register (RST.STATUS) shows that we did NOT get a watchdog timeout, we jump to the application code
	// If the

	MainPowerOff(); // USB and/or button press will keep us from powering down, make sure we do power down if disconnected from USB or if the button is released
	InitButton();
	InitUSBDetect();	// prepare to look for USB power

	// blank the LED
	Seg7A(0);
	Seg7B(0);
	Seg7C(0);
	Seg7D(0);
	Seg7E(0);
	Seg7F(0);
	Seg7G(0);
	Seg7DP(0);

	FastClock();

	temp_int=10000;
	while(temp_int--);	// wait for things to settle

	if(SP_ApplicationCRC() == EE_Rd(EE_APP_CRC)){ // if the application is valid, see if we should run it
		if(!USBPresent()){	// if we're not being powered by USB, there's no way to update, so go to the app
			gotoApp();
		}

		if( (RST.STATUS & RST_WDRF_bm)==0 ){ // if we did not get a watchdog reset, go to the application and let it handle the reset
			gotoApp();
		}
	}

	// The application is not valid, or the button was pressed while we started with USB present
	// we had a watchdog reset while USB is present, wait for release of the button, while keeping the watchdog from tripping again

	// display "U" on the LED
	Seg7A(0);
	Seg7B(1);
	Seg7C(1);
	Seg7D(1);
	Seg7E(1);
	Seg7F(1);
	Seg7G(0);
	Seg7DP(0);

	//-------------------------------------------------------------------------
	// We're here because of a watchdog timer reset that occurred while we were connected to USB
	// If we were reset by the watchdog, that means the application is either damaged, or has received a firmware update command and wants to
	// turn control over to us. If we do not detect USB power, we turn off. If there was a glitch in the application software that tripped the watchdog timer
	// a power cycle may clear it. If there was damage to the application, there's nothing we can do until we get connected to a PC via USB



	//-------------------------------------------------------------------------
	// start of bootloader

	EEPROM_FlushBuffer();
	EEPROM_DisableMapping();

	initbootuart(19, -4); // 921600

  while(1){  	/* Main loop */

    cmd = recchar(); // Wait for command character.

		if(cmd=='!'){	// ! is the go to app command
			gotoApp();
		}
    if(cmd=='a'){    // Check autoincrement status.
      sendchar('Y'); // Yes, we do autoincrement.
    }
    else if(cmd == 'A'){ // Set address... // NOTE: Flash addresses are in words, not bytes.
      address = (recchar() << 8) | recchar(); // Read address high and low byte.	  
	  sendchar('\r'); // Send OK back.
    }
    else if(cmd=='e'){ // Chip erase.
      for(address = 0; address < APP_END; address += PAGESIZE){ // NOTE: Here we use address as a byte-address, not word-address, for convenience.
        SP_WaitForSPM();
      	SP_EraseApplicationPage( address );
				wdt_reset();
      }
      sendchar('\r'); // Send OK back.
    }

    else if(cmd=='b'){ // Check block load support.
			sendchar('Y'); // Report block load supported.
			sendchar((BLOCKSIZE >> 8) & 0xFF); // MSB first.
 			sendchar(BLOCKSIZE & 0xFF); // Report BLOCKSIZE (bytes).
		}
		else if(cmd=='B'){ // Start block load.
  	  DPLEDTog();
			temp_int = (recchar()<<8) | recchar(); // Get block size.
      cmd = recchar(); // Get memtype.
	    sendchar( BlockLoad(temp_int, cmd, &address) ); // Block load.
    }	    
		else if(cmd=='g'){ // Start block read.
  	  DPLEDTog();
			temp_int = (recchar()<<8) | recchar(); // Get block size.
		  cmd = recchar(); // Get memtype
  		BlockRead(temp_int, cmd, &address); // Block read
		}		

    else if((cmd=='P')||(cmd=='L')){ // Enter and leave programming mode.
      sendchar('\r'); // Nothing special to do, just answer OK.
    }

    else if(cmd=='E'){ // Exit bootloader.
      sendchar('\r');
			AppCRC = SP_ApplicationCRC();
			EE_Wr(EE_APP_CRC,AppCRC);
      gotoApp();
    }
	

    else if (cmd=='p'){ // Get programmer type.        
      sendchar('S'); // Answer 'SERIAL'.
    }
    else if(cmd=='t'){ // Return supported device codes.

#if PARTCODE+0 > 0
      sendchar( PARTCODE ); // Supports only this device, of course.
#endif /* PARTCODE */
      sendchar( 0 ); // Send list terminator.
    }
    else if((cmd=='x')||(cmd=='y')||(cmd=='T')){ // Set LED, clear LED and set device type.
      recchar(); // Ignore the command and its parameter.
      sendchar('\r'); // Send OK back.
    }

    else if(cmd=='S'){ // Return programmer identifier.
      sendchar('A'); // Return 'AVRBOOT'.
      sendchar('V'); // Software identifier (aka programmer signature) is always 7 characters.
      sendchar('R');
      sendchar('B');
      sendchar('O');
      sendchar('O');
      sendchar('T');
    }
    else if(cmd=='V'){ // Return software version.
      sendchar('1');
      sendchar('6');
    }        
    else if(cmd=='s') { // Return signature bytes.
      sendchar( SIGNATURE_2 );
      sendchar( SIGNATURE_1 );
      sendchar( SIGNATURE_0 );
    }       
    // The last command to accept is ESC (synchronization).
    else if(cmd!=0x1b){ // If not ESC, then it is unrecognized...
      sendchar('?');
    }
  } // end: while(1)
} // end: main

#endif