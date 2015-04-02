#ifndef _BMP180PRESSURE_H_
#define _BMP180PRESSURE_H_

#include <Arduino.h>
#include <Wire.h>

#include "IPressure.h"
#include <SFE_BMP180.h>

#include "SoftI2cBmp180.h"

#define SAMPLE_WINDOW_SIZE	4


class Bmp180Pressure : public IPressure {
public:
	Bmp180Pressure(char oversample = 0);
	Bmp180Pressure(const char* strName, byte sdaPin, byte sclPin, char oversample = 0);

	virtual void clear();
	virtual bool setup();
	virtual bool sampleTemperature();
	virtual bool samplePressure();
	virtual bool sample();

	virtual bool wasPressed() {
		return true;
	}

	int getSampleIndex() {
		return mSampleIndex;
	}

	long getSampleTimeMillis() {
		return mSampleTimeMillis;
	}

	bool isSampleValid() {
		return mSampleIsValid;
	}

	bool isCalibrated() {
		return mReferenceIsSet;
	}

private:
	void detectReference();

	struct Sample {
		double pressure;
		double temp;
		long millis;
	};

private:
	SFE_BMP180 mHardPressure;
	SoftI2cBmp180 mSoftPressure;

	int mSampleIndex;
	double mSamplePressureValue;
	double mSampleTemperatureValue;
	long mSampleTimeMillis;
	char mOverSample;
	bool mUseHardwareI2C;
	bool mSampleIsValid;

	const char* mName;

	double mReferenceSum;
	double mReferencePressure;
	int mReferenceCount;
	bool mReferenceIsSet;
};

#endif // #ifndef _BMP180PRESSURE_H_
