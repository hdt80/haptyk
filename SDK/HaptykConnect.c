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
 *	to use; you can redistribute it and/or modify
 *  	it under the terms of the GNU General Public 
 *  	License as published by the Free Software Foundation
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "gattlib.h"
#include "HaptykConnect.h"

uuid_t bt_read_char;			// handle for the current characteristic to read from
gatt_connection_t* bt_connection;	// handle for a Bluetooth connection


/*********************************************************************************************
 *
 *	Funtion: Haptyk_init
 *	Return: void
 *	Description: Use this function to connnect to any haptyk device. This is accomplished
 *		by passing in the MAC Address of the device you want to connect to. 
 *
 ********************************************************************************************/
void haptyk_init(const char* connection) {
	if (gattlib_string_to_uuid(HAPTYK_READ_UUID,
			(strlen(HAPTYK_READ_UUID) + 1), &bt_read_char) < 0) {

		fprintf(stderr, "Failed to read UUID\n");
	}

	bt_connection = gattlib_connect(
		NULL, connection, BDADDR_LE_RANDOM, BT_SEC_LOW, 0, 0);
	
	if (bt_connection == NULL) {
		fprintf(stderr, "Failed to connect to Bluetooth device %s\n", connection);
	}
}

/*********************************************************************************************
 *
 *	Funtion: Haptyk_disconnect
 *	Return: void
 *	Description: Use this function to disconnnect from any haptyk device. This is accomplished
 *		by passing the MAC Address of the device ALREADY connected.
 *
 ********************************************************************************************/
void haptyk_disconnect(const char* connection) {
	gattlib_disconnect(bt_connection);
	if (bt_connection == NULL) {
		fprintf(stderr, "Failed to disconnect to Bluetooth device \n");
	}
}

/*********************************************************************************************
 *
 *	Funtion: Haptyk_get_data
 *	Return: void
 *	Description: Use this function to read the current data from any characteristic
 *		defined by Haptyk. This incldues the current button data and the battery level
 *		of the paried Haptyk Device. 
 *
 ********************************************************************************************/
void haptyk_get_data(struct haptyk_buttons_t * data) {
	int handle;				

	uint8_t buffer[12];
	size_t length = sizeof(buffer);

	handle = gattlib_read_char_by_uuid(bt_connection, &bt_read_char, buffer, &length);
	if (handle == -1) {
		fprintf(stderr, "Failed to read gatt char");
	} else {
		data->b0 = buffer[0];	
		data->b1 = buffer[1];
		data->b2 = buffer[2];
		data->b3 = buffer[3];
		data->b4 = buffer[4];
		data->b5 = buffer[5];
		data->b6 = buffer[6];
		data->b7 = buffer[7];
		data->b8 = buffer[8];
		data->b9 = buffer[9];
		data->b10 = buffer[10];
		data->b11 = buffer[11];
	}
}

/*********************************************************************************************
 *
 *	Funtion: Haptyk_print
 *	Return: void
 *	Description: Use this function to print out the button data of the currently
 *		connected Haptyk device. 
 *
 *
 ********************************************************************************************/
void haptyk_print(struct haptyk_buttons_t* data) {
	printf("%x => %i-%i-%i-%i-%i-%i-%i-%i-%i-%i-%i-%i\n", (void*) data,
			data->b0, data->b1, data->b2, data->b3, 
			data->b4, data->b5, data->b6, data->b7,
			data->b8, data->b9, data->b10, data->b11);
}
