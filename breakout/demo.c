#ifndef __AVR_ATmega32U4__
    #define __AVR_ATmega32U4__
#endif

#define F_CPU 16000000UL
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>

//#include "i2c_master.c"
#include "i2c.c"
#include "MPR121_breakout.h"
#define MPR121_ADDR 0x5A

void error_light() { PORTC |= (1 << DDC6); }

void init_MPR121() {
    uint8_t USL = 200;
    uint8_t startAll = 0x0F;
    uint8_t ACE_ARE = 0x03;
    uint8_t LSL = 180;
    uint8_t TL = 100;

    //reset everything back to default
    writeByte(MPR121_ADDR, SOFT_RESET, 0x63);

    connect_i2c(MPR121_ADDR, 0);
    send_i2c(UPSIDE_LIMIT);
    send_i2c(USL);
    stop_i2c();

    connect_i2c(MPR121_ADDR, 0);
    send_i2c(AUTO_CONFIG_REG_0);
    send_i2c(ACE_ARE);
    stop_i2c();

    connect_i2c(MPR121_ADDR, 0);
    send_i2c(LOWSIDE_LIMIT);
    send_i2c(LSL);
    stop_i2c();

    connect_i2c(MPR121_ADDR, 0);
    send_i2c(TARGET_LEVEL);
    send_i2c(TL);
    stop_i2c();

    //set touch and release threshholds
    for (uint8_t i = 0; i < 12; i++) 
    {
        connect_i2c(MPR121_ADDR, 0);
        send_i2c(ELE0_TOUCH_THRESHOLD + (i * 2));
        send_i2c(12);
        stop_i2c();

        connect_i2c(MPR121_ADDR, 0);
        send_i2c(ELE0_RELEASE_THRESHOLD + 1 + (i * 2));
        send_i2c(6);
        stop_i2c();
    }

    writeByte(MPR121_ADDR, NHDR, 0x01);

    writeByte(MPR121_ADDR, MHDR, 0x01);
    writeByte(MPR121_ADDR, NHDR, 0x01);
    writeByte(MPR121_ADDR, NCLR, 0x0E);
    writeByte(MPR121_ADDR, FDLR, 0x00);

    writeByte(MPR121_ADDR, MHDF, 0x01);
    writeByte(MPR121_ADDR, NHDF, 0x05);
    writeByte(MPR121_ADDR, NCLF, 0x01);
    writeByte(MPR121_ADDR, FDLF, 0x00);

    writeByte(MPR121_ADDR, NHDT, 0x00);
    writeByte(MPR121_ADDR, NCLT, 0x00);
    writeByte(MPR121_ADDR, FDLT, 0x00);

    connect_i2c(MPR121_ADDR, 0);
    send_i2c(ELE_CONFIGURATION);
    send_i2c(startAll);
    stop_i2c();

    /*
    if (i2c_writeReg(MPR121_ADDR, UPSIDE_LIMIT, &USL, 1)) {
        error_light();
        while(1) {}
    }
    if (1 == i2c_writeReg(MPR121_ADDR, AUTO_CONFIG_REG_0, &ACE_ARE, 1)){
        error_light();
    }
    if (i2c_writeReg(MPR121_ADDR, LOWSIDE_LIMIT, &LSL, 1)) {
        error_light();
    }
    if (i2c_writeReg(MPR121_ADDR, TARGET_LEVEL, &TL, 1)) {
        error_light();
    }
    if (i2c_writeReg(MPR121_ADDR, ELE_CONFIGURATION, &startAll, 1)){
        error_light();
    }
    */
}

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
    init_i2c();
    init_MPR121();
    
    uint8_t on = 1;

    connect_i2c(MPR121_ADDR, 0);
    send_i2c(NCLR);
    connect_i2c(MPR121_ADDR, 1);
    uint8_t check = read_i2c();
    stop_i2c();

    if (check != 0x0E) 
    {
        error_light();
        while (1) {}
    }

    for (;;) {
        /*
        if (0 != i2c_readReg(MPR121_ADDR, TOUCH_STATUS_0, &on, 1))
        {
            //error_light();
        }
        */
        connect_i2c(MPR121_ADDR, 0);
        send_i2c(TOUCH_STATUS_0);
        connect_i2c(MPR121_ADDR, 1);
        on = read_i2c();
        stop_i2c();

        PINC |= (1 << DDC7); //heartbeat
        if (on != 0)
        {
            PORTB |= (1 << DDB6);
            while (1) {}
        }
        else 
        {
            PORTB &= ~(1 << DDB6);
        }

        _delay_ms(200);   
        //UDR1 = 'f';
        //uart_puts(hello);
    }

    return (0);
}
