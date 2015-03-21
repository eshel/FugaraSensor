#include "Bmp180Pressure.h"

Bmp180Pressure::Bmp180Pressure(char oversample) : 
		mPressure(), 
		mOverSample(oversample) {
	clear();
}

void Bmp180Pressure::clear() {
	mSamplePressureValue = 0.0;
	mSampleTemperatureValue = 0.0;
	mSampleTimeMillis = 0;
	mSampleIndex = 0;
}

bool Bmp180Pressure::setup() {
	clear();
	if (!mPressure.begin()) {
		Serial.println("Bmp180Pressure init ERROR!");
		return false;
	}
	return true;
}

bool Bmp180Pressure::sampleTemperature() {
	double temperature;
	char status = mPressure.startTemperature();
	if (!status) {
		return false;
	}
	delay(status);

	status = mPressure.getTemperature(mSampleTemperatureValue);
	if (!status) {
		return false;
	}

	return true;
}

bool Bmp180Pressure::samplePressure() {
	char delayMs = mPressure.startPressure(mOverSample);
	if (delayMs == 0) {
		return false;
	}

	delay(delayMs);

	char status = mPressure.getPressure(mSamplePressureValue, mSampleTemperatureValue);
	if (status == 0) {
		return false;
	}

	return true;
}


bool Bmp180Pressure::sample() {
	bool success = true;
	mSampleTimeMillis = millis();

	if (!sampleTemperature()) {
		Serial.println("ERROR: sampleTemperature()");
		return false;
	}
	if (!samplePressure()) {
		Serial.println("ERROR: samplePressure()");
		return false;
	}

	mSampleIndex++;
	Serial.print("Sample[");
	Serial.print(mSampleIndex);
	Serial.print("]: ");
	Serial.print(mSamplePressureValue, 2);
	Serial.print(" mbar");
	Serial.println(".");
	return true;
}
