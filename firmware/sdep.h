#pragma once

#include "types.h"

#define SDEP_MESSAGE_TYPE_COMMAND	0x10
#define SDEP_MESSAGE_TYPE_RESPONSE	0x20
#define SDEP_MESSAGE_TYPE_ALERT		0x40
#define SDEP_MESSAGE_TYPE_ERROR		0x80

struct sdep_packet_s {
	u8 type;
	u8 id_high;
	u8 id_low;
	u8 size;
	u8 data[16];
};

void sdep_send(struct sdep_packet_s* message, struct sdep_packet_s* response);

// Print the contents of an SDEP packet
//
// packet - Packet to be printed. If NULL is passed, "null" is printed
//
void sdep_print_packet(struct sdep_packet_s* packet);
