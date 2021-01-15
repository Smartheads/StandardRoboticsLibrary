/*
* MIT License
*
* Copyright (c) 2020 Robert Hutter
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
*
* RGBLED.h - Header file of RGBLED.cpp. Contains class and function prototypes
*   for RGB capable light emitting diodes.
*
*/

#ifndef RGBLED_H
#define RGBLED_H

#include "SRL.h"
#include "Component.h"

#define RGBLED_COMPONENT_NAME "RGBLED"

#define RED SRL::color {255, 0, 255}
#define GREEN SRL::color {0, 255, 0}
#define BLUE SRL::color {0, 0, 255}
#define WHITE SRL::color {255, 255, 255}

#define ON true
#define OFF false

namespace SRL
{

	typedef unsigned char byte;

	typedef struct
	{
		byte red;
		byte green;
		byte blue;
	} color;
	
	bool compareColor(color a, color b);

	class rgbled : public Component
	{
		public:
			rgbled(uint8_t redpin, uint8_t greedpin, uint8_t bluepin);
			~rgbled();
		
			void setColor(byte red, byte green, byte blue);
			void setColor(color ledcol);
			color getColor(void);
		
			void turnOff(void);
			void turnOn(void);
			void setState(bool state);
			bool getState(void);
		
		private:
			color* const ledcol = new color;
			uint8_t* const redpin = new uint8_t;
			uint8_t* const greenpin = new uint8_t;
			uint8_t* const bluepin = new uint8_t;
			bool* const ledstate = new bool;
	};
}

#endif
