// Angle.h

#ifndef _ANGLE_h
#define _ANGLE_h

#include <SRL.h>
#include <math.h>

namespace SRL
{
	class Angle
	{
		public:
			Angle(float size = 0);
			Angle(float size, unsigned int vollWinkel);

			void add(float b);
			void subtract(float b);

			void setAngle(float size);

			float getSize(void);
		private:
			float size;
			unsigned int vollWinkel;
	};
}
#endif

