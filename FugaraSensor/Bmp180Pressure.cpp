#include "Bmp180Pressure.h"
#include "Definitions.h"

#define EPSILON							0.00001

Bmp180Pressure::Bmp180Pressure(char oversample) : 
		mHardPressure(), 
		mSoftPressure(0, 0),
		mOverSample(oversample) {
	mUseHardwareI2C = true;
	mName = "bar";
	mReferencePressure = 0.0;
	clear();
}

Bmp180Pressure::Bmp180Pressure(const char* strName, byte sdaPin, byte sclPin, char oversample) : 
		mHardPressure(), 
		mSoftPressure(sdaPin, sclPin),
		mOverSample(oversample) {
	mUseHardwareI2C = false;
	mReferencePressure = 0.0;
	mName = strName;
	clear();
}


void Bmp180Pressure::clear() {
	mSamplePressureValue = 0.0;
	mSampleTemperatureValue = 0.0;
	mSampleTimeMillis = 0;
	mSampleIsValid = false;

	mReferencePressure = 0.0;
	mReferenceCount = 0;
	mReferenceIsSet = false;
	mReferenceSum = 0.0;
}

bool Bmp180Pressure::setup() {
	clear();
	char ret = false;

	if (mUseHardwareI2C) {
		ret = mHardPressure.begin(); 
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

void Bmp180Pressure::detectReference() {
	if (!mReferenceIsSet && mSampleIsValid) {
		mReferenceSum += mSamplePressureValue;
		mReferenceCount++;

		if (mReferenceCount >= MAX_REFERENCE_SAMPLE_COUNT) {
			mReferencePressure = mReferenceSum / (double)mReferenceCount;
			mReferenceIsSet = true;
		}
	}
}

bool Bmp180Pressure::sampleTemperature() {
	double temperature;
	char status;
	status = mUseHardwareI2C ? 
			mHardPressure.startTemperature() : 
			mSoftPressure.startTemperature();
	if (!status) {
		return false;
	}
	delay(status);

	status = mUseHardwareI2C ? 
			mHardPressure.getTemperature(mSampleTemperatureValue) : 
			mSoftPressure.getTemperature(mSampleTemperatureValue);
	if (!status) {
		return false;
	}

	return true;
}

bool Bmp180Pressure::samplePressure() {
	char delayMs = mUseHardwareI2C ? 
			mHardPressure.startPressure(mOverSample) : 
			mSoftPressure.startPressure(mOverSample);
	if (delayMs == 0) {
		return false;
	}

	delay(delayMs);

	char status =  mUseHardwareI2C ? 
			mHardPressure.getPressure(mSamplePressureValue, mSampleTemperatureValue) :
			mSoftPressure.getPressure(mSamplePressureValue, mSampleTemperatureValue);
	if (status == 0) {
		return false;
	}

	if (mSamplePressureValue <= MIN_VALID_PRESSURE_VALUE) {
		return false;
	}

	return true;
}


bool Bmp180Pressure::sample() {
	bool success = true;
	mSampleTimeMillis = millis();

	mSampleIsValid = sampleTemperature() && samplePressure();
	if (!mSampleIsValid) {
		clear();
		if (SERIAL_IS_ENABLED) {
			Serial.print("invalid! ");		
		}
		return false;
	}

	mSampleIndex++;

	detectReference();

	if (SERIAL_IS_ENABLED) {
		double val = mSamplePressureValue - mReferencePressure;
		Serial.print(mName);
		Serial.print("[");
		Serial.print(mSampleIndex);
		Serial.print("]: ");

		if (val >= 0.0) {
			Serial.print('+');
		}
		Serial.print(val, 2);
		Serial.print(" mbar");
		if (!mReferenceIsSet) {
			Serial.print('*');
		}
		Serial.print(". ");		
	}

	return true;
}
