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
#ifndef SRL_SCOM_H
#define SRL_SCOM_H

#include <SRL.h>

#define VERSION 1204

#define SOH 1
#define STX 2
#define ETX	3
#define EOT 4
#define ACK 5
#define ETB 23

#define OK_CONTINUE 0
#define WAITING_FOR_MASTER_SIGNAL 1
#define WAITING_FOR_MASTER_RESPONCE 2
#define END_COMMUNICATION 3
#define WAITING_FOR_MASTER_COMMAND 4
#define SIGNAL_MODE 5
#define COMMAND_MODE 6
#define IN_TIMEOUT_BUFFER 7

namespace SRL
{
	int16_t calculateSum(String str);
	int16_t calculateSum(int16_t num);
	void sendInt16(int16_t signal);
	int16_t readInt16(void);
}

#endif
