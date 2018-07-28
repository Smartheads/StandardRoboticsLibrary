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
#ifndef _VECTOR_h
#define _VECTOR_h

#include <SRL.h>
#include <Angle.h>
#include <math.h>

namespace SRL
{
	class Vector
	{
		public:
			Vector(double x, double y);
			Vector(SRL::Angle direction, double length);

			static Vector addition(Vector a, Vector b);
			static Vector addition(int argc, Vector* argv);

			Vector add(Vector b);

			/* Getters & setters */
			double getX(void);
			double getY(void);
			double getLength(void);
			SRL::Angle getAngle(void);
			void setX(double x);
			void setY(double y);

		private:
			double x;
			double y;
			double length;

			void updateLength();
	};
}
#endif

