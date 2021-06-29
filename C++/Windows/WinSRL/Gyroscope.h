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
#ifndef _GYROSCOPE_H
#define _GYROSCOPE_H

#include "SRL.h"
#include "Component.h"
#include "Statistics.h"

#define GYRO_CALIBRATION_I 25

namespace SRL
{
  class Gyroscope : virtual public SRL::Component
  {
    public:
      virtual int16_t getRawGyroX(void) = 0;
			virtual int16_t getRawGyroY(void) = 0;
			virtual int16_t getRawGyroZ(void) = 0;

      int16_t getRawGyroXMedian(unsigned int iterations = 5);
      int16_t getRawGyroYMedian(unsigned int iterations = 5);
      int16_t getRawGyroZMedian(unsigned int iterations = 5);

      double getGyroX(void);
			double getGyroY(void);
			double getGyroZ(void);

      double getGyroXMedian(unsigned int iterations = 5);
      double getGyroYMedian(unsigned int iterations = 5);
      double getGyroZMedian(unsigned int iterations = 5);

      /* Getters & setters */
      int16_t getGyroXOffset(void);
			void setGyroXOffset(int16_t offset);
			int16_t getGyroYOffset(void);
			void setGyroYOffset(int16_t offset);
			int16_t getGyroZOffset(void);
			void setGyroZOffset(int16_t offset);

      void setGyroOffsets(int16_t x, int16_t y, int16_t z);
      void calcGyroOffsets(bool console = false, unsigned int interations = 50);

      virtual uint8_t setGyroSensitivity(uint8_t setting) = 0;

    protected:
      int16_t gyroXOffset;
			int16_t gyroYOffset;
			int16_t gyroZOffset;

      double gyroSensitivity;
  };
}

#endif
