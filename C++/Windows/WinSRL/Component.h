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
#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "SRL.h"

namespace SRL
{
  class Component
  {
    public:
      Component(void);
      Component(String name, unsigned int type);

      void initialize(void);

      /* Getters & setters: */
      unsigned int getId(void);
      String getName(void);
      unsigned int getType(void);

      /* Static variables */
      static unsigned int lastId;

      /* Enums */
      typedef enum
      {
        ROTARY_ENCODER = 1,
        ACCELEROMETER = 2,
        GYROSCOPE = 3,
        ACCEL_GYRO = 4,
        SONAR = 5,
        LIGHT = 6,
        SOUND = 7,
		BAROMETER = 8
      } types;

    protected:
      String name;
      unsigned int type;
      unsigned int id;
  };
}

#endif
