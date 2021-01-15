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
#ifndef _SRF05_H
#define _SRF05_H

#include "SRL.h"
#include "Sonar.h"
#include "Component.h"

#define SRF05_COMPONENT_NAME "SRF05"
#define PING_OVERHEAD 5
#define LINEAR_FUNCTION_SLOPE 0.015030304195946
#define LINEAR_FUNCTION_INTERSECT -2.6324

namespace SRL
{
  class SRF05 : public Sonar
  {
    public:
      SRF05(uint8_t triggerPin, uint8_t echoPin, double maxDistanceCm = 400);

      unsigned long ping(void);

      double convertCm(unsigned long us);
      unsigned long convertUs(double cm);

    private:
      void pingTrigger(void);
  };
}
#endif
