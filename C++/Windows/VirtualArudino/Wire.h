/*
* MIT License
*
* Copyright (c) 2021 Robert Hutter
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
* Wire.h - Header file of Wire source code. Part of Virtual Arduino runtime
*	environment.
*
*/

#ifndef __WIRE_H__
#define __WIRE_H__

#include "VirtualArduino.h"
#include <queue>

namespace vard
{
	class HardwareWire
	{
	public:
		HardwareWire(void);
		~HardwareWire(void);

		bool begin(void);
		bool beginTransmission(uint8_t);
		bool write(uint8_t);
		bool write(uint8_t*, uint8_t);
		bool endTransmission(void);

		uint8_t requestFrom(uint8_t, uint8_t);
		uint8_t read(void);

		bool isconnected = false, isavailable = true;

	private:
		uint8_t targetaddr = 0;
		unsigned int pos = 0;
		std::queue<uint8_t> readbuffer;
		std::queue<uint8_t> writebuffer;
	};
}

extern vard::HardwareWire Wire;

#endif