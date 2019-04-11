#pragma once

// Contains the status of each button sensor
//
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
