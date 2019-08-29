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

#include <BMP280.h>

/**
*	Constructor of class BMP280.
*
*	@param addr The I2C address of the device. Defualt 0x78.
*/
SRL::BMP280::BMP280(uint8_t addr) : Component(BMP280_COMPONENT_NAME, BAROMETER), I2CDevice(addr)
{
	
}

/**
*	Destructor of class BMP280
*
*/
SRL::BMP280::~BMP280(void)
{
	
}

/**
*	Initializer method for class BMP280.
*
*	@return bool True if successfull.
*/
bool SRL::BMP280::initialize(void)
{
	Wire.begin();
	
	// Read calibration data
	readUShort(BMP280_DIG_T1, &dig_t1);
	readSShort(BMP280_DIG_T2, &dig_t2);
	readSShort(BMP280_DIG_T3, &dig_t3);
	readUShort(BMP280_DIG_P1, &dig_p1);
	readSShort(BMP280_DIG_P2, &dig_p2);
	readSShort(BMP280_DIG_P3, &dig_p3);
	readSShort(BMP280_DIG_P4, &dig_p4);
	readSShort(BMP280_DIG_P5, &dig_p5);
	readSShort(BMP280_DIG_P6, &dig_p6);
	readSShort(BMP280_DIG_P7, &dig_p7);
	readSShort(BMP280_DIG_P8, &dig_p8);
	readSShort(BMP280_DIG_P9, &dig_p9);
	
	return setMode(NORMAL_MODE) && setOversampling(STANDARD_RES);
}

/**
*	Sets the oversampling setting.
*
*	Oversampling value:
*	XXXYYY (binary)
*
*	XXX: osrs_p, Pressure oversampling
*	YYY: osrs_t, Temperature oversampling
*
*	@param value Oversampling value
*	@return Returns true if successfull.
*/
bool SRL::BMP280::setOversampling(unsigned int value)
{	
	byte* buff = new byte(2);
	
	readBytes(BMP280_CTRL_MEAS, buff, 2);
	byte mode = buff[1] << 2;
	
	buff[0] = value >> 2;
	buff[1] = (value << 2) | mode;
	
	bool ret = writeBytes(BMP280_CTRL_MEAS, 2, buff);
	
	delete buff;
	return ret;
}

/**
*	Sets the BMP280's operation mode.
*
*	@param value Mode value
*	@return Returns true if successfull.
*/
bool SRL::BMP280::setMode(byte value)
{
	byte* buff = new byte(2);
	
	readBytes(BMP280_CTRL_MEAS, buff, 2);

	buff[1] &= 0xC; // Mask
	buff[1] = buff[1] | value;
	
	bool ret = writeBytes(BMP280_CTRL_MEAS, 2, buff);
	
	delete buff;
	return ret;
}

/**
*	Reads an unsigned short from the BMP280.
*
*	@param reg Register to read
*	@param ushort Pointer to the unsigned short to write to
*/
void SRL::BMP280::readUShort(uint8_t reg, unsigned short* ushort)
{
	byte* buff = new byte(2);
	readBytes(reg, buff, 2);
	*ushort = unsigned short (buff[0] << 8 | buff[1]);
	delete buff;
}

/**
*	Reads a signed short from the BMP280.
*
*	@param reg Register to read
*	@param ushort Pointer to the signed short to write to
*/
void SRL::BMP280::readSShort(uint8_t reg, signed short* sshort)
{
	byte* buff = new byte(2);
	readBytes(reg, buff, 2);
	*sshort = signed short (buff[0] << 8 | buff[1]);
	delete buff;
}