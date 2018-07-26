// 
// 
// 

#include "../Angle.h"

SRL::Angle::Angle(float size)
{
	this->vollWinkel = 360;
	setAngle(size);
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
	if (size > 0)
	{
		this->size = fmod(size, vollWinkel);
	}
	else if (size < 0)
	{
		this->size = vollWinkel - (fmod(fabs(size), vollWinkel));
	}
	else
	{
		this->size = 0;
	}
}

float SRL::Angle::getSize(void)
{
	return size;
}
