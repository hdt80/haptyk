#ifndef HAPTYK_CONNECT_H
#define HAPTYK_CONNECT_H

#define HAPTYK_READ_UUID "00000004-0000-1000-8000-00805f9b34fb"

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

void haptyk_init(const char* address);

struct haptyk_buttons_t haptyk_get_data();

#endif
