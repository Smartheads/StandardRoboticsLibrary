/*
* MIT License
*
* Copyright (c) 2018 Robert Hutter
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef _MOTOR_h
#define _MOTOR_h

#include <SRL.h>

namespace SRL
{
	class Motor
	{
	public:
		/* Constructors */
		Motor(unsigned int forwardPin, unsigned int backwardPin, unsigned int pwmPin);
		~Motor(void);

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
		unsigned int getDirection(void);

	private:
		unsigned int forwardPin;
		unsigned int backwardPin;
		unsigned int pwmPin;

		float speed;
		unsigned int direction;
		bool moving;
	};
}
#endif
