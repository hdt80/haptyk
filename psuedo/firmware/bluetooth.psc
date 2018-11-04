
class bluetooth {

	int pairedDeviceID;

	// Pair with the Bluetooth device which will recieve all the sensor data. If
	// Haptyk is currently paired with a device is will be unpaired
	//
	// returns: The ID of the device Haptyk has paired with. -1 means no pairing
	//		was completed
	int enterPairing() {

	}

	// Get the ID of the Bluetooth Haptyk is paired to
	//
	// returns: The ID of the Bluetooth device Haptyk is paired to,
	//		or -1 if it is not paird
	int pairedID() {

	}

	// Send data to the connected Bluetooth device
	//
	// data - Byte array to be send. 0 will be sent first
	// length - Lenght of the parameter data
	//
	// returns: If the operation was successful
	boolean sendData(byte[] data, int length) {
		// Send the bytes to the paired device
	}


}
