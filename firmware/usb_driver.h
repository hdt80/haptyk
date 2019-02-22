#pragma once

#include <stdio.h>

int usb_send(char c, FILE* stream);

int usb_read(FILE* stream);

int usb_ready(void);
