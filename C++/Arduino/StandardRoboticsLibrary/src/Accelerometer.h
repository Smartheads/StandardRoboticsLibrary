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
#ifndef _ACCELEROMETER_H
#define _ACCELEROMETER_H

#include <SRL.h>
#include <Component.h>

namespace SRL
{
  class Accelerometer: public SRL::Component
  {
    public:
      virtual void initialize(void) = 0;

      virtual int16_t getRawAccelX(void) = 0;
      virtual int16_t getRawAccelY(void) = 0;
      virtual int16_t getRawAccelZ(void) = 0;

      double getAccelX(void);
      double getAccelY(void);
      double getAccelZ(void);

      int16_t getAccelXOffset(void);
      void setAccelXOffset(int16_t offset);
      int16_t getAccelYOffset(void);
      void setAccelYOffset(int16_t offset);
      int16_t getAccelZOffset(void);
      void setAccelZOffset(int16_t offset);

      void setAccelOffsets(int16_t x, int16_t y, int16_t z);

      virtual bool setAccelSensitivity(uint8_t setting) = 0;

    protected:
      double accelSensitivity;

      int16_t accelXOffset;
      int16_t accelYOffset;
      int16_t accelZOffset;
  };
}

#endif
