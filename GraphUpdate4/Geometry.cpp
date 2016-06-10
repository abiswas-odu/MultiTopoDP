#include <iostream>
#include "Geometry.h"
#include "Vectors.h"
#include "Atom.h"
using namespace std;

double getTorsionAngle(Atom a, Atom b, Atom c, Atom d)
{
	double A1, A2, B1, B2, C1, C2;
	double angle;
	int sign = 1;

	A1 = a.getY()*(b.getZ()-c.getZ())+b.getY()*(c.getZ()-a.getZ())+c.getY()*(a.getZ()-b.getZ());
	B1 = a.getZ()*(b.getX()-c.getX())+b.getZ()*(c.getX()-a.getX())+c.getZ()*(a.getX()-b.getX());
	C1 = a.getX()*(b.getY()-c.getY())+b.getX()*(c.getY()-a.getY())+c.getX()*(a.getY()-b.getY());
			 
	A2 = b.getY()*(c.getZ()-d.getZ())+c.getY()*(d.getZ()-b.getZ())+d.getY()*(b.getZ()-c.getZ());
	B2 = b.getZ()*(c.getX()-d.getX())+c.getZ()*(d.getX()-b.getX())+d.getZ()*(b.getX()-c.getX());
	C2 = b.getX()*(c.getY()-d.getY())+c.getX()*(d.getY()-b.getY())+d.getX()*(b.getY()-c.getY());
	angle = (A1*A2+B1*B2+C1*C2)/(sqrt(A1*A1+B1*B1+C1*C1)*sqrt(A2*A2+B2*B2+C2*C2));
	if((angle>0.9999999999999) && (angle < 1.0000000000001))
		return 0;
	Vectors v1(a, b), v2(c,b), v4(d,c), vNormal;
	vNormal = v1.cross(v2);

	if(v4.dot(vNormal) > 0)
		sign = -1;

	if((angle > -1.0000000001) && (angle < -0.999999999999))
		return double(sign*180);

	if((angle < -1) || (angle > 1))
		cout<<"angle calculation error!"<<endl;

	return sign*acos(angle)*180/3.1415926;
}
