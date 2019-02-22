#include "usb_driver.h"

#include "bt.h"
#include "Descriptors.h"
#include "print.h"
#include "spi.h"

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

void setup(void);

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

/** Contains the current baud rate and other settings of the virtual serial port. While this demo does not use
 *  the physical USART and thus does not use these settings, they must still be retained and returned to the host
 *  upon request or the host will assume the device is non-functional.
 *
 *  These values are set by the host via a class-specific request, however they are not required to be used accurately.
 *  It is possible to completely ignore these value or use other settings as the host is completely unaware of the physical
 *  serial link characteristics and instead sends and receives data in endpoint streams.
 */
static CDC_LineEncoding_t LineEncoding = {
	.BaudRateBPS = 0,
	.CharFormat  = CDC_LINEENCODING_OneStopBit,
	.ParityType  = CDC_PARITY_None,
	.DataBits    = 8
};

int main(void) {
	setup();

	u8 a = 0;

	while(1) {
		ht_logf("test message %x\n", ++a);

		_delay_ms(100);
	}

	return 0;
}

void setup(void) {
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	clock_prescale_set(clock_div_1);

	USB_Init();
	spi_init();
	bt_init();

	GlobalInterruptEnable();

	PORTC |= (1 << DDC7);
	DDRC |= (1 << DDC7);

	while (1) {
		bt_connected();
		_delay_ms(500);
	}

	while (bt_connected() == 0) {
		_delay_ms(500);
	}
	PINC |= (1 << DDC7);
}

void EVENT_USB_Device_Connect(void) { }

void EVENT_USB_Device_Disconnect(void) { }

void EVENT_USB_Device_ConfigurationChanged(void) {
	bool ConfigSuccess = true;

	ConfigSuccess &= Endpoint_ConfigureEndpoint(
			CDC_NOTIFICATION_EPADDR, EP_TYPE_INTERRUPT, CDC_NOTIFICATION_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(
			CDC_TX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(
			CDC_RX_EPADDR, EP_TYPE_BULK,  CDC_TXRX_EPSIZE, 1);

	LineEncoding.BaudRateBPS = 0;
}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
 *  the device from the USB host before passing along unhandled control requests to the library for processing
 *  internally.
 */
void EVENT_USB_Device_ControlRequest(void) {
	switch (USB_ControlRequest.bRequest) {
		case CDC_REQ_GetLineEncoding:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				/* Write the line coding data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
				Endpoint_ClearOUT();
			}

			break;

		case CDC_REQ_SetLineEncoding:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE)) {
				Endpoint_ClearSETUP();
				/* Read the line coding data in from the host into the global struct */
				Endpoint_Read_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
				Endpoint_ClearIN();
			}
			break;

		case CDC_REQ_SetControlLineState:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE)) {
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();
				/* NOTE: Here you can read in the line state mask from the host, to get the current state of the output handshake
				   lines. The mask is read in from the wValue parameter in USB_ControlRequest, and can be masked against the
				   CONTROL_LINE_OUT_* masks to determine the RTS and DTR line states using the following code:
				   */
			}
			break;
	}
}
