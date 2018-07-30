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
#include <Accelerometer.h>

SRL::Accelerometer::Accelerometer(String name) : Component(name)
{
	
}

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
