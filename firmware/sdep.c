#include "sdep.h"

#include <stddef.h>
#include <util/delay.h>

#include "spi.h"
#include "print.h"

void sdep_send(struct sdep_packet_s* packet, struct sdep_packet_s* response) {
	spi_cs_assert();

	ht_log("Send packet:\n");

	while (spi_transfer(packet->type) == 0xFE) {
		spi_cs_negate();
		_delay_ms(1);
		spi_cs_assert();
	}

	_delay_ms(1);

	spi_transfer(packet->id_low);
	spi_transfer(packet->id_high);
	spi_transfer(packet->size);
	for (u8 i = 0; i < packet->size; ++i) {
		spi_transfer(packet->data[i]);
	}

	spi_cs_negate();

	ht_log("Response:\n");

	response->type = spi_transfer(0xFF);
	response->id_low = spi_transfer(0xFF);
	response->id_high = spi_transfer(0xFF);
	response->size = spi_transfer(0xFF);
	for (u8 i = 0; i < response->size; ++i) {
		response->data[i] = spi_transfer(0xFF);
	}

}
