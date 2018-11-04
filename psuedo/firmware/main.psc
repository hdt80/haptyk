

////////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////////

// How many bytes long the data from the i2c capacitive sensor deivce will be
const int I2C_CAP_DATA_LENGTH;

// Byte array containing the command to send to the cap. sensor device to
// get the capacitive sensor data
const byte[] I2C_CAP_GET_COMMAND;

////////////////////////////////////////////////////////////////////////////////
// Data structs
////////////////////////////////////////////////////////////////////////////////

struct gyro_data {
	// Gyroscope data
	float gyro_x;
	float gyro_y;
	float gyro_z;

	// Accelerometer data
	float accel_x;
	float accel_y;
	float accel_z;

	// Magnometer data
	float magno_x;
	float magno_y;
	float magno_z;
}

////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////

boolean standByMode;

main() {
	
	while (true) {
		if (controlModule.isStandByPressed() == true) {
			standByMode = !standByMode;
		}

		if (standByMode == true) {
			sleep(1s);
			continue; // Continue to next iteration of loop
		}

		byte[12] capData = getCapacitiveSensorData()

		var dofData = getNineSensorData()
		Bluetooth.sendData(dofData)
	}

}

byte[12] getCapacitveSensorData() {

	byte[12] data = new byte[12];

	if (i2c::selectSlave(settings::i2cCapAddr) == false) {
		LOG_ERROR("Failed to connect to i2c device %x", settings::i2cCapAddr);
		return data;
	}

	// Command to get the data has been sent
	if (i2c::sendData(I2C_CAP_GET_COMMAND, I2C_CAP_DATA_LENGTH) == false) {
		LOG_ERROR("Failed to send command %x", settings::i2cCapAddr);
		return data;
	}

	// Failed to read the data from the i2c slave
	if (i2c::readData(data, 12) == false) {
		LOG_ERROR("");
	}

	return data;
}

gyro_data getNineSensorData() {
	gyro_data data = new gyro_data();

	


}
