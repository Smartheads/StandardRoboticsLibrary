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
#ifndef _MPU6050_H
#define _MPU6050_H

#include "SRL.h"
#include "I2C.h"
#include "AccelGyro.h"
#include "Component.h"

#define MPU6050_COMPONENT_NAME "MPU6050"
#define MPU6050_ADDR         0x68
#define MPU6050_SMPLRT_DIV   0x19
#define MPU6050_CONFIG       0x1a
#define MPU6050_GYRO_CONFIG  0x1b
#define MPU6050_ACCEL_CONFIG 0x1c
#define MPU6050_WHO_AM_I     0x75
#define MPU6050_PWR_MGMT_1   0x6b
#define MPU6050_TEMP_H       0x41
#define MPU6050_TEMP_L       0x42

#define MPU6050_ACCELX_DATA  0x3b
#define MPU6050_ACCELY_DATA  0x3d
#define MPU6050_ACCELZ_DATA  0x3f
#define MPU6050_TEMP_DATA 	 0x41
#define MPU6050_GYROX_DATA	 0x43
#define MPU6050_GYROY_DATA	 0x45
#define MPU6050_GYROZ_DATA	 0x47

#define MPU6050_GYRO_CONFIG_FS_SEL_BIT  4
#define MPU6050_GYRO_CONFIG_FS_SEL_LENGTH 2

#define MPU6050_ACCEL_CONFIG_FS_SEL_BIT 4
#define MPU6050_ACCEL_CONFIG_FS_SEL_LENGTH 2

#define MPU6050_2G_ACCEL_SENSITIVITY 16384
#define MPU6050_4G_ACCEL_SENSITIVITY 8192
#define MPU6050_8G_ACCEL_SENSITIVITY 4096
#define MPU6050_16G_ACCEL_SENSITIVITY 2048

#define MPU6050_250DS_GYRO_SENSITIVITY 131
#define MPU6050_500DS_GYRO_SENSITIVITY 65.5
#define MPU6050_1000DS_GYRO_SENSITIVITY 32.8
#define MPU6050_2000DS_GYRO_SENSITIVITY 16.4

#define MPU6050_TEMP_BIAS 12412
#define MPU6050_TEMP_DIVISOR 340

namespace SRL
{
	/**
	*	Class MPU6050. A class for communicating with the MPU6050 accelerometer
	* and gyroscope.
	*/
	class MPU6050 : protected SRL::I2CDevice, public SRL::AccelGyro
	{
		public:
			MPU6050(uint8_t address, float aC = 0.02f, float gC = 0.98f);
			void initialize(void);

			/* Read data from device */
			int16_t getRawAccelX(void);
      		int16_t getRawAccelY(void);
      		int16_t getRawAccelZ(void);

			int16_t getRawGyroX(void);
			int16_t getRawGyroY(void);
			int16_t getRawGyroZ(void);

			int16_t getRawTemp(void);
			double getTemp(void);

			/* Getters and setters */
			uint8_t setAccelSensitivity(uint8_t setting);
			uint8_t setGyroSensitivity(uint8_t setting);
	};
}

#endif
