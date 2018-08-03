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
#ifndef _TANK_H
#define _TANK_H

#include <SRL.h>
#include <Motor.h>

namespace SRL
{
  /**
  * Class Tank. Defines a tank according to SCSRLG/TG1204.
  * Includes functions for the control of 2 motors next to eachother.
  */
  class Tank
  {
    public:
      Tank(SRL::Motor* leftMotor, SRL::Motor* rightMotor);
      ~Tank(void);

      /* Movement commands */
      void start(void);
      void stop(void);
      void faceRight(void);
      void faceLeft(void);

      void setDirection(unsigned int state);
      void forwards(void);
      void backwards(void);
      void setUnifiedSpeed(float speed);

      /* Enums */
      enum States
      {
        FORWARD = 1,
        BACKWARD = 2,
        LEFT = 3,
        RIGHT = 4
      };

      /* Getters & setters */
      unsigned int getDirection(void);
      SRL::Motor* getLeftMotor(void);
      SRL::Motor* getRightMotor(void);

    protected:
      SRL::Motor* leftMotor;
      SRL::Motor* rightMotor;

    private:
      unsigned int direction;

  };
}

#endif
