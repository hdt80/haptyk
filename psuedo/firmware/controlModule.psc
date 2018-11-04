
class controlModule {
	
	// Pin to check if the pairing mode should be turned on
	int pairPin;

	// Pin to check if Haptyk is in stand by mode
	int standByPin;

	// Pin to check if a recalibration should be performed
	int recalibratePin;

	// Check if the pairing button has been pressed
	//
	// returns: The result from digitalRead on pairPin
	bool isPairPressed() {
		return digitalRead(pairPin);
	}

	// Check if the stand by button has been pressed
	//
	// returns: The result from digitalRead on standByPin
	bool isStandByPressed() {
		return digitalRead(standByPin);
	}

	// Check if the recalibration button has been pressed
	//
	// returns: The result from digitalRead on recalibratePin
	bool isRecalibratePressed() {
		return digitalRead(recalibratePin);
	}

}

