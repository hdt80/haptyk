#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "Adafruit_MPR121.h"

#include "config.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// Hardware SPI
// CS = 8, IRQ = 7, RST = 4

Adafruit_BluefruitLE_SPI bt(HW_SPI_CS, HW_SPI_IRQ, HW_SPI_RST);
Adafruit_MPR121 capacs = Adafruit_MPR121();

struct packet_data_s {
	char device_id;
	char packet_id;
	char length;
	char data[12]; // Max 12 needed for now
} packet;

// Contains the status of each button sensor
struct button_data_s {
	unsigned char b0;
	unsigned char b1;
	unsigned char b2;
	unsigned char b3;
	unsigned char b4;
	unsigned char b5;
	unsigned char b6;
	unsigned char b7;
	unsigned char b8;
	unsigned char b9;
	unsigned char b10;
	unsigned char b11;
} button_data_prev, button_data_curr;

void error(const char* err) {
	Serial.println(err);
	while(1);
}

void print_button_data(const struct button_data_s* data) {
	Serial.print(data->b0, HEX);
	Serial.print(data->b1, HEX);
	Serial.print(data->b2, HEX);
	Serial.print(data->b3, HEX);
	Serial.print(data->b4, HEX);
	Serial.print(data->b5, HEX);
	Serial.print(data->b6, HEX);
	Serial.print(data->b7, HEX);
	Serial.print(data->b8, HEX);
	Serial.print(data->b9, HEX);
	Serial.print(data->b10, HEX);
	Serial.println(data->b11, HEX);
}

struct button_data_s get_button_data() {
	static struct button_data_s instance;
	uint16_t currtouched = capacs.touched();
	instance.b0 = currtouched & _BV(0);
	instance.b1 = (currtouched & _BV(1)) >> 1;
	instance.b2 = (currtouched & _BV(2)) >> 2;
	instance.b3 = (currtouched & _BV(3)) >> 3;
	instance.b4 = (currtouched & _BV(4)) >> 4;
	instance.b5 = (currtouched & _BV(5)) >> 5;
	instance.b6 = (currtouched & _BV(6)) >> 6;
	instance.b7 = (currtouched & _BV(7)) >> 7;
	instance.b8 = (currtouched & _BV(8)) >> 8;
	instance.b9 = (currtouched & _BV(9)) >> 9;
	instance.b10 = (currtouched & _BV(10)) >> 10;
	instance.b11 = (currtouched & _BV(11)) >> 11;
	return instance;
}

void send_packet(struct packet_data_s* data) {
	if (data->length == 0) {
		return;
	}

	bt.print(data->device_id);
	Serial.print(data->device_id, HEX);
	bt.print(data->packet_id);
	Serial.print(data->packet_id, HEX);
	bt.print(data->length);
	Serial.println(data->length, HEX);
	for (int i = 0; i < data->length; ++i) {
		bt.print(data->data[i]);
		Serial.print(data->data[i], HEX);
	}
	Serial.println("");
}

void setup() {
	delay(500);

	Serial.begin(115200);
	while (!Serial) { //keep from starting too fast
		delay(10);
	}
	Serial.println("Haptyk");

	if (!bt.begin(BT_VERBOSE)) {
		error("Failed to begin BT module");
	}
	bt.echo(false);
	bt.info();
	bt.verbose(false);

	while (!bt.isConnected()) {
		delay(500);
	}
	Serial.println("Connected");

	bt.setMode(BLUEFRUIT_MODE_DATA);

	randomSeed(analogRead(0));

	packet.device_id = HW_ID;

	if (!capacs.begin(0x5A)) {
		error("MPR121 not found, check wiring?");
	}
	Serial.println("MPR121 found");
}

#define BUTTON_IS_NOW_PRESSED(x) \
	button_data_prev.x == 0 && button_data_curr.x == 1

#define BUTTON_NOW_NOT_PRESSED(x) \
	button_data_prev.x == 1 && button_data_curr.x == 0

void loop() {

	// Clear any pending data to be read
	while (bt.available()) {
		bt.read();
	}

	button_data_curr = get_button_data();
	print_button_data(&button_data_curr);
	packet.length = 0;

	// Buttons that were released, now pressed
	if (BUTTON_IS_NOW_PRESSED(b0)) { packet.data[packet.length++] = 0x01; }
	if (BUTTON_IS_NOW_PRESSED(b1)) { packet.data[packet.length++] = 0x02; }
	if (BUTTON_IS_NOW_PRESSED(b2)) { packet.data[packet.length++] = 0x03; }
	if (BUTTON_IS_NOW_PRESSED(b3)) { packet.data[packet.length++] = 0x11; }
	if (BUTTON_IS_NOW_PRESSED(b4)) { packet.data[packet.length++] = 0x12; }
	if (BUTTON_IS_NOW_PRESSED(b5)) { packet.data[packet.length++] = 0x13; }
	if (BUTTON_IS_NOW_PRESSED(b6)) { packet.data[packet.length++] = 0x21; }
	if (BUTTON_IS_NOW_PRESSED(b7)) { packet.data[packet.length++] = 0x22; }
	if (BUTTON_IS_NOW_PRESSED(b8)) { packet.data[packet.length++] = 0x23; }
	if (BUTTON_IS_NOW_PRESSED(b9)) { packet.data[packet.length++] = 0x31; }
	if (BUTTON_IS_NOW_PRESSED(b10)) { packet.data[packet.length++] = 0x32; }
	if (BUTTON_IS_NOW_PRESSED(b11)) { packet.data[packet.length++] = 0x33; }

	if (packet.length > 0) {
		packet.packet_id = PACKET_ID_PRESSED;
		send_packet(&packet);
		packet.length = 0;
	}

	// Buttons that were pressed, now released
	if (BUTTON_NOW_NOT_PRESSED(b0)) { packet.data[packet.length++] = 0x01; }
	if (BUTTON_NOW_NOT_PRESSED(b1)) { packet.data[packet.length++] = 0x02; }
	if (BUTTON_NOW_NOT_PRESSED(b2)) { packet.data[packet.length++] = 0x03; }
	if (BUTTON_NOW_NOT_PRESSED(b3)) { packet.data[packet.length++] = 0x11; }
	if (BUTTON_NOW_NOT_PRESSED(b4)) { packet.data[packet.length++] = 0x12; }
	if (BUTTON_NOW_NOT_PRESSED(b5)) { packet.data[packet.length++] = 0x13; }
	if (BUTTON_NOW_NOT_PRESSED(b6)) { packet.data[packet.length++] = 0x21; }
	if (BUTTON_NOW_NOT_PRESSED(b7)) { packet.data[packet.length++] = 0x22; }
	if (BUTTON_NOW_NOT_PRESSED(b8)) { packet.data[packet.length++] = 0x23; }
	if (BUTTON_NOW_NOT_PRESSED(b9)) { packet.data[packet.length++] = 0x31; }
	if (BUTTON_NOW_NOT_PRESSED(b10)) { packet.data[packet.length++] = 0x32; }
	if (BUTTON_NOW_NOT_PRESSED(b11)) { packet.data[packet.length++] = 0x33; }

	if (packet.length > 0) {
		packet.packet_id = PACKET_ID_RELEASED;
		send_packet(&packet);
		packet.length = 0;
	}

	button_data_prev = button_data_curr;

	delay(5000);
}