// Motor.h

#ifndef _MOTOR_h
#define _MOTOR_h

#include "SRL.h"

namespace SRL
{
	class Motor
	{
	public:
		/* Constructors */
		Motor(unsigned int forwardPin, unsigned int backwardPin, unsigned int pwmPin);

		/* Movement commands */
		void start(void);
		void stop(void);

		void setDirection(unsigned int state);
		void forwards(void);
		void backwards(void);

		/* Enums */
		enum States
		{
			FORWARD = 1,
			BACKWARD = 2
		};

		/* Getters & setters */
		unsigned int getForwardPin(void);
		unsigned int getBackwardPin(void);
		unsigned int getPwmPin(void);
		float getSpeed(void);
		
		void setSpeed(float speed);

	private:
		unsigned int forwardPin;
		unsigned int backwardPin;
		unsigned int pwmPin;

		float speed;
	};
}
#endif