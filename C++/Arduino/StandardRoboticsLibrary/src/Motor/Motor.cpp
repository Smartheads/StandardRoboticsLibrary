// 
// 
// 

#include <Motor.h>

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

void SRL::Motor::start(void)
{
	analogWrite(pwmPin, (int ((SRL::PWM_MAX_VALUE / 100.0f) * speed)));
	moving = true;
}

void SRL::Motor::stop(void)
{
	digitalWrite(pwmPin, LOW);
	moving = false;
}

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
			digitalWrite(backwardPin, HIGH);
			digitalWrite(forwardPin, LOW);
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
		
		if (moving)
		{
			start();
		}
	}
}
