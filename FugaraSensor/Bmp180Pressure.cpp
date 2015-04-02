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
	mIsValid = false;
	for (unsigned int i=0; i<BAROMETER_WINDOW_SIZE; i++) {
		mSample[i].clear();
	}
	resetCalibration();
	mSampleIndex = -1;
}

void Bmp180Pressure::resetCalibration() {
	mReference.clear();
	mIsValid = false;
}

bool Bmp180Pressure::setup() {
	clear();
	char ret = false;
	mLastPressMillis = 0;

	if (mUseHardwareI2C) {
		ret = sHardPressure.begin(); 
	} else {
		ret = mSoftPressure.begin();
	}

	if (!ret) {
		if (SERIAL_IS_ENABLED && BAROMETER_DEBUG_PRINTS) {
			Serial.println("Bmp180Pressure init ERROR!");
		}
		return false;
	}
	return true;
}

void Bmp180Pressure::performCalibration() {
	Sample& latest = getSample();

	if (!mReference.isValid() && latest.isValid()) {
		mReference.sum += latest.pressure;
		mReference.index++;

		if (mReference.index >= BAROMETER_CALIBRATION_COUNT) {
			mReference.pressure = mReference.sum / (float)mReference.index;
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

	if ((getSampleCount() > 1) && (abs(pressure - getSample(1).pressure) >= BAROMETER_MAX_DIFF_ALLOWED)) {
		return false;
	}

	return true;
}


bool Bmp180Pressure::sample() {
	bool success = false;

	mSampleIndex++;
	Sample& target = getSample(0);
	target.clear();

	// Using 32 bits will be okay as long as we're not up for more than 46 days straight
	uint32_t sampleTime = (uint32_t)millis();

	success = sampleTemperature(target) && samplePressure(target);
	if (!success) {
		resetCalibration();
		if (SERIAL_IS_ENABLED && BAROMETER_DEBUG_PRINTS) {
			Serial.print(mName);
			Serial.print(": invalid! ");		
		}
		mIsValid = false;
		return false;
	} else {
		target.index = mSampleIndex;
		target.millis = sampleTime;
	}

	performCalibration();

	bool noisy = isNoisyEnough();
	bool enoughSamples = (getSampleCount() >= BAROMETER_MIN_NOISE_COUNT);
	if (enoughSamples && !noisy) {
		resetCalibration();
	}

	if (enoughSamples && noisy && mReference.isValid()) {
		mIsValid = true;
	}

	if (SERIAL_IS_ENABLED && BAROMETER_DEBUG_PRINTS) {
		printSample(target);
	}

	if (mIsValid) {
		bool touched = detectTouch();
		if (touched) {
			mLastPressMillis = millis();
		}
	}

	return true;
}


bool Bmp180Pressure::isNoisyEnough() {
	int count = getSampleCount();
	int aboveEpsilonCount = 0;
	float latest, p, diff;
	if (count > 2) {
		latest = getSample(0).pressure;
		for (int i=1; i<count; i++) {
			p = getSample(i).pressure;
			diff = abs(p - latest);
			if (diff >= EPSILON) {
				aboveEpsilonCount++;
			}
		}
	}

	return (aboveEpsilonCount >= BAROMETER_MIN_NOISE_COUNT);
}

bool Bmp180Pressure::detectTouch() {
	if (millis() - mLastPressMillis < BAROMETER_MIN_TOUCH_INTERVAL_MS) {
		return false;
	}
	return false;
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
	if (!isNoisyEnough()) {
		Serial.print('~');
	}
	if (!mIsValid) {
		Serial.print('!');
	}	
	Serial.print(". ");
}
