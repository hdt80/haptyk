#include "print.h"

#include <stdarg.h>
#include <stdio.h>

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>
#include "Descriptors.h"

void ht_log(char* str) {
	Endpoint_SelectEndpoint(CDC_TX_EPADDR);
	Endpoint_Write_Stream_LE(str, strlen(str), NULL);
	USB_USBTask();
}

void ht_logf(char* fmt, ...) {
	static char buffer[256];

	va_list vargs;
	va_start(vargs, fmt);

	vsnprintf(buffer, sizeof(buffer), fmt, vargs);

	va_end(vargs);

	ht_log(buffer);
}
