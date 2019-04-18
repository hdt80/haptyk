/*
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

int main(int argc, char * argv[]){
	uint8_t buffer[20];
	int index, handle;
	size_t length = sizeof(buffer);
	gatt_connection_t * connection;

	if (gattlib_string_to_uuid(HAPTYK_READ_UUID, strlen(HAPTYK_READ_UUID) + 1, &g_uuid) < 0) {
		fprintf(stderr, "READ UUID ERROR\n");
		return 1;
	}

	connection = gattlib_connect(NULL, argv[1], BDADDR_LE_PUBLIC, BT_SEC_LOW, 0, 0); // TODO SWITCH ARGV[1] TO FUNC
	if (connection == NULL) {
		fprintf(stderr, "Fail to connect to the bluetooth device.\n");
		return 1;
	}

	if (operation == READ){
		handle = gattlib_read_char_by_uuid(connection, &g_uuid, buffer, &length);	// read the characteristic
		if (handle == -1) {
			char error[MAX_LEN_UUID_STR + 1];

			gattlib_uuid_to_string(&g_uuid, error, sizeof(error));

			fprintf(stderr, "Could not find GATT Characteristic with UUID %s\n", error);
			goto done;
		}

		printf("Read UUID completed: ");
		for (index = 0; index < length; index++) {
			printf("%02x ", buffer[index]);
		}
		printf("\n");


	}
	//	else {
	//add write command

	//	}


done:
	gattlib_disconnect(connection);	
	return 0;
}
