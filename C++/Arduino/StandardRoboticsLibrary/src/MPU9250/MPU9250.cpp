/*
* MIT License
*
* Copyright (c) 2020 Robert Hutter
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
#include <MPU9250.h>


/**
*	Constructor of the MPU9250 class.
*
*	@param address The address of the MPU9250.
*   @param aC Complementary filter's accelerometer coefficient. Default value: 0.02
*	@param gC Complementary filter's gyroscope coefficient. Default value: 0.98
*/
SRL::MPU9250::MPU9250(uint8_t address, float aC, float gC) : I2CDevice(address), Component(MPU9250_COMPONENT_NAME, Component::ACCEL_GYRO), AccelGyro(aC, gC)
{

}

/**
*	Initializes the MPU9250 for use.
*	@return Returns 0 if successfull and 1 if not.
*/
uint8_t SRL::MPU9250::initialize(void)
{
	setAccelSensitivity(0);
	setGyroSensitivity(0);
	
	setAccelOffsets(0, 0, 0);
	setGyroOffsets(0, 0, 0);

	byte buff[1];
	return readBytes(MPU9250_WHO_AM_I, buff, 1);
}

/**
*	Set the MPU9250's full scale accelerometer range.
*	Sensitivity table:
*	2g = 0
*	4g = 1
* 8g = 2
* 16g = 3
*
*	@param setting The sensitivity setting.
*
* @return
* Returns 1 if the setting was sucessfully updated.
*/
uint8_t SRL::MPU9250::setAccelSensitivity(uint8_t setting)
{
	switch (setting)
	{
		case 0:
			accelSensitivity = MPU9250_2G_ACCEL_SENSITIVITY;
			break;

		case 1:
			accelSensitivity = MPU9250_4G_ACCEL_SENSITIVITY;
			break;

		case 2:
			accelSensitivity = MPU9250_8G_ACCEL_SENSITIVITY;
			break;

		case 3:
			accelSensitivity = MPU9250_16G_ACCEL_SENSITIVITY;
			break;

		default:
			return 1;
	}

	return writeBits(MPU9250_ACCEL_CONFIG, MPU9250_ACCEL_CONFIG_FS_SEL_BIT, MPU9250_ACCEL_CONFIG_FS_SEL_LENGTH, setting);
}

/**
*	Set the the MPU9250's full scale gyroscope range.
*	Sensitivity table:
*	250°/s = 0
*	500°/s = 1
*	1000°/s = 2
* 2000°/s = 3
*
*	@param setting The sensitivity setting.
*
*	@return
*	Returns 0 if the setting was sucessfully updated.
*/
uint8_t SRL::MPU9250::setGyroSensitivity(uint8_t setting)
{
	switch (setting)
	{
		case 0:
			gyroSensitivity = MPU9250_250DS_GYRO_SENSITIVITY;
			break;

		case 1:
			gyroSensitivity = MPU9250_500DS_GYRO_SENSITIVITY;
			break;

		case 2:
			gyroSensitivity = MPU9250_1000DS_GYRO_SENSITIVITY;
			break;

		case 3:
			gyroSensitivity = MPU9250_2000DS_GYRO_SENSITIVITY;
			break;

		default:
			return 1;
	}

	return writeBits(MPU9250_GYRO_CONFIG, MPU9250_GYRO_CONFIG_FS_SEL_BIT, MPU9250_GYRO_CONFIG_FS_SEL_LENGTH, setting);
}

int16_t SRL::MPU9250::getRawAccelX(void)
{
	return readInt16_t(MPU9250_ACCELX_DATA);
}

int16_t SRL::MPU9250::getRawAccelY(void)
{
	return readInt16_t(MPU9250_ACCELY_DATA);
}

int16_t SRL::MPU9250::getRawAccelZ(void)
{
	return readInt16_t(MPU9250_ACCELZ_DATA);
}

int16_t SRL::MPU9250::getRawGyroX(void)
{
	return readInt16_t(MPU9250_GYROX_DATA);
}

int16_t SRL::MPU9250::getRawGyroY(void)
{
	return readInt16_t(MPU9250_GYROY_DATA);
}

int16_t SRL::MPU9250::getRawGyroZ(void)
{
	return readInt16_t(MPU9250_GYROZ_DATA);
}
