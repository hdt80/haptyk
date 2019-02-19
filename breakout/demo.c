#define __AVR_ATmega32U4__
#define F_CPU 16000000UL
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
//#include <VirtualSerial.h>

#include "i2c_master.h"
/*
void init_uart(uint16_t baudrate){

	//uint16_t UBRR_val = (F_CPU / 16) / (baudrate - 1);

	UBRR1H = UBRRH_VALUE;
	UBRR1L = UBRRL_VALUE;

	UCSR1B |= (1<<TXEN1) | (1<<RXEN1);// | (1<<RXCIE1); // UART TX 
	UCSR1C |= (1<<USBS1) | (3<<UCSZ10); //Modus Asynchron 8N1 (8 databits, No Parity, 1 Stopbit)
}

void uart_putc(unsigned char c){

    //loop_until_bit_is_set(UCSR1A, UDRE1);
	while(!(UCSR1A & (1<<UDRE1))) 
    {
       //_delay_ms(500);
       //PINB |= (1 << DDB6); 

    } // wait until sending is possible
	UDR1 = c; // output character saved in c
}

void uart_puts(char *s){
	//while(*s != '!'){
		uart_putc(*s);
	//	s++;
	//}
}
*/

int main (void)
{
    PORTC |= (1 << DDC7);
    PORTB |= (1 << DDB6);
    //DDxn = 1 means output
    DDRC |= (1 << DDC7);
    DDRB |= (1 << DDB6);
    //ddrb = 1 & ddb5
    //PORtxn = 1
    //init_uart(9600);
    _delay_ms(1000);
    //char hello[] = "hello world!";

    for (;;) {
        PINC |= (1 << DDC7);
        _delay_ms(200);   
        UDR1 = 'f';
        //uart_puts(hello);
    }

    return (0);
}
