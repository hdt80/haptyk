#include "bt.h"
#include "spi.h"
#include "sdep.h"

#include <stddef.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

void bt_init(void) {
	spi_init();

	// Line reset
	DDRD |= 0x10; // Set reset pin as output
	PORTD |= 0x10; // Ensure that reset is kept high
	PORTD &= ~0x10; // Negate reset line for 10ms
	_delay_ms(10);
	PORTD |= 0x10; // Reset kept low, go back high

	struct sdep_packet_s packet;
	packet.type = SDEP_MESSAGE_TYPE_COMMAND;
	packet.id_high = 0xBE;
	packet.id_low = 0xEF;
	packet.size = 0x00;

	sdep_send(&packet, NULL);
}

u8 bt_connected() {
	struct sdep_packet_s packet;
	packet.type = SDEP_MESSAGE_TYPE_COMMAND;
	packet.id_high = 0xA0;
	packet.id_low = 0x00;
	packet.size = 0x0D;

	char* cmd = "AT+GAPGETCONN";
	memcpy(&packet.data, cmd, strlen(cmd));

	struct sdep_packet_s response;

	sdep_send(&packet, &response);

	return response.data[0] == '0' ? 0 : 1;
}

void bt_write(u8* data, u8 len) {
	struct sdep_packet_s command;
	command.type = SDEP_MESSAGE_TYPE_COMMAND;
	command.id_high = 0xA0;
	command.id_low = 0x01;
	command.size = len;
	
	memcpy(&command.data, data, len);

	struct sdep_packet_s response;

	sdep_send(&command, &response);
}

const char* bt_at_command(const char* message) {
	struct sdep_packet_s command;
	command.type = SDEP_MESSAGE_TYPE_COMMAND;
	command.id_high = 0xA0;
	command.id_low = 0x00;
	command.size = strlen(message);

	u8 bytes_left = command.size;

	return "";
}

