#define BUFFER_SIZE 128
#define BT_VERBOSE	0

#define HW_ID		0x42

#define HW_SPI_CS	8
#define HW_SPI_IRQ	7
#define HW_SPI_RST	4

#define PACKET_ID_PRESSED	0x01
#define PACKET_ID_RELEASED	0x02

#define USE_CAP_SENSORS		0x00

// Unrecoverable error has occured and Haptyk cannot continue functioning
//
// err - Error that occured. Will be printed to the serial interface
//
void error(const char* err) {
	Serial.println(err);
	while(1);
}
