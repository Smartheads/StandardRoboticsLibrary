// 
// 
// 

#include "../Motor.h"

SRL::Motor::Motor(unsigned int forwardPin, unsigned int backwardPin, unsigned int pwmPin)
{
	this->forwardPin = forwardPin;
	this->backwardPin = backwardPin;
	this->pwmPin = pwmPin;

	pinMode(this->forwardPin, OUTPUT);
	pinMode(this->backwardPin, OUTPUT);
	pinMode(this->pwmPin, OUTPUT);

	this->speed = 0.0f;
}

void SRL::Motor::start(void)
{
	analogWrite(pwmPin, (int)(SRL::PWM_MAX_VALUE / 100U) * speed);
}

void SRL::Motor::stop(void)
{
	digitalWrite(pwmPin, LOW);
}

void SRL::Motor::setDirection(const unsigned int state)
{
	switch (state)
	{
		case FORWARD:
			digitalWrite(forwardPin, HIGH);
			digitalWrite(backwardPin, LOW);
			break;

		case BACKWARD:
			digitalWrite(backwardPin, HIGH);
			digitalWrite(forwardPin, LOW);
			break;
		
		default:
			break;
	}
}

void SRL::Motor::forwards(void)
{
	setDirection(FORWARD);
}

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

void SRL::Motor::setSpeed(float speed)
{
	if (speed >= 0 && speed <= 100)
	{
		this->speed = speed;
	}
}
