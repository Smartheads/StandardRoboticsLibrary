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
#ifndef _SONAR_H
#define _SONAR_H

#include "SRL.h"
#include "Component.h"

#define NO_ECHO 0
#define PING_MEDIAN_DELAY 29000

namespace SRL
{
  class Sonar : public virtual SRL::Component
  {
    public:
      Sonar(uint8_t triggerPin, uint8_t echoPin, unsigned long maxDistanceCm = 22770);

      /* Get data from sensor */
      virtual unsigned long ping(void) = 0;
      double pingCm(void);
      double pingMm(void);
      unsigned long pingMedian(unsigned int interations = 5);
      double pingMedianCm(unsigned int interations = 5);
      double pingMedianMm(unsigned int interations = 5);

      virtual double convertCm(unsigned long us) = 0;
      double convertMm(unsigned long us);
      virtual unsigned long convertUs(double cm) = 0;


      /* Getters & setters */
      uint8_t getTriggerPin(void);
      void setTriggerPin(uint8_t triggerPin);
      uint8_t getEchoPin(void);
      void setEchoPin(uint8_t echoPin);

      double getMaxDistance(void);
      void setMaxDistance(double cm);
      void setMaxDistance(unsigned long us);

    protected:
      uint8_t triggerPin;
      uint8_t echoPin;

      unsigned long maxDistance;
  };
}

#endif
