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
*	@param basePressure Pressure reading at ground level in Pa. Used in calculating altitude.
*	@return bool True if successfull.
*/
bool SRL::BMP280::initialize(double basePressure)
{
	Wire.begin();
	
	this->basePressure = basePressure;
	
	// Read calibration data
	readUShort(BMP280_DIG_T1, &dig_T1);
	readSShort(BMP280_DIG_T2, &dig_T2);
	readSShort(BMP280_DIG_T3, &dig_T3);
	readUShort(BMP280_DIG_P1, &dig_P1);
	readSShort(BMP280_DIG_P2, &dig_P2);
	readSShort(BMP280_DIG_P3, &dig_P3);
	readSShort(BMP280_DIG_P4, &dig_P4);
	readSShort(BMP280_DIG_P5, &dig_P5);
	readSShort(BMP280_DIG_P6, &dig_P6);
	readSShort(BMP280_DIG_P7, &dig_P7);
	readSShort(BMP280_DIG_P8, &dig_P8);
	readSShort(BMP280_DIG_P9, &dig_P9);
	
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
	
	delete[] buff;
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
	
	delete[] buff;
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
	*ushort = (unsigned short) (buff[0] << 8 | buff[1]);
	delete[] buff;
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
	*sshort = (signed short) (buff[0] << 8 | buff[1]);
	delete[] buff;
}

/**
*	Returns the latest pressure reading.
*
*	@return The latest pressure reading in pa.
*/
double SRL::BMP280::getPressure(void)
{
	// Get uncalculated pressure measurements
	byte* pbuffer = new byte[3];
	readBytes(BMP280_PRESS, pbuffer, 3);

	double press = (double)(pbuffer[0] * 4096 + pbuffer[1] * 16 + pbuffer[2] / 16);
	double temp = getTemperature() * 5120.0;
	
	delete[] pbuffer;
	
	double var1 = (temp / 2.0) - 64000.0;
	double var2 = var1 * var1 * dig_P6 / 32768.0;
	var2 = var2 + var1 * dig_P5 * 2.0;
	var1 = (dig_P3 * var1 * var1 / 524288.0 + dig_P2 * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0) * dig_P1;
	
	if (var1 == 0.0)
	{
		return 0;
	}
	
	double p = 1048576.0 - press;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = dig_P9 * p * p / 2147483648.0;
	var2 = p * dig_P8 / 32768.0;
	p = p + (var1 + var2 + dig_P7) / 16.0;
	
	return p;
}

/**
*	Returns the latest temperature reading.
*
*	@return The latest temperature reading in Celcius.
*/
double SRL::BMP280::getTemperature(void)
{
	byte* tbuffer = new byte[3];
	readBytes(BMP280_TEMP, tbuffer, 3);
	
	double temp = (double)(tbuffer[0] * 4096 + tbuffer[1] * 16 + tbuffer[2] / 16);
	
	double var1 = (temp / 16384.0 - dig_T1 / 1024.0) * dig_T2;
	double var2 = ((temp / 131072.0 - dig_T1 / 8192.0) * (temp / 131072.0 - dig_T1 / 8192.0)) * dig_T3;
	
	return (var1 + var2) / 5120.0;
}

/**
*	Returns the latest altitude reading.
*
*	@return The latest altitude reading in meters.
*/
double SRL::BMP280::getAltitude(void)
{
	return (44330.0 * (1 - pow(getPressure() / basePressure, 1 / 5.255)));
}

/**
*	Returns the median of a few pressure measurements.
*
*	@param samples The amount of measurements to take.
*	@return Median of pressure measurements.
*/
double SRL::BMP280::getMedianPressure(unsigned int samples)
{
	double readings[samples];
	for (unsigned int i = 0; i < samples; i++)
	{
		readings[i] = getPressure();
	}
	
	return getMedian<double>(samples, readings);
}

/**
*	Returns the median of a few temperature measurements.
*
*	@param samples The amount of measurements to take.
*	@return Median of temperature measurements.
*/
double SRL::BMP280::getMedianTemperature(unsigned int samples)
{
	double readings[samples];
	for (unsigned int i = 0; i < samples; i++)
	{
		readings[i] = getTemperature();
	}
	
	return getMedian<double>(samples, readings);
}

/**
*	Sets the base pressure setting.
*
*	@param basePressure Pressure reading at the ground level in Pa.
*/
void SRL::BMP280::setBasePressure(double basePressure)
{
	this->basePressure = basePressure;
}

/**
*	Returns the base pressure setting.
*
*	@return Base pressure settin in Pa.
*/
double SRL::BMP280::getBasePressure(void)
{
	return basePressure;
}

/**
*	Sets the base pressure setting to the median of pressure readings.
*
*	@param samples The number of measurements to take.
*/
void SRL::BMP280::calibrateBasePressure(unsigned int samples)
{
	basePressure = getMedianPressure(samples) / 10;
}
