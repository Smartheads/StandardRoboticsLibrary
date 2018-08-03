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
#include <Tank.h>

/**
* Class Tank's contructor.
*
* @param leftMotor The tank's left motor.
* @param rightMotor The tank's right motor.
*/
SRL::Tank::Tank(SRL::Motor* leftMotor, SRL::Motor* rightMotor)
{
  this->leftMotor = leftMotor;
  this->rightMotor = rightMotor;
}

SRL::Tank::~Tank(void)
{
  delete leftMotor;
  delete rightMotor;
}

/**
* Starts the tank's both motors.
*/
void SRL::Tank::start(void)
{
  leftMotor->start();
  rightMotor->start();
}

/**
* Stops the tank's both motors.
*/
void SRL::Tank::stop(void)
{
  leftMotor->stop();
  rightMotor->stop();
}

/**
* Sets the direction of the tank's motors so that the rover
* will turn left if the motors are ran.
*/
void SRL::Tank::faceLeft(void)
{
  leftMotor->backwards();
  rightMotor->forwards();
  direction = LEFT;
}

/**
* Sets the direction of the tank's motors so that the rover
* will turn left if the motors are ran.
*/
void SRL::Tank::faceRight(void)
{
  leftMotor->forwards();
  rightMotor->backwards();
  direction = RIGHT;
}

/**
* Sets the tank's both motors direction to forwards.
*/
void SRL::Tank::forwards(void)
{
  leftMotor->forwards();
  rightMotor->forwards();
  direction = FORWARD;
}

/**
* Sets the tank's both motors direction to backwards.
*/
void SRL::Tank::backwards(void)
{
  leftMotor->backwards();
  rightMotor->backwards();
  direction = BACKWARD;
}

/**
* Sets the speed of both motors to the argument.
*
* @param speed The speed to set the motors to.
*/
void SRL::Tank::setUnifiedSpeed(float speed)
{
  leftMotor->setSpeed(speed);
  rightMotor->setSpeed(speed);
}

/**
* Sets the tank's motors direction to the given argument.
*
* @param state Position to set the tank's motors to.
*/
void SRL::Tank::setDirection(unsigned int state)
{
  switch (state)
  {
    case FORWARD:
      forwards();
      break;

    case BACKWARD:
      backwards();
      break;

    case LEFT:
      faceLeft();
      break;

    case RIGHT:
      faceRight();
      break;

    default:
      return;
  }
}

unsigned int SRL::Tank::getDirection(void)
{
  return direction;
}

SRL::Motor* SRL::Tank::getLeftMotor(void)
{
  return leftMotor;
}

SRL::Motor* SRL::Tank::getRightMotor(void)
{
  return rightMotor;
}
