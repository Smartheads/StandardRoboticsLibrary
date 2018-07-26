// 
// 
// 

#include "Rover.h"

SRL::Rover::Rover(SRL::Motor * leftMotor, SRL::Motor * rightMotor, double x, double y, float direction)
{
	this->leftMotor = leftMotor;
	this->rightMotor = rightMotor;
	this->x = x;
	this->y = y;
	this->direction = SRL::Angle(direction);
}

SRL::Rover::Rover(SRL::Motor * leftMotor, SRL::Motor * rightMotor, NewPing * leftSonar, NewPing * rightSonar, double x, double y, float direction)
{
	this->leftMotor = leftMotor;
	this->rightMotor = rightMotor;
	this->leftSonar = leftSonar;
	this->rightSonar = rightSonar;
	this->direction = SRL::Angle(direction);
}

SRL::Rover::Rover(SRL::Motor * leftMotor, SRL::Motor * rightMotor, NewPing * leftSonar, NewPing * rightSonar, MPU6050* mpu, double x, double y, float direction)
{
	this->leftMotor = leftMotor;
	this->rightMotor = rightMotor;
	this->leftSonar = leftSonar;
	this->rightSonar = rightSonar;
	this->mpu = mpu;
	this->direction = SRL::Angle(direction);
}

SRL::Rover::Rover(SRL::Motor * leftMotor, SRL::Motor * rightMotor, NewPing * leftSonar, NewPing * rightSonar, Encoder* leftEncoder, Encoder* rightEncoder, MPU6050* mpu, double x, double y, float direction)
{
	this->leftMotor = leftMotor;
	this->rightMotor = rightMotor;
	this->leftSonar = leftSonar;
	this->rightSonar = rightSonar;
	this->mpu = mpu;
	this->direction = SRL::Angle(direction);
	this->rightEncoder = rightEncoder;
	this->leftEncoder = leftEncoder;
}

void SRL::Rover::init(void)
{
	if (mpu != NULL)
	{
		mpu->begin();
		Timer0.attachInterrupt(this->updateDirection).start(GYROSCOPE_INTERVAL);
	}

	if (leftEncoder != NULL && rightEncoder != NULL)
	{
		Timer1.attachInterrupt(this->updatePosition).start(UPDATE_POSITION_INTERAL);
	}
}

void SRL::Rover::updateDirection(void)
{
	mpu->interruptUpdate(GYROSCOPE_INTERVAL);
	direction.setAngle(mpu->getAngleX());
}

void SRL::Rover::updatePosition(void)
{
	
}

float SRL::Rover::getDirection(void)
{
	return direction.getSize();
}

double SRL::Rover::getX(void)
{
	return x;
}

double SRL::Rover::getY(void)
{
	return y;
}

void SRL::Rover::calcGyroOffsets(bool console, unsigned int sampleSize)
{
	if (mpu != NULL)
	{
		mpu->calcGyroOffsets(console, sampleSize);
	}
}