#include "sdep.h"
#include "spi.h"

#include <stddef.h>
#include <util/delay.h>

void sdep_send(struct sdep_packet_s* packet, struct sdep_packet_s* response) {
	spi_cs_assert();

	while (spi_transfer(packet->type) == 0xFE) {
		spi_cs_negate();
		_delay_ms(1);
		spi_cs_assert();
	}

	spi_transfer(packet->id_low);
	spi_transfer(packet->id_high);
	spi_transfer(packet->size);
	for (u8 i = 0; i < packet->size; ++i) {
		spi_transfer(packet->data[i]);
	}

	spi_cs_negate();

	// Don't care about the response?
	if (response == NULL) { return; }

}
