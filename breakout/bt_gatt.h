#pragma once

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "structs.h"

#define Adafruit_BT Adafruit_BluefruitLE_SPI

extern u8 bt_response[65];

extern u8 gatt_index;

// Setup the GATT service and characteristic to use
//
void setup_gatt_service(Adafruit_BT* bt) {
	Serial.print("Starting GATT service... ");

	bt->verbose(true);
	bt->setMode(BLUEFRUIT_MODE_COMMAND);

	int32_t cmdResult = 0;

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

	Serial.println("done");
}

// Update the GATT characteristic with new data
//
// data - Data to update the char with
//
void update_gatt(Adafruit_BT* bt, struct button_data_s* data) {
	bt->setMode(BLUEFRUIT_MODE_COMMAND);

	int32_t cmdResult;

	// AT+GATTCHAR=1,01-01-01-01-01-01-01-01-01-01-01-01
	// AT+GATTCHAR=1,00-00-00-00-00-01-00-00-00-00-00-00

	static char values[] = "AT+GATTCHAR=1,00-00-00-00-00-00-00-00-00-00-00-00\r\0";

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

	//Serial.println();
	//Serial.println(values);

	bt->verbose(true);
	bt->atcommandIntReply(values, &cmdResult);
	bt->verbose(false);
}
