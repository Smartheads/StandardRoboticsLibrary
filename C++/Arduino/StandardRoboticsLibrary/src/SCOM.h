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
#ifndef SRL_SCOM_H
#define SRL_SCOM_H

#include <SRL.h>

#define VERSION 1206

#define SOH 0x01
#define STX 0x02
#define ETX	0x03
#define EOT 0x04
#define ABF 0x05
#define ACK 0x06
#define ANT 0x07

#define OK_CONTINUE 0x00
#define WAITING_FOR_ASCII_SUM 0x01
#define IN_ACK_TIMEOUT_BUFFER 0x02
#define WAITING_FOR_INFO_MESSAGE 0x03
//#define WAITING_FOR_MORE_ABF 0x04
#define WAITING_FOR_ACK 0x05
#define WAITING_FOR_REP_MESSAGE 0x06
#define WAITING_FOR_ANT 0x07
#define CONNECTION_CLOSED 0x08
#define WAITING_FOR_MORE_ANT 0x09
#define WAITING_FOR_SIGNAL 0x10

#define TIMEOUT 1000L
#define ABF_INTERVAL 1000L
#define ACK_WAIT 550L
#define ACK_TIMEOUT 400L
#define REFRESH_INTERVAL 10L

#define SIGNAL_LENGTH 4
#define MAX_ABF_ATTEMPTS 10

namespace SRL
{
	
	class Signal
	{
		public:
			Signal(int16_t messageId, int16_t message);
			Signal(byte* buffer);
			Signal(Signal* sig);
			~Signal(void);
			
			int16_t getMessageId(void);
			int16_t getMessage(void);
			unsigned long getCreatedAt(void);
			
			void setMessageId(int16_t messageId);
			void setMessage(int16_t message);
			
			int16_t getSum(void);
			
			byte* getBytes(void);
		
		private:
			byte* sigmem;
			unsigned long createdAt;
	};
	
	class InfoMessage
	{
		public:
			InfoMessage(String message);
			InfoMessage(InfoMessage* msg);
			
			String getMessage();
			int16_t getSum(void);
			
		private:
			String message;
	};
	
	int16_t calculateSum(String str);
	int16_t calculateSum(int16_t num);
	void writeSignal(Signal* sig);
	Signal* readSignal(void);
	InfoMessage* readInfoMessage(void);
	void clearSerialBuffer(void);
}

#endif
