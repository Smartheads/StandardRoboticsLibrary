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
*	@return Returns 0 if successful and 1 if not.
*/
uint8_t SRL::BMP280::initialize(double basePressure)
{
	this->basePressure = basePressure;
	
	bool ok = true;
	
	// Read calibration data
	ok = readUShort(BMP280_DIG_T1, &dig_T1) == 0 ? ok : false;
	ok = readSShort(BMP280_DIG_T2, &dig_T2) == 0 ? ok : false;
	ok = readSShort(BMP280_DIG_T3, &dig_T3) == 0 ? ok : false;
	ok = readUShort(BMP280_DIG_P1, &dig_P1) == 0 ? ok : false;
	ok = readSShort(BMP280_DIG_P2, &dig_P2) == 0 ? ok : false;
	ok = readSShort(BMP280_DIG_P3, &dig_P3) == 0 ? ok : false;
	ok = readSShort(BMP280_DIG_P4, &dig_P4) == 0 ? ok : false;
	ok = readSShort(BMP280_DIG_P5, &dig_P5) == 0 ? ok : false;
	ok = readSShort(BMP280_DIG_P6, &dig_P6) == 0 ? ok : false;
	ok = readSShort(BMP280_DIG_P7, &dig_P7) == 0 ? ok : false;
	ok = readSShort(BMP280_DIG_P8, &dig_P8) == 0 ? ok : false;
	ok = readSShort(BMP280_DIG_P9, &dig_P9) == 0 ? ok : false;
	
	return (ok && setPowerMode(BMP280_NORMAL_MODE) == 0 &&
	 setPressureOversampling(BMP280_4X_OVERSAMPLING) == 0 &&
	 setTemperatureOversampling(BMP280_1X_OVERSAMPLING) == 0
	 ) ? 0 : 1;
}

/**
*	Sets oversampling value for pressure measurments.
*	@param value (see header file)
*	@see BMP280 data sheet 4.3.4.
*
*/
uint8_t SRL::BMP280::setPressureOversampling(byte value)
{
	if (value > BMP280_16X_OVERSAMPLING) // Value too large
		return 1;
	
	byte buff[1] = { 0 };
	if (readBytes(BMP280_CTRL_MEAS, buff, 1) != 0)
		return 1;
	
	byte b = 0xE3 & buff[0]; // 11100011 mask (clears old setting)
	b = b | (value << 2);
	
	return writeBytes(BMP280_CTRL_MEAS, 1, &b);
}

/**
*	Sets oversampling value for temperature measurments.
*	@param value (see header file)
*	@see BMP280 data sheet 4.3.4.
*
*/
uint8_t SRL::BMP280::setTemperatureOversampling(byte value)
{
	if (value > BMP280_16X_OVERSAMPLING) // Value too large
		return 1;
	
	byte buff[1] = { 0 };
	if (readBytes(BMP280_CTRL_MEAS, buff, 1) != 0)
		return 1;
	
	byte b = 0x1F & buff[0]; // 00011111 mask (clears old setting)
	b = b | (value << 5);
	
	return writeBytes(BMP280_CTRL_MEAS, 1, &b);
}

/**
*	Sets the BMP280's power mode.
*
*	@param value Mode value
*	@see BMP280 Product specification chapter 3.6.

*	@return Returns true if successfull.
*/
uint8_t SRL::BMP280::setPowerMode(byte value)
{
	byte buff[1] = { 0 }; // Current settings
	if (readBytes(BMP280_CTRL_MEAS, buff, 1) != 0)
		return 1;
	
	byte b = 0xFC & buff[0]; // 11111100 mask (clears old setting)
	b = b | value;
	
	return writeBytes(BMP280_CTRL_MEAS, 1, &b);
}

/**
*	Returns the latest pressure reading.
*
*	Calculation for 32-bit floating point measurements. Based on calculation
*		method found in BMP280 data sheet 8.1.
*
*	@return The latest pressure reading in Pa.
*/
double SRL::BMP280::getPressure(void)
{
	// Get uncalculated pressure measurements
	byte pbuffer[3] { 0 };
	readBytes(BMP280_PRESS, pbuffer, 3);

	int32_t adc_P = (int32_t) ((((uint32_t) pbuffer[0]) << 12) | (((uint32_t) pbuffer[1]) << 4) | (((uint32_t) pbuffer[2]) >> 4));
	int32_t t_fine = (int32_t) getTemperature() * 5120;
	
	double var1, var2, p;
	var1 = ((double)t_fine/2.0) - 64000.0;
	var2 = var1 * var1 * ((double)dig_P6) / 32768.0;
	var2 = var2 + var1 * ((double)dig_P5) * 2.0;
	var2 = (var2/4.0)+(((double)dig_P4) * 65536.0);
	var1 = (((double)dig_P3) * var1 * var1 / 524288.0 + ((double)dig_P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double)dig_P1);
	if (var1 == 0.0)
	{
		return 0;
	}
	p = 1048576.0 - (double)adc_P;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)dig_P9) * p * p / 2147483648.0;
	var2 = p * ((double)dig_P8) / 32768.0;
	p = p + (var1 + var2 + ((double)dig_P7)) / 16.0;
	
	return p;
}

/**
*	Returns the latest temperature reading.
*
*	Calculation for 32-bit floating point measurements. Based on calculation
*		method found in BMP280 data sheet 8.1.
*
*	@return The latest temperature reading in Celcius.
*/
double SRL::BMP280::getTemperature(void)
{
	byte tbuffer[3] = { 0 };
	readBytes(BMP280_TEMP, tbuffer, 3);
	int32_t adc_T = (int32_t) ((((uint32_t) tbuffer[0]) << 12) | (((uint32_t) tbuffer[1]) << 4) | (((uint32_t) tbuffer[2]) >> 4));
	
	double var1, var2, T;
	var1 = (((double)adc_T)/16384.0 - ((double)dig_T1)/1024.0) * ((double)dig_T2);
	var2 = ((((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0) *
		(((double)adc_T)/131072.0 - ((double)dig_T1)/8192.0)) * ((double)dig_T3);
	
	T = (var1 + var2) / 5120.0;
	
	return T;
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
	double readings[samples] = { 0.0 };
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
	double readings[samples] = { 0.0 };
	for (unsigned int i = 0; i < samples; i++)
	{
		readings[i] = getTemperature();
	}
	
	return getMedian<double>(samples, readings);
}

/**
*	Returns the median of a few altitude measurements.
*
*	@param samples The amount of measurements to take.
*	@return Median of temperature measurements
*/
double SRL::BMP280::getMedianAltitude(unsigned int samples)
{
	double readings[samples] = { 0.0 };
	for (unsigned int i = 0; i < samples; i++)
	{
		readings[samples] = getAltitude();
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
