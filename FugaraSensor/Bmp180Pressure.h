#ifndef _BMP180PRESSURE_H_
#define _BMP180PRESSURE_H_

#include <Arduino.h>
#include <Wire.h>
#include "IPressure.h"
#include <SFE_BMP180.h>


class Bmp180Pressure : public IPressure {
public:
	Bmp180Pressure(char oversample = 0);
	virtual void clear();
	virtual bool setup();
	virtual bool sampleTemperature();
	virtual bool samplePressure();
	virtual bool sample();

	virtual double getPressure() {
		return mSamplePressureValue;
	}

	virtual bool wasPressed() {
		return true;
	}

private:
	SFE_BMP180 mPressure;
	int mSampleIndex;
	double mSamplePressureValue;
	double mSampleTemperatureValue;
	long mSampleTimeMillis;
	char mOverSample;

};

#endif // #ifndef _BMP180PRESSURE_H_
