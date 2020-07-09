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
*
*	NOTE: This library only works on microcontrollers with at least 32 bit architectures.
*/
#ifndef SRL_BMP_180
#define SRL_BMP_180

#include <SRL.h>
#include <Component.h>
#include <I2C.h>
#include <Statistics.h>

#define BMP280_COMPONENT_NAME "BMP280"
#define BMP280_DEFAULT_ADDRESS 0x78
#define BMP280_CTRL_MEAS 0xF4
#define BMP280_PRESS 0xF7
#define BMP280_TEMP	0xFA

#define BMP280_DIG_T1 0x88
#define BMP280_DIG_T2 0x8A
#define BMP280_DIG_T3 0x8C
#define BMP280_DIG_P1 0x8E
#define BMP280_DIG_P2 0x90
#define BMP280_DIG_P3 0x92
#define BMP280_DIG_P4 0x94
#define BMP280_DIG_P5 0x96
#define BMP280_DIG_P6 0x98
#define BMP280_DIG_P7 0x9A
#define BMP280_DIG_P8 0x9C
#define BMP280_DIG_P9 0x9E

namespace SRL
{
	class BMP280 : public SRL::Component, public SRL::I2CDevice
	{
		public:
			BMP280(uint8_t addr = BMP280_DEFAULT_ADDRESS);
			~BMP280(void);
			
			uint8_t initialize(double basePressure = 101325);
			uint8_t setOversampling(unsigned int value);
			uint8_t setMode(byte value);
			
			void setBasePressure(double basePressure);
			double getBasePressure(void);
			void calibrateBasePressure(unsigned int samples = 5);
			
			double getTemperature(void);
			double getPressure(void);
			double getAltitude(void);
			
			double getMedianTemperature(unsigned int samples = 5);
			double getMedianPressure(unsigned int samples = 5);
			double getMedianAltitude(unsigned int samples = 5);
			
			
			typedef enum
			{
				ULTRA_LOW_POWER = 0x9,
				LOW_POWER = 0x11,
				STANDARD_RES = 0x19,
				HIGH_RES = 0x21,
				ULTRA_HIGH_RES = 0x22
			} oversampling;
			
			typedef enum
			{
				SLEEP_MODE = 0x00,
				FORCED_MODE = 0x01,
				NORMAL_MODE = 0x03
			} mode;
			
		private:
			double basePressure;
			
			// Calibration data
			unsigned short dig_T1, dig_P1;
			signed short dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
	};
}

#endif
