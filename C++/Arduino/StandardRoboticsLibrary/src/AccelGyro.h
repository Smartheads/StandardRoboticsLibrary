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
#ifndef _ACCELGYRO_H
#define _ACCELGYRO_H

#include "SRL.h"
#include "Accelerometer.h"
#include "Gyroscope.h"
#include "Angle.h"

namespace SRL
{
  class AccelGyro : public virtual SRL::Accelerometer, public virtual SRL::Gyroscope
  {
    public:
      AccelGyro(float aC, float gC);

      virtual int16_t getRawAccelX(void) = 0;
      virtual int16_t getRawAccelY(void) = 0;
      virtual int16_t getRawAccelZ(void) = 0;

      virtual int16_t getRawGyroX(void) = 0;
      virtual int16_t getRawGyroY(void) = 0;
      virtual int16_t getRawGyroZ(void) = 0;

      void update(unsigned int sampleSize = 1);
      void update(unsigned long deltaT, unsigned int sampleSize = 1);

      /* Getters & setters */
      float getAccelCoeff(void);
			void setAccelCoeff(float aC);
			float getGyroCoeff(void);
			void setGyroCoeff(float gC);

      float getAngleX(void);
      float getAngleY(void);
      float getAngleZ(void);
      void setAngleX(float angle);
      void setAngleY(float angle);
      void setAngleZ(float angle);

    protected:
      float aC;
      float gC;
      Angle angleX, angleY, angleZ;
  };
}

#endif
