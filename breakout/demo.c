#define __AVR_ATmega32U4__
#define F_CPU 8000000UL
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
//#include <VirtualSerial.h>

#include "i2c_master.c"
#include "MPR121_breakout.h"
#define MPR121_ADDR 0x5A

//void error_light() {PORTC |= (1 << DDC6);}

void init_MPR121() {
    uint8_t ACE_ARE = 0x03;
    i2c_start(MPR121_ADDR);
    i2c_write(AUTO_CONFIG_REG_0);
    i2c_write(ACE_ARE);
    i2c_stop();
    //i2c_writeReg(MPR121_ADDR, AUTO_CONFIG_REG_0, &ACE_ARE, 1);
    uint8_t USL = 200;
    uint8_t LSL = 180;
    uint8_t TL = 100;
    i2c_writeReg(MPR121_ADDR, UPSIDE_LIMIT, &USL, 1);
    i2c_writeReg(MPR121_ADDR, LOWSIDE_LIMIT, &LSL, 1);
    i2c_writeReg(MPR121_ADDR, TARGET_LEVEL, &TL, 1);
    uint8_t startAll = 0x0F;
    i2c_writeReg(MPR121_ADDR, ELE_CONFIGURATION, &startAll, 1);
}

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
    PORTC &= ~(1 << DDC6);
    PORTB |= (1 << DDB6);
    //DDxn = 1 means output
    DDRC |= (1 << DDC7) | (1 << DDC6);
    DDRB |= (1 << DDB6);
    //ddrb = 1 & ddb5
    //PORtxn = 1
    //init_uart(9600);
    //
    i2c_init();
    init_MPR121();
    
    //char hello[] = "hello world!";
    uint8_t on = 0;

    for (;;) {
        //i2c_readReg(MPR121_ADDR, TOUCH_STATUS_0, &on, 1);
        PINC |= (1 << DDC7); //heartbeat
        if (on != 0)
            PORTB |= (1 << DDB6);
        else
            PORTB &= ~(1 << DDB6);

        _delay_ms(200);   
        //UDR1 = 'f';
        //uart_puts(hello);
    }

    return (0);
}
