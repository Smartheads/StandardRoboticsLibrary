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
#include "AccelGyro.h"

SRL::AccelGyro::AccelGyro(float aC, float gC)
{
	this->aC = aC;
	this->gC = gC;
	angleX = Angle();
	angleY = Angle();
	angleZ = Angle();
}

/**
*	Update the accel gyro's angles.
*
* @param sampleSize Number of samples to take median from on each axis's readings.
*/
void SRL::AccelGyro::update(unsigned int sampleSize)
{
	static unsigned long t1 = 0;
	unsigned long t2 = micros();

	update(t2 - t1, sampleSize);
	t1 = t2;
}

/**
*	Update the accel gyro's angles. Designed to be used in ISRs.
*
* @param deltaT The time ellapsed between update method calls. Time in micro seconds.
*	@param sampleSize Number of samples to take median from on each axis's readings.
*/
void SRL::AccelGyro::update(unsigned long deltaT, unsigned int sampleSize)
{
	static double prevGyroX = 0.0;
	static double prevGyroY = 0.0;
	static double prevGyroZ = 0.0;
	double gyroX = getGyroXMedian(sampleSize);
	double gyroY = getGyroYMedian(sampleSize);
	double gyroZ = getGyroZMedian(sampleSize);

	angleX.add((((prevGyroX + gyroX) / 2) * (deltaT / 1000000) * gC) + (aC * ((atan2f(getAccelYMedian(sampleSize), getAccelZMedian(sampleSize) + abs(getAccelXMedian(sampleSize))) * 180 ) / PI)));
	//angleY.add();
	//angleZ.add();

	prevGyroX = gyroX;
	prevGyroY = gyroY;
	prevGyroZ = gyroZ;
}

float SRL::AccelGyro::getAccelCoeff(void)
{
	return aC;
}

void SRL::AccelGyro::setAccelCoeff(float aC)
{
	this->aC = aC;
}

float SRL::AccelGyro::getGyroCoeff(void)
{
	return gC;
}

void SRL::AccelGyro::setGyroCoeff(float gC)
{
	this->gC = gC;
}

float SRL::AccelGyro::getAngleX(void)
{
	return angleX.getSize();
}

float SRL::AccelGyro::getAngleY(void)
{
	return angleY.getSize();
}

float SRL::AccelGyro::getAngleZ(void)
{
	return angleZ.getSize();
}

void SRL::AccelGyro::setAngleX(float angle)
{
	angleX.setAngle(angle);
}

void SRL::AccelGyro::setAngleY(float angle)
{
	angleY.setAngle(angle);
}

void SRL::AccelGyro::setAngleZ(float angle)
{
	angleZ.setAngle(angle);
}
