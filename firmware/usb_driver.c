#include "usb_driver.h"

#include "Descriptors.h"

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

int usb_send(char c, FILE* stream) {
	static char* newline = "\n";

	Endpoint_SelectEndpoint(CDC_TX_EPADDR);

	Endpoint_Write_Stream_LE(&c, 1, NULL);
	Endpoint_Write_Stream_LE(newline, 1, NULL);
	USB_USBTask();

	return 0;
}

int usb_read(FILE* stream) {
	return 0;
}
