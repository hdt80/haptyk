#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "config.h"

// Hardware SPI
// CS = 8, IRQ = 7, RST = 4

Adafruit_BluefruitLE_SPI bt(HW_SPI_CS, HW_SPI_IRQ, HW_SPI_RST);

struct packet_data_s {
	char device_id;
	char packet_id;
	char length;
	char data[12]; // Max 12
} packet;

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

struct button_data_s get_button_data() {
	static struct button_data_s instance;
	instance.b0 = random(0x00, 0x02);
	instance.b1 = random(0x00, 0x02);
	instance.b2 = random(0x00, 0x02);
	instance.b3 = random(0x00, 0x02);
	instance.b4 = random(0x00, 0x02);
	instance.b5 = random(0x00, 0x01);
	instance.b6 = random(0x00, 0x01);
	instance.b7 = random(0x00, 0x01);
	instance.b8 = random(0x00, 0x01);
	instance.b9 = random(0x00, 0x01);
	instance.b10 = random(0x00, 0x01);
	instance.b11 = random(0x00, 0x01);
	return instance;
}

void send_packet(struct packet_data_s* data) {
	if (data->length == 0) {
		return;
	}

	bt.print(data->device_id);
	Serial.println(data->device_id, HEX);
	bt.print(data->packet_id);
	bt.print(data->length);
	Serial.println(data->packet_id, HEX);
	for (int i = 0; i < data->length; ++i) {
		bt.print(data->data[i]);
		Serial.println(data->data[i], HEX);
	}
}

void setup() {
	delay(500);

	Serial.begin(115200);
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
	packet.length = 0;

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
