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

SRL::Tank::Tank(SRL::Motor* leftMotor, SRL::Motor* rightMotor)
{
  this->leftMotor = leftMotor;
  this->rightMotor = rightMotor;
}

void SRL::Tank::start(void)
{
  leftMotor->start();
  rightMotor->start();
}

void SRL::Tank::stop(void)
{
  leftMotor->stop();
  rightMotor->stop();
}

void SRL::Tank::faceLeft(void)
{
  leftMotor->backwards();
  rightMotor->forwards();
  direction = LEFT;
}

void SRL::Tank::faceRight(void)
{
  leftMotor->forwards();
  rightMotor->backwards();
  direction = RIGHT;
}

void SRL::Tank::forwards(void)
{
  leftMotor->forwards();
  rightMotor->forwards();
  direction = FORWARD;
}

void SRL::Tank::backwards(void)
{
  leftMotor->backwards();
  rightMotor->backwards();
  direction = BACKWARD;
}

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
