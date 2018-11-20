
const enum EVENT_TYPE {
	BUTTON_PRESSED	= 0x01,
	BUTTON_RELEASED = 0x02
};

class bluetooth {

	int pairedDeviceID;

	// Pair with the Bluetooth device which will recieve all the sensor data. If
	// Haptyk is currently paired with a device is will be unpaired
	//
	// returns: The ID of the device Haptyk has paired with. -1 means no pairing
	//		was completed
	int enterPairingMode() {

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

	// Send data to the Bluetooth device that a button has changed it state,
	// either going from released to pressed (pressed = 1), or pressed to
	// releated (pressed = 0)
	//
	// buttonID - ID of the button that was pressed. The first nibble will be
	//		what slot the button was plugged into, and what index of the button
	//		on the slot is.
	//	Example:
	//		0x12 -> First slot, 3th button (0 indexed)
	// pressed - If the button is going from released to pressed, or pressed
	//		to released
	//
	// returns: If the operation was successful
	boolean sendButtonPressed(byte buttonID, boolean pressed) {
		byte[2] packet = new byte[2];
		packet[1] = buttonID;

		if (pressed == true) {
			packet[0] = EVENT_TYPE.BUTTON_PRESSED;
		} else {
			packet[0] = EVENT_TYPE.BUTTON_RELEASED;
		}

		return sendData(packet, 2);
	}

}
