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
*/
#include "Accelerometer.h"

/**
*	Set the accelerometer's offsets.
*
*	@param x The x-axis's offset.
*	@param y The y-axis's offset.
*	@param z The z-axis's offset.
*/
void SRL::Accelerometer::setAccelOffsets(int16_t x, int16_t y, int16_t z)
{
	setAccelXOffset(x);
	setAccelYOffset(y);
	setAccelZOffset(z);
}

/**
*	Calculate the accelerometer's offsets, based on its default orientation.
*
*	@param orientation The accelerometer's orientation.
* @param console Output stuff to serial on the progress of the calibration.
* @param iterations The number of iterations used to Calculate the offsets.
*/
void SRL::Accelerometer::calcAccelOffsets(uint8_t orientation, bool console, unsigned int iterations)
{
	int16_t targetX = 0, targetY = 0, targetZ = 0;

	if (console)
	{
		Serial.println("Calibrating accelerometer");
	}

	switch (orientation) {
		case X_UP:
			targetX = accelSensitivity;
			break;

		case X_DOWN:
			targetX = -1 * accelSensitivity;
			break;

		case Y_UP:
			targetY = accelSensitivity;
			break;

		case Y_DOWN:
			targetY = -1 * accelSensitivity;
			break;

		case Z_UP:
			targetZ = accelSensitivity;
			break;

		case Z_DOWN:
			targetZ = -1 * accelSensitivity;
			break;

		default:
			calcAccelOffsets();
			return;
	}

	for (int x = 0; x < iterations; x++)
	{
		long accelX = 0, accelY = 0, accelZ = 0;

		for (unsigned int i = 0; i < ACCEL_CALIBRATION_I; i++)
		{
				accelX += getRawAccelX() - getAccelXOffset();
				accelY += getRawAccelY() - getAccelYOffset();
				accelZ += getRawAccelZ() - getAccelZOffset();
		}

		if (console)
		{
			Serial.print(".");
		}

		accelX /= ACCEL_CALIBRATION_I;
		accelY /= ACCEL_CALIBRATION_I;
		accelZ /= ACCEL_CALIBRATION_I;

		setAccelOffsets(
			 (targetX - accelX) * -1 + getAccelXOffset(),
			 (targetY - accelY) * -1 + getAccelYOffset(),
			 (targetZ - accelZ) * -1 + getAccelZOffset()
		 );
	}

	if (console)
	{
		Serial.println("\nCalibration complete! Your offsets are:");
		Serial.print("x: "); Serial.print(getAccelXOffset(), DEC);
		Serial.print(" y: "); Serial.print(getAccelYOffset(), DEC);
		Serial.print(" z: "); Serial.println(getAccelZOffset(), DEC);
		Serial.println("Your accelerometer readings are:");
		Serial.print("x: "); Serial.print(getRawAccelX() - getAccelXOffset());
		Serial.print(" y: "); Serial.print(getRawAccelY() - getAccelYOffset());
		Serial.print(" z: "); Serial.println(getRawAccelZ() - getAccelZOffset());
	}
}

/**
*	Returns the median of raw accelerometer readings on the x-axis.
*	Note: untested since last change.
*
* @param iterations The number of readings to take.
*/
int16_t SRL::Accelerometer::getRawAccelXMedian(unsigned int iterations)
{
	int16_t* samples = new int16_t[iterations];
	for (unsigned int i = 0; i < iterations; i++)
	{
		samples[i] = getRawAccelX();
	}

	int16_t result = getMedian<int16_t>(iterations, samples);
	delete[] samples;
	return result;
}

/**
*	Returns the median of raw accelerometer readings on the y-axis.
*	Note: untested since last change.
*
* @param iterations The number of readings to take.
*/
int16_t SRL::Accelerometer::getRawAccelYMedian(unsigned int iterations)
{
	int16_t* samples = new int16_t[iterations];
	for (unsigned int i = 0; i < iterations; i++)
	{
		samples[i] = getRawAccelY();
	}

	int16_t result = getMedian<int16_t>(iterations, samples);
	delete[] samples;
	return result;
}
/**
*	Returns the median of raw accelerometer readings on the z-axis.
*	Note: untested since last change.
*
* @param iterations The number of readings to take.
*/
int16_t SRL::Accelerometer::getRawAccelZMedian(unsigned int iterations)
{
	int16_t* samples = new int16_t[iterations];
	for (unsigned int i = 0; i < iterations; i++)
	{
		samples[i] = getRawAccelZ();
	}

	int16_t result = getMedian<int16_t>(iterations, samples);
	delete[] samples;
	return result;
}

double SRL::Accelerometer::getAccelXMedian(unsigned int iterations)
{
	return (getRawAccelXMedian() - accelXOffset) / accelSensitivity;
}

double SRL::Accelerometer::getAccelYMedian(unsigned int iterations)
{
	return (getRawAccelXMedian() - accelYOffset) / accelSensitivity;
}

double SRL::Accelerometer::getAccelZMedian(unsigned int iterations)
{
	return (getRawAccelZMedian() - accelXOffset) / accelSensitivity;
}

double SRL::Accelerometer::getAccelX(void)
{
	return (getRawAccelX() - accelXOffset) / accelSensitivity;
}

double SRL::Accelerometer::getAccelY(void)
{
	return (getRawAccelY() - accelYOffset) / accelSensitivity;
}

double SRL::Accelerometer::getAccelZ(void)
{
	return (getRawAccelZ() - accelZOffset) / accelSensitivity;
}

int16_t SRL::Accelerometer::getAccelXOffset(void)
{
	return accelXOffset;
}

void SRL::Accelerometer::setAccelXOffset(int16_t offset)
{
	this->accelXOffset = offset;
}

int16_t SRL::Accelerometer::getAccelYOffset(void)
{
	return accelYOffset;
}

void SRL::Accelerometer::setAccelYOffset(int16_t offset)
{
	this->accelYOffset = offset;
}

int16_t SRL::Accelerometer::getAccelZOffset(void)
{
	return accelZOffset;
}

void SRL::Accelerometer::setAccelZOffset(int16_t offset)
{
	this->accelZOffset = offset;
}
