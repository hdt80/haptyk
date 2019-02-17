/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * Simple AVR demonstration.  Controls a LED that can be directly
 * connected from OC1/OC1A to GND.  The brightness of the LED is
 * controlled with the PWM.  After each period of the PWM, the PWM
 * value is either incremented or decremented, that's all.
 *
 * $Id: demo.c 1637 2008-03-17 21:49:41Z joerg_wunsch $
 */
#define __AVR_ATmega32U4__
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

#include "i2c_master.h"

void init_uart(uint16_t baudrate){

	uint16_t UBRR_val = (F_CPU / 16) / (baudrate - 1);

	UBRR1H = UBRR_val >> 8;
	UBRR1L = UBRR_val;

	UCSR1B |= (1<<TXEN1) | (1<<RXEN1) | (1<<RXCIE1); // UART TX 
	UCSR1C |= (1<<USBS1) | (3<<UCSZ10); //Modus Asynchron 8N1 (8 databits, No Parity, 1 Stopbit)
}

void uart_putc(unsigned char c){

	while(!(UCSR1A & (1<<UDRE1))); // wait until sending is possible
	UDR1 = c; // output character saved in c
}

void uart_puts(char *s){
	while(*s){
		uart_putc(*s);
		s++;
	}
}

int main (void)
{
    //init_uart(9600);
    //uart_puts("hello world");

    PORTC |= (1 << DDC7);
    //DDxn = 1 means output
    DDRC |= (1 << DDC7);
    //ddrb = 1 & ddb5
    //PORtxn = 1
    for (;;) {
        PINC |= (1 << DDC7);
        _delay_ms(200);   
    }

    return (0);
}
