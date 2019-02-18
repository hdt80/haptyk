#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#define BAUD 9600

#include <stdio.h>

#define USE_STATIC_OPTIONS \
	(USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL)
#define USB_DEVICE_ONLY
#define USE_FLASH_DESCRIPTORS
#define FIXED_CONTROL_ENDPOINT_SIZE      8
#define FIXED_NUM_CONFIGURATIONS         1

// Initalize the UART driver. F_CPU must be already defined for this call to
//		function. AFter this call, the UART driver can be read and send data.
void uart_init(void);

// Send a single character to the stream of the UART
// c - Character to be put
// stream - Pointer to the file stream that the char will be sent to
// returns: 0, always. This method must have a return however for FDEV
int uart_send(char c, FILE* stream);

// Read a single byte from the UART stream
// stream - Pointer to the file stream that the UART driver is
// returns:
int uart_read(FILE* stream);

void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

#endif // UART_DRIVER_H
