#include <iostream>
#include <cmath>
#include "Vectors.h"
#include "Point.h"
using namespace std;

Vectors::Vectors(Point a, Point b)
{
	x = a.getX() - b.getX();
	y = a.getY() - b.getY();
	z = a.getZ() - b.getZ();
}

Vectors::Vectors(Point a)
{
	x = a.getX();
	y = a.getY();
	z = a.getZ();
}

//calculate dot product
double Vectors::dot(Vectors v)
{
	return ((x*v.x)+(y*v.y)+(z*v.z));
}

//calculate cross product
Vectors Vectors::cross(Vectors v)
{
	Vectors temp;
	temp.x = y*v.z - z*v.y;
	temp.y = z*v.x - x*v.z;
	temp.z = x*v.y - y*v.x;
	return temp;
}

//set the vector
void Vectors::setVector(double xt, double yt, double zt)
{
	x = xt;
	y = yt;
	z = zt;
}

Vectors Vectors::operator -(const Vectors &v)
{
        Vectors temp;
        temp.x = x - v.x;
        temp.y = y - v.y;
        temp.z = z - v.z;
        return temp;
}

Vectors Vectors::operator +(const Vectors &v)
{
        Vectors temp;
        temp.x = x - v.x;
        temp.y = y - v.y;
        temp.z = z - v.z;
        return temp;
}

double Vectors::getAngle(Vectors v) const
{
	 return acos(((x*v.x + y*v.y + z*v.z)/(sqrt(x*x + y*y + z*z) * sqrt(v.x*v.x + v.y*v.y + v.z*v.z))));
}

double Vectors::getAngleDegree(Vectors v) const
{
	return getAngle(v)*180/3.1415926;
}

double Vectors::getLength() const
{
	return sqrt(x*x + y*y + z*z);
}

Vectors Vectors::time(double v)
{
        Vectors temp;
        temp.x = x*v;
        temp.y = y*v;
        temp.z = z*v;
        return temp;
}
