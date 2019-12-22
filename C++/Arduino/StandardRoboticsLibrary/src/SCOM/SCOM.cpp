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
#include <SCOM.h>

/**
*	Sends an int16_t over on serial.
*
*	@param num Int16_t to send.
*/
void SRL::sendInt16(int16_t num)
{
	byte* o = new byte[2];
	o[0] = num >> 8;
	o[1] = num;
	Serial.write(o, 2);
	Serial.flush();
	delete[] o;
}

/**
*	Reads an int16_t from the serial buffer.
*
*
*/
int16_t SRL::readInt16(void)
{
	int len = Serial.available();
	byte* k = new byte[len];
    Serial.readBytes(k, len);
    int16_t out = k[len-2] << 8 | k[len-1];
    delete[] k;
    
    return out;
}

/**
*	Returns the ASCII sum of a string.
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
      sum += (int) c[i];
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
	return calculateSum(String(signal));
}
