#ifndef BOOTLOADER

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include "avr_compiler.h"

/*
// Hardware defines
#define SPI_SS_bm    PIN4_bm
#define SPI_MOSI_bm  PIN5_bm
#define SPI_MISO_bm  PIN6_bm
#define SPI_SCK_bm   PIN7_bm

// SPI master status code defines
#define SPI_OK              0     // transmission completed successfully
#define SPI_INTERRUPTED     1     // transmission was interrupted by another master
#define SPI_BUSY            2     // SPI module is busy with another transmission

// SPI master descriptor
typedef struct SPI_Descriptor{
	SPI_t *module;                // pointer to the spi module to use
	PORT_t *port;                	// pointer to the port that holds the spi pins
} SPI_Descriptor_t;

// Prototype functions. Documentation found in source file

void SPI_SS_Lo(SPI_Descriptor_t *descriptor);
void SPI_SS_Hi(SPI_Descriptor_t *descriptor);

void SPI_Init(SPI_Descriptor_t *descriptor,		// pointer to port descriptor
                    SPI_t *module,				// pointer to SPI module
										bool lsbFirst,				// lsb goes first if one
                    SPI_MODE_t mode,			// mode (clock polarity and phase)
                    bool clk2x,						// 2x mode if one
                    SPI_PRESCALER_t clockDivision);	// bit rate

uint8_t SPI_TransceiveByte(SPI_Descriptor_t *spi, uint8_t TXdata);
*/

#endif
#endif