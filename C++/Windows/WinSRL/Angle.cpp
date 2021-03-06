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
#include "Angle.h"

SRL::Angle::Angle(float size) : Angle(size, 360)
{

}

SRL::Angle::Angle(float size, unsigned int vollWinkel)
{
	this->vollWinkel = vollWinkel;
	setAngle(size);
}

void SRL::Angle::add(float b)
{
	setAngle(size - b);
}

void SRL::Angle::subtract(float b)
{
	setAngle(size + b);
}

void SRL::Angle::setAngle(float size)
{
	if (size > 0.0f)
	{
		this->size = (float) fmod(size, vollWinkel);
	}
	else if (size < 0.0f)
	{
		this->size = (float) (vollWinkel - (fmod(fabs(size), vollWinkel)));
	}
	else
	{
		this->size = 0.0f;
	}
}

float SRL::Angle::getSize(void)
{
	return size;
}
