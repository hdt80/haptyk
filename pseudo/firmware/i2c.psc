
class i2c {

	// Select a slave address to use and begin the transmission.
	// After sendData() or readData() is called the slave will 
	// no longer be selected and must be selected again
	boolean selectSlave(slaveID) {

	}

	// Send data to the selected i2c slave.
	//
	// data - Byte array to be send. 0 will be sent first
	// length - How many bytes in data to send
	//
	// returns: If the operation was successful
	boolean sendData(byte[] data, int length) {

	}

	// Read data from the selected i2c slave
	//
	// data - Where the read data will be stored
	// length - How many bytes are expected to be read
	//
	// returns: If the operation was successful
	boolean readData(byte[] data, int length) {

	}

}
