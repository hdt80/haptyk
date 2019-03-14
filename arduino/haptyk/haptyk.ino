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

#ifdef DEBUG

void MPR121_reg_dump() {
    Serial.println("BEGIN MPR121 REGISTER DUMP");
      for (uint8_t i=0; i<0x7F; i++) {
          Serial.print("$"); Serial.print(i, HEX); 
          Serial.print(": 0x"); Serial.println(readRegister8(i));
      }
    Serial.println("END MPR121 REGISTER DUMP");
}

#endif DEBUG

// Hardware SPI
// CS = 8, IRQ = 7, RST = 4

Adafruit_BluefruitLE_SPI bt(HW_SPI_CS, HW_SPI_IRQ, HW_SPI_RST);
Adafruit_MPR121 capacs = Adafruit_MPR121();

// Connected to the breakout board?
unsigned char cs_connected = 0x00;

// Shift register to debounce the button for disconnecting to the
//		connected target device
u8 btn_reconnect = 0x00;

// Shift register to debounce the button for recalibrating the cap. sensors
u8 btn_recalibrate = 0x00;

// If Haptyk is currently connected to a device
u8 bt_connected = 0x00;

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

void print_bluetooth_packet(const struct packet_data_s* data) {

}

void setup_control_buttons() {
	// Setups an internal pull-up
	DDRB |= (1 << DDB7);
	PORTB |= (1 << PORTB7);
}

struct button_data_s get_button_data() {
	static struct button_data_s instance;

	if (cs_connected != 0) {
        //check for OCVF fault condition
        if (0x80 & capacs.readRegister8(MPR121_TOUCHSTATUS_H)) 
            capacs.begin(0x5A); //and reset if occured

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
	} else {
		instance.b0 = random(0, 2);
		instance.b1 = random(0, 2);
		instance.b2 = random(0, 2);
		instance.b3 = random(0, 2);
		instance.b4 = random(0, 2);
		instance.b5 = random(0, 2);
		instance.b6 = random(0, 2);
		instance.b7 = random(0, 2);
		instance.b8 = random(0, 2);
		instance.b9 = random(0, 2);
		instance.b10 = random(0, 1);
		instance.b11 = random(0, 1);
	}

	return instance;
}

void send_packet(struct packet_data_s* data) {
	if (data->length == 0) {
		return;
	}

	u8 buffer[16];
	buffer[0] = data->device_id;
	buffer[1] = data->packet_id;
	buffer[2] = data->length;
	for (int i = 0; i < data->length; ++i) {
		buffer[3 + i] = data->data[i];
	}

	// Write the packet in a single command
	bt.write(buffer, 3 + data->length);
}

void setup() {
	Serial.begin(115200);
	while (!Serial) { //keep from starting too fast
		delay(10);
	}
	Serial.println("Haptyk");

	if (!bt.begin(BT_VERBOSE)) {
		error("Failed to begin BT module");
	}
	Serial.println("Began BT module");

	bt.echo(false);
	bt.info();
	bt.verbose(false);

	bt.setMode(BLUEFRUIT_MODE_DATA);

	randomSeed(analogRead(0));

	packet.device_id = HW_ID;

	if (capacs.begin(0x5A)) {
		cs_connected = 1;
	}
	Serial.print("Using the MPR121? ");
	Serial.println(cs_connected);

	setup_control_buttons();
}

#define BUTTON_IS_NOW_PRESSED(x) \
	button_data_prev.x == 0 && button_data_curr.x == 1

#define BUTTON_NOW_NOT_PRESSED(x) \
	button_data_prev.x == 1 && button_data_curr.x == 0

// Bluetooth loop
void bt_loop() {
	// Clear any pending data to be read
	while (bt.available()) {
		bt.read();
	}

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
}

// Control loop
void ctl_loop() {
	// Only check the reconnect button if it isn't pressed
	if (bt_connected != 0x00) {
		u8 btn_reconnect_curr = PINB & 0x80;
		if (btn_reconnect == 0x80 && btn_reconnect_curr == 0x00) {
			Serial.println("disconnected");
			bt_connected = 0x00;
			bt.disconnect();
		}
		btn_reconnect = btn_reconnect_curr;
	}

}

void loop() {
	ctl_loop();

	button_data_curr = get_button_data();

	if (bt_connected == 0x00) {
		Serial.print("Connecting... ");
		while (!bt.isConnected()) {
			_delay_ms(10);
		}
		bt_connected = 0x01;
		Serial.println("done");
	} else {
		bt_loop();
	}

	button_data_prev = button_data_curr;

	_delay_ms(10);
}
