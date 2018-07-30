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
#include <Motor.h>

/**
*	Class Motor's constructor.
*
*/
SRL::Motor::Motor(unsigned int forwardPin, unsigned int backwardPin, unsigned int pwmPin)
{
	this->forwardPin = forwardPin;
	this->backwardPin = backwardPin;
	this->pwmPin = pwmPin;

	pinMode(this->forwardPin, OUTPUT);
	pinMode(this->backwardPin, OUTPUT);
	pinMode(this->pwmPin, OUTPUT);

	forwards();
	stop();

	setSpeed(100.0f);
}

SRL::Motor::~Motor(void)
{
	stop();
}

/**
*	Turns the motor on.
*/
void SRL::Motor::start(void)
{
	analogWrite(pwmPin, (int ((SRL::PWM_MAX_VALUE / 100.0f) * speed)));
	moving = true;
}

/**
*	Stops the motor.
*/
void SRL::Motor::stop(void)
{
	digitalWrite(pwmPin, LOW);
	moving = false;
}

/**
*	Set the motors direction to the given argument.
*
*	@param state The direction to set the motor to.
*/
void SRL::Motor::setDirection(unsigned int state)
{
	switch (state)
	{
		case FORWARD:
			digitalWrite(forwardPin, HIGH);
			digitalWrite(backwardPin, LOW);
			direction = state;
			break;

		case BACKWARD:
			digitalWrite(forwardPin, LOW);
			digitalWrite(backwardPin, HIGH);
			direction = state;
			break;

		default:
			break;
	}
}

unsigned int SRL::Motor::getDirection(void)
{
	return direction;
}

/**
*	Sets the motor's direction to forwards.
*/
void SRL::Motor::forwards(void)
{
	setDirection(FORWARD);
}

/**
*	Sets the motor's direction to backwards.
*/
void SRL::Motor::backwards(void)
{
	setDirection(BACKWARD);
}

unsigned int SRL::Motor::getForwardPin(void)
{
	return forwardPin;
}

unsigned int SRL::Motor::getBackwardPin(void)
{
	return backwardPin;
}

unsigned int SRL::Motor::getPwmPin(void)
{
	return pwmPin;
}

float SRL::Motor::getSpeed(void)
{
	return speed;
}

/**
*	Sets the motors speed to the given argument.
*
*	@param float Speed of the motor (in procentage!).
*/
void SRL::Motor::setSpeed(float speed)
{
	if (speed >= 0 && speed <= 100)
	{
		this->speed = speed;

		if (moving)
		{
			start();
		}
	}
}
