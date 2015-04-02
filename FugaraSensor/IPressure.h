#ifndef _IPRESSURE_H_
#define _IPRESSURE_H_

#include <Arduino.h>

class IPressure {
public:
	virtual bool setup() = 0;
	virtual bool sample() = 0;
	virtual bool wasPressed() = 0;
};

#endif // #ifndef _IPRESSURE_H_