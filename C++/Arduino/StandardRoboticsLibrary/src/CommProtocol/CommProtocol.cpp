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

#include <CommProtocol.h>

/**
* Write a byte to the device.
*
*	@param reg The register to write to.
* @param data The byte to write.
*
* @return
*	Returns true if the data was sucessfully sent.
*/
uint8_t SRL::CommProtocol::writeByte(uint8_t reg, byte data)
{
	return writeBytes(reg, 1, &data);
}

/**
*	Overwrite specific bits of a byte in the device.
* Indexing: 7654 3210
*
* @param reg The register to write to.
* @param startBit The first bit's index which will be overwritten.
* @param len The length of the bit sequence which will be overwritten.
* @param data The byte who's bits will be copied.
*
* @return
*	Returns true if the data was sucessfully overwritten.
*/
uint8_t SRL::CommProtocol::writeBits(uint8_t reg, uint8_t startBit, uint8_t len, byte data)
{
	byte b = readByte(reg);

	byte mask = ((1 << len) - 1) << (startBit - len + 1);
	data <<= (startBit - len + 1);
	data &= mask;
	b &= ~(mask);
	b |= data;

	return writeByte(reg, b);
}

/**
*	Reads a byte from the device.
*
*	@param reg The register to read from.
*
*	@return
* Returns the byte read.
*/
byte SRL::CommProtocol::readByte(uint8_t reg)
{
	byte b;
	readBytes(reg, &b, 1);

	return b;
}

/**
*	Read two bytes from the device.
*
*	@param reg The register to read from.
*
*	@return
*	Returns the 2 bytes in int16_t form.
*/
int16_t SRL::CommProtocol::readInt16_t(uint8_t reg)
{
	return ((int16_t) readByte(reg)) << 8 | readByte(++reg);
}

uint8_t SRL::CommProtocol::getAddress(void)
{
	return address;
}

void SRL::CommProtocol::setAddress(uint8_t address)
{
	this->address = address;
}
