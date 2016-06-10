#ifndef VECTORS_H
#define VECTORS_H
#include <iostream>
#include <cmath>
#include "Point.h"
using namespace std;

class Vectors
{
	private: double x, y, z;
	public:
		 //Constructor
		 Vectors()
		 {
		 }

		 Vectors(Point a, Point b);

		 Vectors(double cx, double cy, double cz):x(cx),y(cy),z(cz){}

		 Vectors(Point a);

		 //Mutator
		 //calculate dot product
		 double dot(Vectors v);

		 //calculate cross product
		 Vectors cross(Vectors v);

		 //set the vector
		 void setVector(double xt, double yt, double zt);

                 Vectors operator -(const Vectors &v);

                 Vectors operator +(const Vectors &v);

		 double getAngle(Vectors v) const;

		 double getAngleDegree(Vectors v) const;

		 double getLength() const;

                 Vectors time(double v);

		 //calculate the angle between two projections on the xy plane
		 double getAngleXY(Vectors v)
		 {
			 return acos((x*v.x + y*v.y)/(sqrt(x*x + y*y)*sqrt(v.x*v.x + v.y*v.y)));
		 }

		 //calculate the angle between two vectors
		 double getAngle(Vectors v)
		 {
			 return acos(((x*v.x + y*v.y + z*v.z)/(sqrt(x*x + y*y + z*z) * sqrt(v.x*v.x + v.y*v.y + v.z*v.z))));
		 }

		 //calculate the angle betweeen two vectors and translate into degree
		 double getAngleDegree(Vectors v)
		 {
			 return getAngle(v)*180/3.1415926;
		 }

		 //calculate the length of the vector
		 double getLength()
		 {
			 return sqrt(x*x + y*y + z*z);
		 }

		 //calculate the unit vector of the vector
		 Vectors getUnit()
		 {
			 Vectors temp;
			 temp.x = x/getLength();
			 temp.y = y/getLength();
			 temp.z = z/getLength();

			 return temp;
		 }

		 //get the value of x
		 double getX()
		 {
			 return x;
		 }

		 //get the value of y
		 double getY()
		 {
			 return y;
		 }

		 //get the value of z
		 double getZ()
		 {
			 return z;
		 }

		 //show the vector
		 void showVector()
		 {
			 cout<<"Vecotr: "<<x<<" "<<y<<" "<<z<<endl;
		 }
};
#endif
