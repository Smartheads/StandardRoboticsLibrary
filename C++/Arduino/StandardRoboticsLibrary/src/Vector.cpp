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
#include "Vector.h"

SRL::Vector::Vector(double x, double y)
{
	this->x = x;
	this->y = y;
	updateLength();
}

SRL::Vector::Vector(SRL::Angle direction, double length)
{
	this->x = (length * cos(((90 - direction.getSize()) * PI) / 180));
	this->y = (length * cos((direction.getSize() * PI) / 180));
}

SRL::Vector SRL::Vector::addition(Vector a, Vector b)
{
	return Vector(a.getX() + b.getX(), a.getY() + b.getY());
}

SRL::Vector SRL::Vector::addition(int argc, Vector * argv)
{
	Vector v(0, 0);

	for (int i = 0; i < argc; i++)
	{
		v = v.add(*(argv + i));
	}

	return v;
}

SRL::Vector SRL::Vector::add(Vector b)
{
	return Vector::addition(*this, b);
}

double SRL::Vector::getX(void)
{
	return x;
}

double SRL::Vector::getY(void)
{
	return y;
}

double SRL::Vector::getLength(void)
{
	return length;
}

SRL::Angle SRL::Vector::getAngle(void)
{
	return SRL::Angle((sinh(x / length) * PI) / 180);
}

void SRL::Vector::setX(double x)
{
	this->x = x;
	updateLength();
}

void SRL::Vector::setY(double y)
{
	this->y = y;
	updateLength();
}

void SRL::Vector::updateLength()
{
	length = sqrt(x*x + y*y);
}
