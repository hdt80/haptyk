/*
 * Originally by: Benjamin Frank
 * Edited by: Garrett Fechter
 */

#ifndef I2C_H
#define I2C_H

#include <util/twi.h>
#define I2C_READ 1
#define I2C_WRITE 0
#define SCL_CLOCK 400000L

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

void init_i2c();
void send_i2c(uint8_t);
int8_t read_i2c();
int8_t readAck_i2c();
void send_address_i2c(uint8_t adr, uint8_t w);
void start_i2c();
void connect_i2c(uint8_t,uint8_t);
void stop_i2c();
void writeByte(uint8_t device, uint8_t adr, uint8_t data);
int8_t readByte(uint8_t device, uint8_t adr);

#endif 
