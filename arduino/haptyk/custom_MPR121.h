#ifndef CUSTOM_MPR121_H
#define CUSTOM_MPR121_H

#ifndef __AVR_ATmega32U4__
#define __AVR_ATmega32U4__
#endif

#ifndef BAUD
#define BAUD 9600
#endif

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

uint8_t init_MPR121(void);
int16_t touched_status(void);
uint8_t readRegister8(uint8_t adr);

#endif CUSTOM_MPR121_H
