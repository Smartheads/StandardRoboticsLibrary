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
#include <I2C.h>

/**
*	Constructor of I2C device.
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
*
* @param reg The register to write to.
* @param bytec The number of bytes to write.
* @param bytev Byte pointer pointing to the first element of the array.
* @param start The index of the first byte of the array to write. Default value: 0.
*
*	@return
* Returns true if the data was sucessfully sent.
*/
bool SRL::I2CDevice::writeBytes(uint8_t reg, uint8_t bytec, byte* bytev, uint8_t start)
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

	return x == bytec - start;
}

/**
*	Read bytes from the I2C device.
*
*	@param reg The register to read from.
*	@param buff Pointer to the byte buffer.
* @param len Number of bytes to read.
*/
void SRL::I2CDevice::readBytes(uint8_t reg, byte* buff, uint8_t len)
{
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom(address, len);

	for (int i = 0; i < len; i++)
	{
		*(buff + i) =  Wire.read();
	}

	Wire.endTransmission();
}
