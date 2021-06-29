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
#ifndef _I2C_H
#define _I2C_H

#include "SRL.h"
#include <Wire.h>
#include "CommProtocol.h"

namespace SRL
{
	/**
	*	Class I2CDevice. Defines a device that communicates with the I2C protocol.
	* Uses Wire.h
	* Includes methods to read and write data to the I2C device.
	*/
	class I2CDevice : virtual public SRL::CommProtocol
	{
		public:
			I2CDevice(uint8_t address);

			uint8_t writeBytes(uint8_t reg, uint8_t bytec, byte* bytev, uint8_t start = 0);
			uint8_t readBytes(uint8_t reg, byte* buff, uint8_t len);
			uint8_t readUShort(uint8_t reg, unsigned short* ushort);
			uint8_t readSShort(uint8_t reg, signed short* sshort);
	};
}

#endif
