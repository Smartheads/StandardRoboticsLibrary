// SRL.h

#ifndef SRL_LIB_H

#define SRL_LIB_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace SRL
{
	const unsigned int PWM_MAX_VALUE = 255;
}

#endif // !SRL_LIB_H