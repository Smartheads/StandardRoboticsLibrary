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
#include <Rover.h>

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
	}
}

void SRL::Rover::updateDirection(void)
{
	
}

void SRL::Rover::updatePosition(void)
{
	mpu->interruptUpdate(GYROSCOPE_INTERVAL);
	direction.setAngle(mpu->getAngleX());
	
	if(movingStraight)
	{
		
	}
}

void SRL::Rover::goTo(double x, double y)
{
	SRL::Vector v(x - this->x, y - this->y);
	turnTo(v.getAngle());
	forward(v.getLength());
}

void SRL::Rover::turnTo(SRL::Angle dir)
{
	if (dir.getSize() > direction.getSize())
	{
		// Turn right
		turnRight(dir.getSize() - direction.getSize());
	}
	else if (dir.getSize() != direction.getSize())
	{
		// Turn left
		turnLeft(direction.getSize() - dir.getSize());
	}
}

void SRL::Rover::forward(double distance)
{
	SRL::Vector g(direction, distance);
	xGoal = g.getX() + x;
	yGoal = g.getY() + y;
	
	leftMotor->forwards();
	rightMotor->forwards();
	
	leftMotor->start();
	rightMotor->start();
	
	movingStraight = true;
}

void SRL::Rover::backward(double distance)
{
	SRL::Vector g(direction, distance * -1);
	xGoal = g.getX() + x;
	yGoal = g.getY() + y;
	
	leftMotor->backwards();
	rightMotor->backwards();
	
	leftMotor->start();
	rightMotor->start();
	
	movingStraight = true;
}

void SRL::Rover::turnRight(double amount)
{
	turnGoal =  SRL::Angle(direction.getSize() + amount);
	
	leftMotor->forwards();
	rightMotor->backwards();
	
	leftMotor->start();
	rightMotor->start();
	
	turning = true;
}

void SRL::Rover::turnLeft(double amount)
{
	turnGoal =  SRL::Angle(direction.getSize() + amount * -1);
	
	leftMotor->backwards();
	rightMotor->forwards();
	
	leftMotor->start();
	rightMotor->start();
	
	turning = true;
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