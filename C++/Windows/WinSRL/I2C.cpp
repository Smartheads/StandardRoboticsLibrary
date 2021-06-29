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
#include "I2C.h"

/**
*	Constructor of the I2C device.
*
* @param address The address of the I2C device.
*/
SRL::I2CDevice::I2CDevice(uint8_t address)
{
	this->address = address;
	Wire.begin();
}

/**
*	Writes bytes from a byte array to the I2C device.
*	Note: untested since last change.
*
* @param reg The register to write to.
* @param bytec The number of bytes to write.
* @param bytev Byte pointer pointing to the first element of the array.
* @param start The index of the first byte of the array to write. Default value: 0.
* @return Returns 0 (false) if the data was sucessfully sent and 1 if not.
*/
uint8_t SRL::I2CDevice::writeBytes(uint8_t reg, uint8_t bytec, byte* bytev, uint8_t start)
{
	Wire.beginTransmission(address);
	Wire.write(reg);

	byte* data = new byte[bytec - start];

	for (int i = start; i < bytec; i++)
	{
		data[i-start] = *(bytev + i);
	}

	int x = Wire.write(data, bytec - start);
	Wire.endTransmission();

	delete[] data;
	return (x == bytec - start) ? 0 : 1;
}

/**
*	Read bytes from the I2C device.
*
*	@param reg The register to read from.
*	@param buff Pointer to the byte buffer.
*   @param len Number of bytes to read.
*	@return Returns 0 (false) if success, 1 if fails.
*/
uint8_t SRL::I2CDevice::readBytes(uint8_t reg, byte* buff, uint8_t len)
{
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.endTransmission();

	if(Wire.requestFrom(address, len) != len)
	{
		return 1;
	}

	for (int i = 0; i < len; i++)
	{
		*(buff + i) =  Wire.read();
	}
	
	return 0;
}

/**
*	Reads an unsigned short from the I2C device.
*
*	@param reg Register to read
*	@param ushort Pointer to the unsigned short to write to
*	@return Returns 0 (false) if successful, and 1 (true) if not.
*/
uint8_t SRL::I2CDevice::readUShort(uint8_t reg, unsigned short* ushort)
{
	byte buff[2];
	
	if (readBytes(reg, buff, 2) == 1)
	{
		return 1;
	}
	
	*ushort = (unsigned short) (buff[0] << 8 | buff[1]);
	
	return 0;
}

/**
*	Reads a signed short from the I2C device.
*
*	@param reg Register to read
*	@param ushort Pointer to the signed short to write to
*	@return Returns 0 (false) if successful and 1 (true) if not.
*/
uint8_t SRL::I2CDevice::readSShort(uint8_t reg, signed short* sshort)
{
	byte buff[2];
	
	if(readBytes(reg, buff, 2) == 1)
	{
		return 1;
	}
	
	*sshort = (signed short) (buff[0] << 8 | buff[1]);
	
	return 0;
}
