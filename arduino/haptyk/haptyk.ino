#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "custom_MPR121.c"
#include "MPR121_Addrs.h"

#include "config.h"
#include "structs.h"
#include "bt_gatt.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// Hardware SPI
// CS = 8, IRQ = 7, RST = 4
Adafruit_BluefruitLE_SPI bt(HW_SPI_CS, HW_SPI_IRQ, HW_SPI_RST);

#define DEBUG 1

#ifdef DEBUG

void MPR121_reg_dump() {
    Serial.println("BEGIN MPR121 REGISTER DUMP");
	for (uint8_t i = 0; i < 0x7F; ++i) {
		Serial.print("$");
		Serial.print(i, HEX);
		Serial.print(": 0x");
		Serial.println(readRegister8(i), HEX);
	}
    Serial.println("END MPR121 REGISTER DUMP");
}

#endif DEBUG

// Connected to the breakout board?
u8 cs_connected = 0x00;

// Shift register to debounce the button for disconnecting to the
//		connected target device
u8 btn_reconnect = 0x00;

// Shift register to debounce the button for recalibrating the cap. sensors
u8 btn_recalibrate = 0x00;

// If Haptyk is currently connected to a device
u8 bt_connected = 0x00;

// Response after sending an AT command, used for initilization
u8 bt_response[65];

// Index of the GATT characteristic to write to when updating the button data
u8 gatt_index;

// Are we currently in standby mode?
u8 is_standby;

// Setup pins for using the control buttons
//
void setup_control_buttons() {
	// Setups an internal pull-up
	DDRB |= (1 << DDB7) | (1 << DDB6) | (1 << DDB5);
	PORTB |= (1 << PORTB7) | (1 << PORTB6) | (1 << PORTB5);
}

// Get the state of each sensor button
//
// returns: A button_data_s containing the state of each sensor button
//
struct button_data_s get_button_data() {
	static struct button_data_s instance;

	if (cs_connected != 0) {
		uint16_t currtouched = touched_status();
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
		instance.b0 = random(0, 3);
		instance.b1 = random(0, 3);
		instance.b2 = random(0, 3);
		instance.b3 = random(0, 3);
		instance.b4 = random(0, 3);
		instance.b5 = random(0, 3);
		instance.b6 = random(0, 3);
		instance.b7 = random(0, 3);
		instance.b8 = random(0, 3);
		instance.b9 = random(0, 3);
		instance.b10 = random(0, 3);
		instance.b11 = random(0, 3);
    }

	return instance;
}

// Function Arduino calls when the program starts running
//
void setup() {
	Serial.begin(115200);
	while (!Serial) { // Don't start without the serial interface
		delay(10);
	}
	Serial.println("Haptyk");

	bt_setup(&bt);
	setup_gatt_service(&bt);

	// Using the MPR121 for button sensors?
	if (init_MPR121()) {
		cs_connected = 1;
	} else { // Nope? Use random data
		cs_connected = 0;
		randomSeed(analogRead(0));
	}
	Serial.print("Using the MPR121? ");
	Serial.println(cs_connected);

	setup_control_buttons();

	pinMode(LED_BUILTIN, OUTPUT);
}

// Control loop for control buttons
//
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

	u8 btn_recalibrate_curr = PINB & 0x40;
	if (btn_recalibrate != 0x00 && btn_recalibrate_curr == 0x00) {
		Serial.println("Recalibate");
		init_MPR121();
	}
	btn_recalibrate = btn_recalibrate_curr;

	is_standby = (PINB & 0x20) == 0x00;
}

void loop() {
	ctl_loop(); // Perform control loop for any control button changes

	if (is_standby != 0x00) {
		digitalWrite(LED_BUILTIN, 1);
		_delay_ms(100);
		return;
	} else {
		digitalWrite(LED_BUILTIN, 0);
	}

	button_data_curr = get_button_data();

	bt_loop(&bt);

	button_data_prev = button_data_curr;

	_delay_ms(10);
}
