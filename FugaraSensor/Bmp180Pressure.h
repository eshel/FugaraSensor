#ifndef _BMP180PRESSURE_H_
#define _BMP180PRESSURE_H_

#include <Arduino.h>
#include <Wire.h>

#include "IPressure.h"
#include <SFE_BMP180.h>

#include "SoftI2cBmp180.h"


class Bmp180Pressure : public IPressure {
public:
	Bmp180Pressure(char oversample = 0);
	Bmp180Pressure(const char* strName, byte sdaPin, byte sclPin, char oversample = 0);

	virtual void clear();
	virtual bool setup();
	virtual bool sample();

	virtual bool wasPressed() {
		return true;
	}

	bool isCalibrated() {
		return mReference.isValid();
	}

private:
	void performCalibration();

	struct Sample {
		long index;
		long millis;
		float pressure;
		float temp;
		float sum;

		inline void clear() {
			index = 0;
			millis = 0;
			pressure = 0.0;
			temp = 0.0;
			sum = 0.0;
		}

		inline bool isValid() {
			return millis > 0;
		}
	};

	inline Sample& getSample() {
		return getSample(0);
	}

	inline Sample& getSample(int8_t offset) {
		return mSample;
	}

	inline int getSampleCount() {
		return mSampleIndex + 1;
	}

	bool sampleTemperature(Sample& o);
	bool samplePressure(Sample& o);
	void printSample(const Sample& s);

private:
	static SFE_BMP180 sHardPressure;
	SoftI2cBmp180 mSoftPressure;

	Sample mSample;
	Sample mReference;

	char mOverSample;
	bool mUseHardwareI2C;
	long mSampleIndex;

	const char* mName;
};

#endif // #ifndef _BMP180PRESSURE_H_
