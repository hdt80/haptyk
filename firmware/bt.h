#pragma once

#include "types.h"

enum PACKET_ID {
	BTN_PRESSED = 0x01,
	BTN_RELEASED = 0x02
};

typedef struct {
	u8 packet_id;
	u8 device_id;
	u8 data_length;
	u8* data;
} bt_packet;

// Initialize the Bluetooth module by setting up the SPI with settings used
// for SDEP
void bt_init(void);

// Check if the Bluetooth module has connected to the target device and can
// sent bt_packets to it
//
// returns: 0 if the Bluetooth module is not connected to the target device
//			1 if the Bluetooth module is connected to the target device
u8 bt_connected();

void bt_write(u8* data, u8 len);

const char* bt_at_command(const char* message);

// Send a bt_packet to the target device
//
// packet - bt_packet to be sent to the target device
void bt_send(bt_packet* packet);
