
#include <Arduino.h>
#include <Wire.h>

// Unfortunately, we need to include library files from the .INO file.
#include <SFE_BMP180.h>

// Our project files
#include "Bmp180Pressure.h"


// Sensor instances and such
Bmp180Pressure pressure;


void setup() {
	Serial.begin(38400);
	Serial.println("Serial Initialized");

	if (!pressure.setup()) {
		Serial.println("BMP180 Initialization Error");
	}

	Serial.println("Ready");
}


void loop() {
	pressure.sample();

	// Wait for 1 second
	delay(1000);
}

