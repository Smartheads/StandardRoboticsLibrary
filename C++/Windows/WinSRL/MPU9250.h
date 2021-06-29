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
#ifndef _MPU9250_H
#define _MPU9250_H

#include "SRL.h"
#include "I2C.h"
#include "AccelGyro.h"
#include "Component.h"

#define MPU9250_COMPONENT_NAME "MPU9250"
#define MPU9250_ADDR 0x68

#define MPU9250_GYRO_CONFIG  0x1b
#define MPU9250_ACCEL_CONFIG 0x1c
#define MPU9250_WHO_AM_I	 0x75

#define MPU9250_ACCELX_DATA  0x3b
#define MPU9250_ACCELY_DATA  0x3d
#define MPU9250_ACCELZ_DATA  0x3f
#define MPU9250_TEMP_DATA 	 0x41
#define MPU9250_GYROX_DATA	 0x43
#define MPU9250_GYROY_DATA	 0x45
#define MPU9250_GYROZ_DATA	 0x47

#define MPU9250_GYRO_CONFIG_FS_SEL_BIT  4
#define MPU9250_GYRO_CONFIG_FS_SEL_LENGTH 2

#define MPU9250_ACCEL_CONFIG_FS_SEL_BIT 4
#define MPU9250_ACCEL_CONFIG_FS_SEL_LENGTH 2

#define MPU9250_2G_ACCEL_SENSITIVITY 16384
#define MPU9250_4G_ACCEL_SENSITIVITY 8192
#define MPU9250_8G_ACCEL_SENSITIVITY 4096
#define MPU9250_16G_ACCEL_SENSITIVITY 2048

#define MPU9250_250DS_GYRO_SENSITIVITY 131
#define MPU9250_500DS_GYRO_SENSITIVITY 65.5
#define MPU9250_1000DS_GYRO_SENSITIVITY 32.8
#define MPU9250_2000DS_GYRO_SENSITIVITY 16.4

namespace SRL
{
	/**
	*	Class MPU9250. A class for communicating with the InvenSense MPU9250
	* accelerometer and gyroscope.MPU9250
	*/
	class MPU9250 : protected SRL::I2CDevice, public SRL::AccelGyro
	{
		public:
			MPU9250(uint8_t address = MPU9250_ADDR, float aC = 0.02f, float gC = 0.98f);
			~MPU9250(void);
			
			uint8_t initialize(void);
			
			/* Read data from device */
			int16_t getRawAccelX(void);
      		int16_t getRawAccelY(void);
      		int16_t getRawAccelZ(void);

			int16_t getRawGyroX(void);
			int16_t getRawGyroY(void);
			int16_t getRawGyroZ(void);
			
			/* Getters and setters */
			uint8_t setAccelSensitivity(uint8_t setting);
			uint8_t setGyroSensitivity(uint8_t setting);
	};
}

#endif
