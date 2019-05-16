/*
   Blink

   Turns an LED on for one second, then off for one second, repeatedly.

   Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
   it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
   the correct LED pin independent of which board is used.
   If you want to know what pin the on-board LED is connected to on your Arduino
   model, check the Technical Specs of your board at:
https://www.arduino.cc/en/Main/Products

modified 8 May 2014
by Scott Fitzgerald
modified 2 Sep 2016
by Arturo Guadalupi
modified 8 Sep 2016
by Colby Newman

This example code is in the public domain.

http://www.arduino.cc/en/Tutorial/Blink
 */

#define FORWARD 2
#define BACKWARD 3
#define RIGHT 8
#define LEFT 9

// the setup function runs once when you press reset or power the board
void setup() {
	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(FORWARD, OUTPUT);
	pinMode(BACKWARD, OUTPUT);
	pinMode(RIGHT, OUTPUT);
	pinMode(LEFT, OUTPUT);
	digitalWrite(FORWARD, HIGH);
	digitalWrite(RIGHT, HIGH);
	digitalWrite(LEFT, HIGH);
	digitalWrite(BACKWARD, HIGH);
	Serial.begin(9600);
}

int incomingByte = 0;

// the loop function runs over and over again forever
void loop() {
	/*digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
	  digitalWrite(FORWARD, LOW);
	  digitalWrite(LEFT, LOW);
	  delay(250);
	  digitalWrite(FORWARD, HIGH);
	  digitalWrite(LEFT, HIGH);
	  delay(1000);*/                      // wait for a second
	if (Serial.available() > 0) {
		// read the incoming byte:
		incomingByte = Serial.read();

		// say what you got:
		Serial.print("Received: ");
		Serial.println(incomingByte);
		bool curr_forward = digitalRead(FORWARD);
		bool curr_backward = digitalRead(BACKWARD);
		bool curr_left = digitalRead(LEFT);
		bool curr_right = digitalRead(RIGHT);

		switch(incomingByte) {

			case 'W':
				if (curr_backward == 1)
					digitalWrite(FORWARD, 0);
				break;
			case 'w':
					digitalWrite(FORWARD, 1);
				break;

			case 'S':
				if (curr_forward == 1)
					digitalWrite(BACKWARD, 0);
				break;
			case 's':
					digitalWrite(BACKWARD, 1);
				break;

			case 'D':
				if (curr_left == 1)
					digitalWrite(RIGHT, 0);
				break;
			case 'd':
					digitalWrite(RIGHT, 1);
				break;

			case 'A':
				if (curr_right == 1)
					digitalWrite(LEFT, 0);
				break;
			case 'a':
					digitalWrite(LEFT, 1);
				break;
		}

	}
}
