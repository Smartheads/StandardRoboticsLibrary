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
#include <SCOM.h>

/**
*
*	Writes a Signal to serial.
*
*/
void SRL::writeSignal(Signal* sig)
{
	Serial.write(sig->getBytes(), SIGNAL_LENGTH);
}

/**
*	Reads a Signal from the Serial buffer.
*
*
*/
SRL::Signal* SRL::readSignal(void)
{
	int len = Serial.available();
	byte* k = new byte[len];
	Serial.readBytes(k, len);
	
	byte* buffer = new byte[4];
	buffer[0] = k[0];
	buffer[1] = k[1];
	buffer[2] = k[2];
	buffer[3] = k[3];
	
	delete[] k;
	
	return new Signal(buffer);
}

/**
*  Reads an info message from the Serial Buffer.
*
*
*/
SRL::InfoMessage* SRL::readInfoMessage(void)
{	
	return new InfoMessage(Serial.readString());
}

/**
*	Returns the ASCII sum of a string.
*	Tested. Works 100%.
*
*	@param str
*/
int16_t SRL::calculateSum(String str)
{
	char* c = new char[str.length()+1];
    str.toCharArray(c, str.length()+1);

    int16_t sum = 0;
    for (int i = 0; i < str.length()+1; i++)
    {
      sum += (int16_t) c[i];
    }
    delete[] c;
  
    return sum;
}

/**
*	Returns the ASCII sum of a signal.
*
*	@param signal Signal
*/
int16_t SRL::calculateSum(int16_t signal)
{
	return calculateSum(String(signal, DEC));
}

/**
*	Clears the Serial buffer by reading asnd deleting all
*	bytes in the buffer.
*
*/
void SRL::clearSerialBuffer(void)
{
	byte* cb = new byte[Serial.available()];
	Serial.readBytes(cb, Serial.available());
	delete[] cb;	
}

/**
*
*
*
*/
SRL::Signal::Signal(int16_t messageId, int16_t message)
{
	sigmem = new byte[SIGNAL_LENGTH];
	sigmem[0] = message >> 8;
	sigmem[1] = message;
	sigmem[2] = messageId >> 8;
	sigmem[3] = messageId;
	this->createdAt = millis();
}

/**
*
*
*
*/
SRL::Signal::Signal(byte* buffer)
{
	this->sigmem = buffer;
}

/**
*
*
*
*/
SRL::Signal::Signal(Signal* sig)
{
	sigmem = new byte[SIGNAL_LENGTH];
	this->createdAt = sig->getCreatedAt();
	setMessageId(sig->getMessageId());
	setMessage(sig->getMessage());
}

/**
*
*
*
*/
SRL::Signal::~Signal(void)
{
	delete[] sigmem;
}

/**
*
*
*
*/
int16_t SRL::Signal::getMessageId(void)
{
	int16_t ret = sigmem[2];
	ret = ret << 8;
	ret = ret | sigmem[3];
	return ret;
}

/**
*
*
*
*/
int16_t SRL::Signal::getMessage(void)
{
	int16_t ret = sigmem[0];
	ret = ret << 8;
	ret = ret | sigmem[1];
	return ret;
}

/**
*
*
*
*/
unsigned long SRL::Signal::getCreatedAt(void)
{
	return this->createdAt;
}

/**
*
*
*
*/
void SRL::Signal::setMessageId(int16_t messageId)
{
	sigmem[2] = messageId >> 8;
	sigmem[3] = messageId;
}

/**
*
*
*
*/
void SRL::Signal::setMessage(int16_t message)
{
	sigmem[0] = message >> 8;
	sigmem[1] = message;
}

/**
*
*	Returns the sum of the Signal's message.
*	@return
*
*/
int16_t SRL::Signal::getSum(void)
{
	return calculateSum(this->getMessage());
}

/**
*
*
*
*/
byte* SRL::Signal::getBytes(void)
{
	return sigmem;
}

/**
*
*
*
*/
SRL::InfoMessage::InfoMessage(String message)
{
	this->message = message;
}

/**
*
*
*
*/
SRL::InfoMessage::InfoMessage(InfoMessage* msg)
{
	this->message = msg->getMessage();
}

/**
*
*
* @return
*/
String SRL::InfoMessage::getMessage(void)
{
	return this->message;
}

/**
*
*
*
*/
int16_t SRL::InfoMessage::getSum(void)
{
	return calculateSum(this->message);
}
