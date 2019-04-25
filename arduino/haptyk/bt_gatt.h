#pragma once

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "structs.h"

#define Adafruit_BT Adafruit_BluefruitLE_SPI

#define BUTTON_IS_NOW_PRESSED(x) \
	gatt_update_needed == 0x00 && \
	button_data_prev.x == 0 && button_data_curr.x == 1

#define BUTTON_NOW_NOT_PRESSED(x) \
	gatt_update_needed == 0x00 && \
	button_data_prev.x == 1 && button_data_curr.x == 0

extern u8 bt_response[65];

extern u8 gatt_index;

extern u8 bt_connected;

// Update the GATT characteristic with new data
//
// data - Data to update the char with
//
void update_gatt(Adafruit_BT* bt, struct button_data_s* data) {
	bt->setMode(BLUEFRUIT_MODE_COMMAND);

	// AT+GATTCHAR=1,01-01-01-01-01-01-01-01-01-01-01-01
	// AT+GATTCHAR=1,00-00-00-00-00-01-00-00-00-00-00-00

	static char values[] = "AT+GATTCHAR=1,00-00-00-00-00-00-00-00-00-00-00-00";

	int32_t cmdResult;

	// Offset 14 for the AT+GATTCHAR=1, part at the start
	values[14 + 1] = (data->b0 == 0) ? '0' : '1';
	values[14 + 4] = (data->b1 == 0) ? '0' : '1';
	values[14 + 7] = (data->b2 == 0) ? '0' : '1';
	values[14 + 10] = (data->b3 == 0) ? '0' : '1';
	values[14 + 13] = (data->b4 == 0) ? '0' : '1';
	values[14 + 16] = (data->b5 == 0) ? '0' : '1';
	values[14 + 19] = (data->b6 == 0) ? '0' : '1';
	values[14 + 22] = (data->b7 == 0) ? '0' : '1';
	values[14 + 25] = (data->b8 == 0) ? '0' : '1';
	values[14 + 28] = (data->b9 == 0) ? '0' : '1';
	values[14 + 31] = (data->b10 == 0) ? '0' : '1';
	values[14 + 34] = (data->b11 == 0) ? '0' : '1';

#if BT_VERBOSE == 0x01
	bt->verbose(true);
#endif

	bt->atcommandIntReply(values, &cmdResult);

#if BT_VERBOSE == 0x01
	bt->verbose(false);
#endif
}

// Setup the BT module
//
void bt_setup(Adafruit_BT* bt) {
	if (!bt->begin(BT_VERBOSE)) {
		error("Failed to start BT module");
	}

	bt->echo(false);
	bt->info();

	Serial.println("BT module started");
}

void bt_update(Adafruit_BT* bt) {
	// Should an update be performed on the GATT stuff?
	u8 gatt_update_needed = 0x00;

	// Buttons that were released, now pressed
	if (BUTTON_IS_NOW_PRESSED(b0)) { gatt_update_needed = 0x01; }
	if (BUTTON_IS_NOW_PRESSED(b1)) { gatt_update_needed = 0x01; }
	if (BUTTON_IS_NOW_PRESSED(b2)) { gatt_update_needed = 0x01; }
	if (BUTTON_IS_NOW_PRESSED(b3)) { gatt_update_needed = 0x01; }
	if (BUTTON_IS_NOW_PRESSED(b4)) { gatt_update_needed = 0x01; }
	if (BUTTON_IS_NOW_PRESSED(b5)) { gatt_update_needed = 0x01; }
	if (BUTTON_IS_NOW_PRESSED(b6)) { gatt_update_needed = 0x01; }
	if (BUTTON_IS_NOW_PRESSED(b7)) { gatt_update_needed = 0x01; }
	if (BUTTON_IS_NOW_PRESSED(b8)) { gatt_update_needed = 0x01; }
	if (BUTTON_IS_NOW_PRESSED(b9)) { gatt_update_needed = 0x01; }
	if (BUTTON_IS_NOW_PRESSED(b10)) { gatt_update_needed = 0x01; }
	if (BUTTON_IS_NOW_PRESSED(b11)) { gatt_update_needed = 0x01; }

	// Buttons that were pressed, now released
	if (BUTTON_NOW_NOT_PRESSED(b0)) { gatt_update_needed = 0x01; }
	if (BUTTON_NOW_NOT_PRESSED(b1)) { gatt_update_needed = 0x01; }
	if (BUTTON_NOW_NOT_PRESSED(b2)) { gatt_update_needed = 0x01; }
	if (BUTTON_NOW_NOT_PRESSED(b3)) { gatt_update_needed = 0x01; }
	if (BUTTON_NOW_NOT_PRESSED(b4)) { gatt_update_needed = 0x01; }
	if (BUTTON_NOW_NOT_PRESSED(b5)) { gatt_update_needed = 0x01; }
	if (BUTTON_NOW_NOT_PRESSED(b6)) { gatt_update_needed = 0x01; }
	if (BUTTON_NOW_NOT_PRESSED(b7)) { gatt_update_needed = 0x01; }
	if (BUTTON_NOW_NOT_PRESSED(b8)) { gatt_update_needed = 0x01; }
	if (BUTTON_NOW_NOT_PRESSED(b9)) { gatt_update_needed = 0x01; }
	if (BUTTON_NOW_NOT_PRESSED(b10)) { gatt_update_needed = 0x01; }
	if (BUTTON_NOW_NOT_PRESSED(b11)) { gatt_update_needed = 0x01; }

	// Any changes? Lets update the GATT characteristic with new data
	if (gatt_update_needed != 0x00) {
		update_gatt(bt, &button_data_curr);
	}
}

void bt_loop(Adafruit_BT* bt) {
	// Is Haptyk connected to a device? If not, wait until paired
	if (bt_connected == 0x00) {
		Serial.print("Connecting... ");

		bt->verbose(false); // Disable output when connecting, lots of noise
		while (!bt->isConnected()) {
			_delay_ms(50);
		}
		Serial.println("done");

		bt_connected = 0x01;
		bt->verbose(BT_VERBOSE); // Renable verbose if needed
	} else { // If so, do the Bluetooth loop
		bt_update(bt);
	}
}

// Setup the GATT service and characteristic to use
//
void setup_gatt_service(Adafruit_BT* bt) {
	Serial.print("Starting GATT service... ");

	bt->verbose(true);
	bt->setMode(BLUEFRUIT_MODE_COMMAND);

	int32_t cmdResult = 0;

	bt->atcommandIntReply("AT+FACTORYRESET", &cmdResult);
	Serial.print("Result: ");
	Serial.println(cmdResult);

	bt->atcommandIntReply("AT+GATTCLEAR", &cmdResult);
	Serial.print("Result: ");
	Serial.println(cmdResult);

	bt->atcommandIntReply("AT+GATTADDSERVICE=UUID=0x1122", &cmdResult);
	Serial.print("Result: ");
	Serial.println(cmdResult);

	bt->println("AT+GATTADDCHAR=UUID=0x0004,PROPERTIES=0x02,"
		"MIN_LEN=12,MAX_LEN=12,VALUE=00-00-00-00-00-00-00-00-00-00-00-00");
	while (bt->readline(bt_response, 64)) {
		u8 idx = bt_response[0];
		if (idx > 0x39 || idx < 0x31) {
			Serial.print("Invalid index: ");
			Serial.print(idx);
			Serial.print(" (");
			Serial.print(idx, HEX);
			Serial.println(")");
		} else {
			gatt_index = idx - 0x30;
		}
	}

	bt->atcommandIntReply("ATZ", &cmdResult);
	Serial.print("Result: ");
	Serial.println(cmdResult);
	bt->verbose(false);

	Serial.println("GATT module started");
}
