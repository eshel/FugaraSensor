
#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>

// Unfortunately, we need to include library files from the .INO file.
#include <SFE_BMP180.h>
#include <SoftI2CMaster.h>

#include "SoftI2cBmp180.h"

// Our project files
#include "Bmp180Pressure.h"

#include "Definitions.h"


// Sensor instances and such
#define SENSORS_NUM		4

Bmp180Pressure barA(SENSOR_A_NAME, SENSOR_A_PIN_SDA, SENSOR_A_PIN_SCL, 0);
Bmp180Pressure barB(SENSOR_B_NAME, SENSOR_B_PIN_SDA, SENSOR_B_PIN_SCL, 0);
Bmp180Pressure barC(SENSOR_C_NAME, SENSOR_C_PIN_SDA, SENSOR_C_PIN_SCL, 0);
Bmp180Pressure barD(SENSOR_D_NAME, SENSOR_D_PIN_SDA, SENSOR_D_PIN_SCL, 0);


IPressure* pressureSensors[SENSORS_NUM] = {
	&barA, 
	&barB, 
	&barC, 
	&barD
};

// State
bool initialized = false;

#define SLAVE_ADDR_BAD	255

uint8_t readSlaveAddr() {
	uint8_t r0 = EEPROM.read(0);
	uint8_t r1 = EEPROM.read(1);
	if (r0 == SLAVE_ADDR_BAD) {
		return SLAVE_ADDR_BAD;
	} else if (r0 != ~r1) {
		return SLAVE_ADDR_BAD;
	} else {
		return r0;
	}
}

void writeSlaveAddr(uint8_t addr) {
	EEPROM.write(0, addr);
	EEPROM.write(1, ~addr);
}

uint8_t slaveAddr = SLAVE_ADDR_BAD;

void setup() {
	if (SERIAL_IS_ENABLED) {
		Serial.begin(SERIAL_BAUD);
		Serial.println("");
	}

	slaveAddr = readSlaveAddr();

	if (slaveAddr == SLAVE_ADDR_BAD) {
		if (SERIAL_IS_ENABLED) {
			Serial.println("WARNING: Slave Address not set!");
		}
	}

	if (SERIAL_IS_ENABLED) {
		Serial.print("FugaraSensor[");
		if (slaveAddr == SLAVE_ADDR_BAD) {
			Serial.print("?");
		} else {
			Serial.print(slaveAddr);
		}
		Serial.print("] Init... ");
	}

	for (int i=0; i<SENSORS_NUM; i++) {
		if (!pressureSensors[i]->setup()) {
			if (SERIAL_IS_ENABLED) {
				Serial.print("SENSOR ERROR (#");
				Serial.print(i);
				Serial.println(")");
			}
			return;
		}
	}

	initialized = true;
	if (SERIAL_IS_ENABLED) {
		Serial.println("READY");
	}
}


void loop() {
	if (!initialized) {
		if (SERIAL_IS_ENABLED) {
			Serial.println("NOT INITIALIZED!");
		}
		delay(1000);
	}

	pressureSampleLoop();
}



void pressureSampleLoop() {
	for (int i=0; i<SENSORS_NUM; i++) {
		pressureSensors[i]->sample();
	}
	Serial.println("");
	delay(SAMPLE_DELAY_MS);
}
