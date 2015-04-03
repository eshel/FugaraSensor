#include <Arduino.h>
#include <EEPROM.h>

// CHANGE THIS VALUE TO SET THE ADDRESS, FOR EACH DEVICE
#define SLAVE_ADDR		1

#define SERIAL_BAUD		38400
#define LOOP_DELAY		2000


void setup() {
	unsigned char addr = SLAVE_ADDR;

	Serial.begin(SERIAL_BAUD);
	Serial.println("");
	Serial.print("Writing Slave Address ");
	Serial.print(SLAVE_ADDR);
	Serial.println(" to EEPROM (lower 2 bytes)...");

	EEPROM.write(0, addr);
	EEPROM.write(1, ~addr);	

	Serial.println("DONE!");
}

void loop() {
	Serial.print("Read Slave Address: ");

	unsigned char r0 = EEPROM.read(0);
	unsigned char r1 = EEPROM.read(1);
	unsigned char notR1 = (~r1);

	Serial.print(r0);
	Serial.print("/");
	Serial.print(r1);
	Serial.print(": ");

	if (r0 != notR1) {
		Serial.println("BAD!");
	} else {
		Serial.println("OK.");
	}

	delay(LOOP_DELAY);
}