#ifndef BOOTLOADER

#include "spi_driver.h"

/*
// Initialize SPI module as master
// given a pointer to a descriptor, and some input arguments, fill the descriptor
// This descriptor remember which SPI port we're using, what port the pins are on and where the data packet descriptor is
void SPI_Init(SPI_Descriptor_t *descriptor,	// descriptor struct pointer
                    SPI_t *module,						// SPI module pointer
                    bool lsbFirst,						// LSB first if non-zero
                    SPI_MODE_t mode,					// SPI mode (Clock polarity and phase)
                    bool clk2x,								// SPI double speed mode if set
                    SPI_PRESCALER_t clockDivision)	// bit rate
{
	descriptor->module = module; // remember the pointer to the SPI module

	// from the pointer to the module, figure out what port it's on and remember that in the descriptor
	if(module == &SPIC){
		descriptor->port = &PORTC;
	}
	else if(module == &SPID){
		descriptor->port = &PORTD;
	}
	else if(module == &SPIE){
		descriptor->port = &PORTE;
	}
	else{
		descriptor->port = &PORTF;
	}

	descriptor->port->DIRSET = SPI_SS_bm | SPI_MOSI_bm | SPI_SCK_bm; 	// make SS, MOSI and SCK outputs
	descriptor->port->OUTSET = SPI_SS_bm;															// deselect the device
	descriptor->port->DIRCLR = SPI_MISO_bm;														// MISO is input

	// load the SPI control register (SS must be an output to enter master mode
	descriptor->module->CTRL   = clockDivision |           // SPI prescaler
	                      (clk2x ? SPI_CLK2X_bm : 0) |     // SPI Clock double
	                      SPI_ENABLE_bm |                  // Enable SPI module
	                      (lsbFirst ? SPI_DORD_bm  : 0) |  // Data order
	                      SPI_MASTER_bm |                  // SPI master
	                      mode;                            // SPI mode
}

void SPI_SS_Lo(SPI_Descriptor_t *descriptor){
	descriptor->port->OUTCLR = SPI_SS_bm;
}

void SPI_SS_Hi(SPI_Descriptor_t *descriptor){
	descriptor->port->OUTSET = SPI_SS_bm;
}

// Transceive a byte. SS is not handled here, to allow stringing multiple calls back-to-back
uint8_t SPI_TransceiveByte(SPI_Descriptor_t *descriptor, uint8_t TXdata){
	descriptor->module->DATA = TXdata;	// send byte
	while(!(descriptor->module->STATUS & SPI_IF_bm)); 	// wait complete
	return(descriptor->module->DATA);	// return received byte
}
*/
#endif