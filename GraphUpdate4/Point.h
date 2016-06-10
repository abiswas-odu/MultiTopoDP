#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
using namespace std;

class Point
{
	private: double x, y, z;//represent the coordinates of the atom
	public:		 
		 //Constructor
		 Point():x(0),y(0),z(0){}

		 Point(double xT, double yT, double zT):x(xT), y(yT), z(zT){}

		 //save point to output file
		 void save(ostream &file) const;//outputfile

		 void save(ostream &file, int) const;//output file, residue index

		 void save(ostream &file, int, int) const;//output file, atom index, residue index

		 void save(ostream &file, int, int, string) const;//output file, atom index, residue index, residue name

		 void save(ostream &file, int, int, string, string) const;//output file, atom index, residue index, residue name, atom name

		 void set(double xTemp, double yTemp, double zTemp);//set up the coordinate for point

		 void setX(double xTemp);

		 void setY(double yTemp);

		 void setZ(double zTemp);

		 //check the distance between two points
		 double dist(Point) const;

		 //get the x value
		 double getX() const {return x;}
		 
		 //get the y value
		 double getY() const {return y;}

		 //get the z value
		 double getZ() const {return z;}

		 //display point
		 void show() const
		 {
			 cout<<setw(10)<<" "<<setw(10)<<x<<setw(10)<<y<<setw(10)<<z<<endl;
		 }

		 //Deconstructor
		 virtual ~Point(){}
};
#endif
