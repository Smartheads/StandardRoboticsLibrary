/*
* MIT License
*
* Copyright (c) 2019 Robert Hutter
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
#ifndef SCOM_SLAVE_H
#define SCOM_SLAVE_H

#include <SCOM.h>

namespace SRL
{
	class Slave
	{
		public:
			Slave(void);
			~Slave(void);
			
			uint8_t openSCOM(unsigned int baudRate);
			void updateSCOM(void);
			
		private:
			Signal* lastSentSignal;
			int16_t awaitedSum;
			unsigned int status;
			int16_t messageId;
			
			unsigned long sumSentAt;
			
			void sendSignal(int16_t message);
			void sendSum(int16_t sum);
	};
}

#endif
