#include <string>
#include <fstream>
#include <cmath>
#include "Point.h"
using namespace std;

void Point::save(ostream &file) const
{

	file<<setw(6)<<"ATOM  "<<setw(5)<<0<<" "<<setw(4)<<" CA "<<setw(1)<<" "<<setw(3)<<"HOH"<<setw(1)<<" "<<setw(1)<<"A"<<setw(4)<<0<<setw(4)<<"    "<<setw(8)<<fixed<<setprecision(3)<<x<<setw(8)<<fixed<<setprecision(3)<<y<<setw(8)<<fixed<<setprecision(3)<<z<<endl;
}

void Point::save(ostream &file, int residueIndex) const
{
	//file<<setw(6)<<"ATOM  "<<setw(5)<<atomIndex<<" "<<setw(4)<<name<<setw(1)<<alternate<<setw(3)<<residueName<<setw(1)<<" "<<setw(1)<<chain<<setw(4)<<residueIndex<<setw(4)<<"    "<<setw(8)<<fixed<<setprecision(3)<<x<<setw(8)<<fixed<<setprecision(3)<<y<<setw(8)<<fixed<<setprecision(3)<<z<<endl;

	file<<setw(6)<<"ATOM  "<<setw(5)<<0<<" "<<setw(4)<<" CA "<<setw(1)<<" "<<setw(3)<<"HOH"<<setw(1)<<" "<<setw(1)<<"A"<<setw(4)<<residueIndex<<setw(4)<<"    "<<setw(8)<<fixed<<setprecision(3)<<x<<setw(8)<<fixed<<setprecision(3)<<y<<setw(8)<<fixed<<setprecision(3)<<z<<endl;
}

void Point::save(ostream &file, int atomIndex, int residueIndex) const
{
       	file<<setw(6)<<"ATOM  "<<setw(5)<<atomIndex<<" "<<setw(4)<<" O  "<<setw(1)<<" "<<setw(3)<<"GLY"<<setw(1)<<" "<<setw(1)<<"A"<<setw(4)<<residueIndex<<setw(4)<<"    "<<setw(8)<<fixed<<setprecision(3)<<x<<setw(8)<<fixed<<setprecision(3)<<y<<setw(8)<<fixed<<setprecision(3)<<z<<endl;
}

void Point::save(ostream &file, int atomIndex, int residueIndex, string residueName) const
{
       	file<<setw(6)<<"ATOM  "<<setw(5)<<atomIndex<<" "<<setw(4)<<" O  "<<setw(1)<<" "<<setw(3)<<residueName<<setw(1)<<" "<<setw(1)<<"A"<<setw(4)<<residueIndex<<setw(4)<<"    "<<setw(8)<<fixed<<setprecision(3)<<x<<setw(8)<<fixed<<setprecision(3)<<y<<setw(8)<<fixed<<setprecision(3)<<z<<endl;
}

void Point::save(ostream &file, int atomIndex, int residueIndex, string residueName, string atomName) const
{
       	file<<setw(6)<<"ATOM  "<<setw(5)<<atomIndex<<" "<<setw(4)<<atomName<<setw(1)<<" "<<setw(3)<<residueName<<setw(1)<<" "<<setw(1)<<"A"<<setw(4)<<residueIndex<<setw(4)<<"    "<<setw(8)<<fixed<<setprecision(3)<<x<<setw(8)<<fixed<<setprecision(3)<<y<<setw(8)<<fixed<<setprecision(3)<<z<<endl;
}

void Point::set(double xTemp, double yTemp, double zTemp)
{
	x = xTemp;
	y = yTemp;
	z = zTemp;
}

void Point::setX(double xTemp)
{
	x = xTemp;
}

void Point::setY(double yTemp)
{
	y = yTemp;
}

void Point::setZ(double zTemp)
{
	z = zTemp;
}

double Point::dist(Point p) const
{
	double tempX, tempY, tempZ;

	tempX = x - p.getX();
	tempY = y - p.getY();
	tempZ = z - p.getZ();

	return sqrt(tempX*tempX+tempY*tempY+tempZ*tempZ);
}
