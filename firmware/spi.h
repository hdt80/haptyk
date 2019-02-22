#pragma once

#include "types.h"
#include <avr/io.h>

void spi_init(void);

static inline void spi_cs_assert(void) {
	PORTB &= ~0x10;
	//spi_current_pins.cs_port |= spi_current_pins.cs_pin;		
}

static inline void spi_cs_negate(void) {
	PORTB |= 0x10;
	//spi_current_pins.cs_port &= ~spi_current_pins.cs_pin;
}

u8 spi_transfer(u8 data);

