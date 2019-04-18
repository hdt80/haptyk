/*
 *
 *	HaptykConnect.c v2
 *	Team Haptyk
 *	4/11/2019
 *
 *	This source is used to interface with a Haptyk
 *	device. Connections will be made, and UUIDs
 *	of services will be read from and updated on	
 *	a global button data array for the developer
 *	to use.
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "gattlib.h"
#include "HaptykConnect.h"

uuid_t bt_read_char;
gatt_connection_t* bt_connection;

void haptyk_init(const char* connection) {
	if (gattlib_string_to_uuid(HAPTYK_READ_UUID,
			strlen(HAPTYK_READ_UUID) + 1, &bt_read_char) < 0) {

		fprintf(stderr, "Failed to read UUID\n");
	}

	bt_connection = gattlib_connect(
		NULL, connection, BDADDR_LE_PUBLIC, BT_SEC_LOW, 0, 0);
	
	if (bt_connection == NULL) {
		fprintf(stderr, "Failed to connect to Bluetooth device %s\n", connection);
	}
}

struct haptyk_buttons_t haptyk_get_data() {
	int handle;

	uint8_t buffer[12];
	uint8_t length = sizeof(buffer);

	handle = gattlib_read_char_by_uuid(bt_connection, &bt_read_char, buffer, &length);
	if (handle == -1) {
		fprintf(stderr, "Failed to read gatt char");
	} else {
		for (int i = 0; i < length; ++i) {
			printf("%02x ", buffer[i]);
		}
		printf("\n");
	}

	struct haptyk_buttons_t data;

	return data;

}
