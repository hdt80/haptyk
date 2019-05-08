#ifndef HAPTYK_CONNECT_H
#define HAPTYK_CONNECT_H

#define HAPTYK_READ_UUID "00000004-0000-1000-8000-00805f9b34fb"

#include <stdint.h>
#include "gattlib.h"

extern uuid_t bt_read_char;
extern gatt_connection_t* bt_connection;

struct haptyk_buttons_t {
        uint8_t b0;
        uint8_t b1;
        uint8_t b2;
        uint8_t b3;
        uint8_t b4;
        uint8_t b5;
        uint8_t b6;
        uint8_t b7;
        uint8_t b8;
        uint8_t b9;
        uint8_t b10;
        uint8_t b11;
};

uint8_t haptyk_init(const char* address);
void haptyk_disconnect(const char* address);
uint8_t haptyk_get_data(struct haptyk_buttons_t * data);

void haptyk_print(struct haptyk_buttons_t* data);

#endif
