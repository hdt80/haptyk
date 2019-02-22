#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#define BAUD 9600

#include <stdio.h>

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

#endif // UART_DRIVER_H
