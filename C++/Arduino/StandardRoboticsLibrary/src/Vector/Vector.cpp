// 
// 
// 

#include <Vector.h>

SRL::Vector::Vector(double x, double y)
{
	this->x = x;
	this->y = y;
	updateLength();
}

SRL::Vector::Vector(Angle direction, double length)
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
