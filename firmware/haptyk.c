#include "uart_driver.h"

#include "bt.h"

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	// Creating the FILE streams to capture the serial output
	FILE uart_out = FDEV_SETUP_STREAM(uart_send, 0, _FDEV_SETUP_WRITE);
	FILE uart_in = FDEV_SETUP_STREAM(0, uart_read, _FDEV_SETUP_READ);

	uart_init();

	stdout = &uart_out;
	stdin = &uart_in;

	printf("Haptyk");

	bt_init();

	PORTC |= (1 << DDC7);
	DDRC |= (1 << DDC7);

	while (bt_connected() == 0) {
		_delay_ms(500);
	}
	PINC |= (1 << DDC7);

	unsigned char* test = "a";
	while(1) {
		bt_write(test, 1);
	}

	return 0;
}
