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
#include "Rover.h"

SRL::Rover::Rover(SRL::Motor* leftMotor, SRL::Motor* rightMotor, double x, double y, float direction): Tank(leftMotor, rightMotor)
{
	this->x = x;
	this->y = y;
	this->direction = SRL::Angle(direction);
}

SRL::Rover::~Rover(void)
{
	delete leftMotor;
	delete rightMotor;
	delete leftEncoder;
	delete rightEncoder;
	delete accelGyro;
}

/**
*	Initialize the rover.
*
*/
void SRL::Rover::initialize(void)
{
	if (leftEncoder != NULL)
		leftEncoder->initialize();

	if (rightEncoder != NULL)
		rightEncoder->initialize();

	if (accelGyro != NULL)
		accelGyro->initialize();
}

void SRL::Rover::goTo(double x, double y)
{
	SRL::Vector v = Vector(x - this->x, y - this->y);
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
	// Calculate target position
	SRL::Vector g = Vector(direction, distance);
	xGoal = g.getX() + x;
	yGoal = g.getY() + y;

	// Command movement
	Tank::forwards();
	Tank::start();
	movingStraight = true;
}

void SRL::Rover::backward(double distance)
{
	// Calculate target position
	SRL::Vector g = Vector(direction, distance * -1);
	xGoal = g.getX() + x;
	yGoal = g.getY() + y;

	// Command movement
	Tank::backwards();
	Tank::start();
	movingStraight = true;
}

void SRL::Rover::turnRight(double amount)
{
	// Calculate target direction
	turnGoal =  SRL::Angle(direction.getSize() + amount);

	// Command movement
	Tank::faceRight();
	Tank::start();
	turning = true;
}

void SRL::Rover::turnLeft(double amount)
{
	// Calculate target direction
	turnGoal =  SRL::Angle(direction.getSize() + amount * -1);

	// Command movement
	Tank::faceLeft();
	Tank::start();
	turning = true;
}

void SRL::Rover::stop(void)
{
	Tank::stop();
	movingStraight = false;
	turning = false;
}

/**
*	An interrupt routine to correct the rover's motors.
*/
void SRL::Rover::correctMotors(void)
{
	if (movingStraight)
	{
		long l = leftEncoder->read();
		long r = rightEncoder->read();

		if (Tank::getDirection() == Tank::FORWARD)
		{
			if (l > r)
			{
				leftMotor->setSpeed(getCorrectionSpeed(abs(l-r)));
				rightMotor->setSpeed(100);
			}
			else if (l != r)
			{
				rightMotor->setSpeed(getCorrectionSpeed(abs(r-l)));
				leftMotor->setSpeed(100);
			}
			else
			{
				setUnifiedSpeed(100);
			}
		}
		else
		{
			if (l < r)
			{
				leftMotor->setSpeed(getCorrectionSpeed(abs(l-r)));
				rightMotor->setSpeed(100);
			}
			else if (l != r)
			{
				rightMotor->setSpeed(getCorrectionSpeed(abs(r-l)));
				leftMotor->setSpeed(100);
			}
			else
			{
				leftMotor->setSpeed(100);
				rightMotor->setSpeed(100);
			}
		}
	}
}

/**
*	An interrupt routine to update the rover's virtual position.
*/
void SRL::Rover::updatePosition(void)
{
	/* Get traveled distance */
	double le = leftEncoder->readCm();
	double re = rightEncoder->readCm();
	double d = (le + re) / 2.0;

	leftEncoder->write(0);
	rightEncoder->write(0);

	/* Get direction */

	/* Combine data with current position */
	SRL::Vector v = Vector(getDirection(), d);
	this->x += v.getX();
	this->y += v.getY();

	/* Check current movement */
	if (movingStraight)
	{
		if (SRL::equals(x, xGoal, PRECISION) && SRL::equals(y, yGoal, PRECISION))
		{
			stop();
		}
	}

	if (turning)
	{

	}
}

float SRL::Rover::getCorrectionSpeed(float x)
{
	return (0 - 0.19 * x + 99.64);
}

float SRL::Rover::getDirection(void)
{
	return direction.getSize();
}

void SRL::Rover::setDirection(float direction)
{
	this->direction = SRL::Angle(direction);
}

double SRL::Rover::getX(void)
{
	return x;
}

void SRL::Rover::setX(double x)
{
	this->x = x;
}

double SRL::Rover::getY(void)
{
	return y;
}

void SRL::Rover::setY(double y)
{
	this->y = y;
}

void SRL::Rover::setAccelGyro(AccelGyro* accelGyro)
{
	this->accelGyro = accelGyro;
}

void SRL::Rover::setLeftEncoder(Encoder* leftEncoder)
{
	this->leftEncoder = leftEncoder;
}

void SRL::Rover::setRightEncoder(Encoder* rightEncoder)
{
	this->rightEncoder = rightEncoder;
}

SRL::AccelGyro* SRL::Rover::getAccelGyro(void)
{
	return accelGyro;
}

SRL::Encoder* SRL::Rover::getLeftEncoder(void)
{
	return leftEncoder;
}

SRL::Encoder* SRL::Rover::getRightEncoder(void)
{
	return rightEncoder;
}

SRL::Motor* SRL::Rover::getLeftMotor(void)
{
	return leftMotor;
}

SRL::Motor* SRL::Rover::getRightMotor(void)
{
	return rightMotor;
}
