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

typedef enum { READ, WRITE } operation_t;
operation_t operation = READ;

static uuid_t g_uuid;

void haptyk_init(const char* connection) {
	gatt_connetion_t* bt_conn;
	uuid_t g_uuid;

	if (gattlib_string_to_uuid(HAPTYK_READ_UUID,
			strlen(HAPTYK_READ_UUID) + 1, &g_uuid) < 0) {

		fprintf(stderr, "Failed to read UUID\n");
	}

	bt_conn = gattlib_connection(NULL, connection, BDADDR_LE_PUBLIC, BT_SEC_LOW, 0, 0);
	if (bt_conn == NULL) {
		fprintf(stderr, "Failed to connect to Bluetooth device %s\n", connection);
	}
}

struct haptyk_buttons_t haptyk_get_data() {
	int handle;

	struct haptyk_buttons_t data;

	return data;

}
