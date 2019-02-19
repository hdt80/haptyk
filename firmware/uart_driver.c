#include "uart_driver.h"

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
/*
#include <util/setbaud.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
*/

#include "LUFA/Drivers/Peripheral/Serial.h"

//#include "uart_desc.h"

/*
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Drivers/Board/Joystick.h>
#include <LUFA/Drivers/Board/LEDs.h>
#include <LUFA/Platform/Platform.h>

static CDC_LineEncoding_t LineEncoding = {
	.BaudRateBPS = 0,
    .CharFormat  = CDC_LINEENCODING_OneStopBit,
    .ParityType  = CDC_PARITY_None,
    .DataBits    = 8
};
*/

void uart_init(void) {
	/*
	MCUSR &= ~(1 << WDRF); // Disable watchdpg
	wdt_disable();

	clock_prescale_set(clock_div_1); // No prescale division

	USB_Init();
	*/

	Serial_Init(9600, 0);
	Serial_CreateStream(NULL);
}

int uart_send(char c, FILE* stream) {
	Serial_SendByte(c);

	return 0;
}

int uart_read(FILE* stream) {
	// Wait until data exists and can be read
	loop_until_bit_is_set(UCSR1A, RXC1);

	return UDR1;
}
