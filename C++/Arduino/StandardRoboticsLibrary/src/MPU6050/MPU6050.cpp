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
#include <MPU6050.h>

/**
*	Constructor of the MPU6050 class.
*
*	@param address The address of the MPU6050.
* @param aC Complementary filter's accelerometer coefficient. Default value: 0.02
*	@param gC Complementary filter's gyroscope coefficient. Default value: 0.98
*/
SRL::MPU6050::MPU6050(uint8_t address, float aC, float gC) : I2CDevice(address), Component(MPU6050_COMPONENT_NAME, Component::ACCEL_GYRO), AccelGyro(aC, gC)
{

}

/**
*	Initialize the MPU6050.
*	Must be called before using the sensor.
*/
void SRL::MPU6050::initialize(void)
{
		writeByte(MPU6050_SMPLRT_DIV, 0x00);
		writeByte(MPU6050_CONFIG, 0x00);
		writeByte(MPU6050_PWR_MGMT_1, 0x01);

		setAccelSensitivity(0);
		setGyroSensitivity(0);

		setAccelOffsets(0, 0, 0);
		setGyroOffsets(0, 0, 0);
}

/**
*	Set the MPU6050's full scale accelerometer range.
*	Sensitivity table:
*	2g = 0
*	4g = 1
* 8g = 2
* 16g = 3
*
*	@param setting The sensitivity setting.
*
* @return
* Returns 0 if the setting was sucessfully updated.
*/
uint8_t SRL::MPU6050::setAccelSensitivity(uint8_t setting)
{
	switch (setting)
	{
		case 0:
			accelSensitivity = MPU6050_2G_ACCEL_SENSITIVITY;
			break;

		case 1:
			accelSensitivity = MPU6050_4G_ACCEL_SENSITIVITY;
			break;

		case 2:
			accelSensitivity = MPU6050_8G_ACCEL_SENSITIVITY;
			break;

		case 3:
			accelSensitivity = MPU6050_16G_ACCEL_SENSITIVITY;
			break;

		default:
			return 1;
	}

	return writeBits(MPU6050_ACCEL_CONFIG, MPU6050_ACCEL_CONFIG_FS_SEL_BIT, MPU6050_ACCEL_CONFIG_FS_SEL_LENGTH, setting);
}

/**
*	Set the the MPU6050's full scale gyroscope range.
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
uint8_t SRL::MPU6050::setGyroSensitivity(uint8_t setting)
{
	switch (setting)
	{
		case 0:
			gyroSensitivity = MPU6050_250DS_GYRO_SENSITIVITY;
			break;

		case 1:
			gyroSensitivity = MPU6050_500DS_GYRO_SENSITIVITY;
			break;

		case 2:
			gyroSensitivity = MPU6050_1000DS_GYRO_SENSITIVITY;
			break;

		case 3:
			gyroSensitivity = MPU6050_2000DS_GYRO_SENSITIVITY;
			break;

		default:
			return 1;
	}

	return writeBits(MPU6050_GYRO_CONFIG, MPU6050_GYRO_CONFIG_FS_SEL_BIT, MPU6050_GYRO_CONFIG_FS_SEL_LENGTH, setting);
}

int16_t SRL::MPU6050::getRawAccelX(void)
{
	return readInt16_t(MPU6050_ACCELX_DATA);
}

int16_t SRL::MPU6050::getRawAccelY(void)
{
	return readInt16_t(MPU6050_ACCELY_DATA);
}

int16_t SRL::MPU6050::getRawAccelZ(void)
{
	return readInt16_t(MPU6050_ACCELZ_DATA);
}

int16_t SRL::MPU6050::getRawGyroX(void)
{
	return readInt16_t(MPU6050_GYROX_DATA);
}

int16_t SRL::MPU6050::getRawGyroY(void)
{
	return readInt16_t(MPU6050_GYROY_DATA);
}

int16_t SRL::MPU6050::getRawGyroZ(void)
{
	return readInt16_t(MPU6050_GYROZ_DATA);
}

int16_t SRL::MPU6050::getRawTemp(void)
{
	return readInt16_t(MPU6050_TEMP_DATA);
}

double SRL::MPU6050::getTemp(void)
{
	return (getRawTemp() + MPU6050_TEMP_BIAS) / MPU6050_TEMP_DIVISOR;
}
