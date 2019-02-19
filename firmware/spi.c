#include "spi.h"

#include <avr/io.h>

void spi_init() {
	DDRB = 0x03; // Set MOSI and clock as ouputs

	SPCR = ( // DS.185
		(0 << SPIE)		// Disable interrupts
		| (1 << SPE)	// Enable SPI unit
		| (0 << DORD)	// MSB first
		| (1 << MSTR)	// Use in master mode
		| (0 << CPOL)	// Idle low
		| (0 << CPHA)	// Sample high, setup low
		| (1 << SPR0)	// SCK = f_osc / 16
	);
}

u8 spi_transfer(u8 data) {
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}

