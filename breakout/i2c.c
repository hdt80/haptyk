/*
 * Originally by: Benjamin Frank
 * Edited by: Garrett Fechter
 */ 
#include <util/twi.h>
#include "i2c.h"
/*
 * Initialization of the I2C bus
 */
void init_i2c() {
	PORTD &= ~(1 << 0); //Port D0 SCL
	PORTD &= ~(1 << 1); //Port D1 SDA
	TWBR = ((( F_CPU / SCL_CLOCK ) - 16) / 2); 
	TWSR = 0; 
	TWCR = ( 1 << TWEN ); // enable the i2c bus f
}

/*
 * Sends the start condition
 * adr: address of the hardware
 * w: read or write flag
 */
void connect_i2c(uint8_t adr, uint8_t w) {

	start_i2c();
	send_address_i2c(adr,w);
}

/*
 * Sends the start condition
 */
void start_i2c() {
	uint8_t   twstatus;
	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	// wait until transmission completed
	while ( !(TWCR & (1<<TWINT)))	;
	twstatus = TW_STATUS & 0xF8;
	if ((twstatus != TW_START) && (twstatus != TW_REP_START)){
	}
}

/*
 * 
 * adr: address of the hardware
 * w: read or write flag
 */
void send_address_i2c(uint8_t adr, uint8_t w) {
	uint8_t   twstatus;
	TWDR = (adr<<1) | w ;
	TWCR = (1 << TWINT) | (1<<TWEN);
	
	while(!(TWCR & (1 << TWINT)));
	twstatus = TW_STATUS & 0xF8;
	if ( (twstatus != TW_MT_SLA_ACK) && (twstatus != TW_MR_SLA_ACK) ) {
	}
}

/*
 * sends the stop condition
 */
void stop_i2c() {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	while(TWCR & (1 << TWSTO));
}

/*
 * sends one byte to the I2C hardware
 * data: byte to send
 */
void send_i2c(uint8_t data) {
	uint8_t   twstatus;
	// send data to the previously addressed device
	TWDR = data;
	TWCR = (1 << TWINT) | (1<<TWEN);
	
	// wait until transmission completed
	while(!(TWCR & (1 << TWINT)));
	twstatus = TW_STATUS & 0xF8;
	if( twstatus != TW_MT_DATA_ACK){
		
	}
}

/*
 * receive one byte of the I2C hardware, followed by a stop condition
 */
int8_t read_i2c() {
	TWCR = (1 << TWINT) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
	return TWDR;
}

/*
 * receive one byte of the I2C hardware, request more data from hardware
 */
int8_t readAck_i2c() {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1<<TWEA);
	while(!(TWCR & (1 << TWINT)));
	return TWDR;
}

/*
 * Write a byte to a register
 */
void writeByte(uint8_t device, uint8_t adr, uint8_t data) 
{
    connect_i2c(device, 0);
    send_i2c(adr);
    send_i2c(data);
    stop_i2c();
}

/*
 * Read a byte from a register
 */
int8_t readByte(uint8_t device, uint8_t adr) 
{
    connect_i2c(device, 0);
    send_i2c(adr);
    connect_i2c(device, 1);
    int8_t data = read_i2c();
    stop_i2c();
    return data;
}
