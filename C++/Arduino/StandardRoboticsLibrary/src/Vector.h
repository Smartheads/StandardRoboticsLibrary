// Vector.h

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
			Vector(Angle direction, double length);

			static Vector addition(Vector a, Vector b);
			static Vector addition(int argc, Vector* argv);

			Vector add(Vector b);

			/* Getters & setters */
			double getX(void);
			double getY(void);
			double getLength(void);
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

