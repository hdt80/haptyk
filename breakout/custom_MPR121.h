#ifndef CUSTOM_MPR121_H
#define CUSTOM_MPR121_H


#ifndef __AVR_ATmega32U4__
#define __AVR_ATmega32U4__
#endif

#define BAUD 9600

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

void init_MPR121();
int16_t touched_status(void);


#endif CUSTOM_MPR121_H
