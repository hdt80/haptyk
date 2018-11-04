
// Compiled time constants for the microcontroller to use
class settings {

	// Pin to a button that will be used to check if Haptyk should enter
	// pairing mode to another Bluetooth device
	int pairPin;

	// Pin to a button that will be used to check if Haptyk should enter
	// stand by mode. If Haptyk is in stand by mode no Bluetooth data will be
	// sent and no sensor data will be pulled for
	int standByPin;

	// Pin to a button that will be used to check if Haptyk should recalibrate
	// its sensos (Capacitive sensors and 9DoF)
	int recalibrationPin;

	// Address of the i2c device to the capacitive sensor device
	int i2cCapAddr;

}
