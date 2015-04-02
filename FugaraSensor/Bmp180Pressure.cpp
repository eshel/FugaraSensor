#include "Bmp180Pressure.h"
#include "Definitions.h"

#define EPSILON							0.00001

SFE_BMP180 Bmp180Pressure::sHardPressure;

Bmp180Pressure::Bmp180Pressure(char oversample) : 
		mSoftPressure(0, 0),
		mOverSample(oversample) {
	mUseHardwareI2C = true;
	mName = "bar";
	mSampleIndex = -1;
	clear();
}

Bmp180Pressure::Bmp180Pressure(const char* strName, byte sdaPin, byte sclPin, char oversample) : 
		mSoftPressure(sdaPin, sclPin),
		mOverSample(oversample) {
	mUseHardwareI2C = false;
	mSampleIndex = -1;
	mName = strName;
	clear();
}


void Bmp180Pressure::clear() {
	mSample.clear();
	mReference.clear();
	mSampleIndex = -1;
}

bool Bmp180Pressure::setup() {
	clear();
	char ret = false;

	if (mUseHardwareI2C) {
		ret = sHardPressure.begin(); 
	} else {
		ret = mSoftPressure.begin();
	}

	if (!ret) {
		if (SERIAL_IS_ENABLED) {
			Serial.println("Bmp180Pressure init ERROR!");
		}
		return false;
	}
	return true;
}

void Bmp180Pressure::performCalibration() {
	Sample& latest = getSample();
	int sampleCount = getSampleCount();

	if (!mReference.isValid() && latest.isValid()) {
		mReference.sum += latest.pressure;

		if (sampleCount >= BAROMETER_CALIBRATION_COUNT) {
			mReference.pressure = mReference.sum / (float)sampleCount;
			mReference.index = mSampleIndex;
			mReference.millis = millis();
		}
	}
}

bool Bmp180Pressure::sampleTemperature(Bmp180Pressure::Sample& o) {
	double temperature;
	char status;
	status = mUseHardwareI2C ? 
			sHardPressure.startTemperature() : 
			mSoftPressure.startTemperature();
	if (!status) {
		return false;
	}
	delay(status);

	status = mUseHardwareI2C ? 
			sHardPressure.getTemperature(temperature) : 
			mSoftPressure.getTemperature(temperature);
	o.temp = (float)temperature;
	if (!status) {
		return false;
	}

	return true;
}

bool Bmp180Pressure::samplePressure(Bmp180Pressure::Sample& o) {
	char delayMs = mUseHardwareI2C ? 
			sHardPressure.startPressure(mOverSample) : 
			mSoftPressure.startPressure(mOverSample);
	if (delayMs == 0) {
		return false;
	}

	delay(delayMs);

	double pressure = o.pressure;
	double temp = o.temp;

	char status =  mUseHardwareI2C ? 
			sHardPressure.getPressure(pressure, temp) :
			mSoftPressure.getPressure(pressure, temp);
	o.pressure = (float)pressure;
	o.temp = (float)temp;
	if (status == 0) {
		return false;
	}

	if (o.pressure < MIN_VALID_PRESSURE_VALUE) {
		return false;
	}

	return true;
}


bool Bmp180Pressure::sample() {
	bool success = false;

	mSampleIndex++;
	Sample& target = getSample(0);
	target.clear();

	long sampleTime = millis();

	success = sampleTemperature(target) && samplePressure(target);
	if (!success) {
		clear();
		if (SERIAL_IS_ENABLED) {
			Serial.print("invalid! ");		
		}
		return false;
	} else {
		target.index = mSampleIndex;
		target.millis = sampleTime;
	}

	performCalibration();

	if (SERIAL_IS_ENABLED) {
		printSample(target);
	}

	return true;
}


void Bmp180Pressure::printSample(const Bmp180Pressure::Sample& s) {
	float val = s.pressure - mReference.pressure;
	Serial.print(mName);
	Serial.print("[");
	Serial.print(s.index);
	Serial.print("]: ");

	if (val >= 0.0) {
		Serial.print('+');
	}
	Serial.print(val, 2);
	Serial.print(" mbar");
	if (!mReference.isValid()) {
		Serial.print('*');
	}
	Serial.print(". ");
}
