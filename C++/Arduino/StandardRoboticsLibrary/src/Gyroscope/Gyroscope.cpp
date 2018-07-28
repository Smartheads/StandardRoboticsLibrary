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
#include <Gyroscope.h>

/**
*	Set the gyroscope's offsets.
*
*	@param x The x-axis's offset.
*	@param y The y-axis's offset.
*	@param z The z-axis's offset.
*/
void SRL::Gyroscope::setGyroOffsets(int16_t x, int16_t y, int16_t z)
{
	setGyroXOffset(x);
	setGyroYOffset(y);
	setGyroZOffset(z);
}

/**
*	Calculate the gyroscope's offsets.
*
*	@param console Boolean print data to console.
*	@param iterations The sample size used in the calculation. Default value: 3000
*/
void SRL::Gyroscope::calcGyroOffsets(bool console, unsigned int iterations)
{
	int16_t gyro[] = {0, 0, 0};

	if (console)
	{
		Serial.println("Calculating gyro offsets...");
	}

	for (int i = 0; i < iterations; i++)
	{
		gyro[0] += getRawGyroX();
		gyro[1] += getRawGyroY();
		gyro[2] += getRawGyroZ();
	}

	setGyroOffsets(gyro[0] / iterations, gyro[1] / iterations, gyro[2] / iterations);

	if (console)
	{
		Serial.print("Your gyro offsets are: ");
		Serial.print(getGyroXOffset()); Serial.print(" ");
		Serial.print(getGyroYOffset()); Serial.print(" ");
		Serial.println(getGyroZOffset());
		Serial.println("The gyro readings should be 0 0 0. They are: ");
		Serial.print(getRawGyroX()); Serial.print(" ");
		Serial.print(getRawGyroY()); Serial.print(" ");
		Serial.println(getRawGyroZ());
	}
}

double SRL::Gyroscope::getGyroX(void)
{
	return (getRawGyroX() - gyroXOffset) / gyroSensitivity;
}

double SRL::Gyroscope::getGyroY(void)
{
	return (getRawGyroY() - gyroYOffset) / gyroSensitivity;
}

double SRL::Gyroscope::getGyroZ(void)
{
	return (getRawGyroZ() - gyroXOffset) / gyroSensitivity;
}

int16_t SRL::Gyroscope::getGyroXOffset(void)
{
	return gyroXOffset;
}

void SRL::Gyroscope::setGyroXOffset(int16_t offset)
{
	this->gyroXOffset = offset;
}

int16_t SRL::Gyroscope::getGyroYOffset(void)
{
	return gyroYOffset;
}

void SRL::Gyroscope::setGyroYOffset(int16_t offset)
{
	this->gyroYOffset = offset;
}

int16_t SRL::Gyroscope::getGyroZOffset(void)
{
	return gyroZOffset;
}

void SRL::Gyroscope::setGyroZOffset(int16_t offset)
{
	this->gyroZOffset = offset;
}
