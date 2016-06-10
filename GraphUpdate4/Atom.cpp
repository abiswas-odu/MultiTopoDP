#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "Atom.h"
using namespace std;

void Atom::read(string lineStr)
{
	string subStr;//define a temp string

	subStr = lineStr.substr(6,5);//extract the atom index
	atomIndex = atoi(subStr.c_str());

	name = lineStr.substr(12,4);//extract the name

	alternate = lineStr.substr(16,1);//extract the alternate location indicator
	residueName = lineStr.substr(17,3);//extract the residue name
	chain = lineStr.substr(21,1);//extract the chain name

	subStr = lineStr.substr(22,4);//extract the residue index
	residueIndex = atoi(subStr.c_str());

	double xTemp, yTemp, zTemp;

	subStr = lineStr.substr(30,8);//extract the x
	xTemp = atof(subStr.c_str());

	subStr = lineStr.substr(38,8);//extract the y
	yTemp = atof(subStr.c_str());

	subStr = lineStr.substr(46,8);//extract the z
	zTemp = atof(subStr.c_str());

	set(xTemp, yTemp, zTemp);//set up x, y and z
}

void Atom::shift(Atom a, Atom b)
{
	double xTemp, yTemp, zTemp;
	xTemp = getX() - a.getX();
	xTemp += b.getX();
	yTemp = getY() - a.getY();
	yTemp += b.getY();
	zTemp = getZ() - a.getZ();
	zTemp += b.getZ();

	set(xTemp, yTemp, zTemp);
}

void Atom::shiftToOrigin(Atom a)
{
	double xTemp, yTemp, zTemp;
	xTemp = getX() - a.getX();
	yTemp = getY() - a.getY();
	zTemp = getZ() -  a.getZ();

	set(xTemp, yTemp, zTemp);
}

void Atom::shiftBack(Atom a)
{
	double xTemp, yTemp, zTemp;
	xTemp = getX() + a.getX();
	yTemp = getY() + a.getY();
	zTemp = getZ() + a.getZ();

	set(xTemp, yTemp, zTemp);
}

Atom Atom::operator=(const Atom &right)
{
	double xTemp, yTemp, zTemp;
	xTemp = right.getX();
	yTemp = right.getY();
	zTemp = right.getZ();
	set(xTemp, yTemp, zTemp);
	atomIndex = right.atomIndex;
	name = right.name;
	alternate = right.alternate;
	chain = right.chain;
	residueName = right.residueName;
	residueIndex = right.residueIndex;
	return *this;
}

Atom Atom::operator-(const Atom &right)
{
	Atom temp;

	temp.setX(getX() - right.getX());
	temp.setY(getY() - right.getY());
	temp.setZ(getZ() - right.getZ());
	temp.atomIndex = atomIndex;
	temp.name = name;
	temp.alternate = alternate;
	temp.chain = chain;
	temp.residueName = residueName;
	temp.residueIndex = residueIndex;

	return temp;
}

Atom Atom::operator+(const Atom &right)
{
	Atom temp;

	temp.setX(getX() + right.getX());
	temp.setY(getY() + right.getY());
	temp.setZ(getZ() + right.getZ());
	temp.atomIndex = atomIndex;
	temp.name = name;
	temp.alternate = alternate;
	temp.chain = chain;
	temp.residueName = residueName;
	temp.residueIndex = residueIndex;

	return temp;
}

void Atom::setName(string n)
{
	name = n;
}

void Atom::setResidueName(string n)
{
	residueName = n;
}

void Atom::setAlternate(string s)
{
	alternate = s;
}

void Atom::setChain(string c)
{
	chain = c;
}

void Atom::setAtomIndex(int i)
{
	atomIndex = i;
}

void Atom::setResidueIndex(int i)
{
	residueIndex = i;
}

bool Atom::hydrogen(string s)
{
	string temp;
	temp = s.substr(13,1);
	if(temp == "H")
		return true;
	temp = s.substr(12,1);
	if(temp == "H")
		return true;
	return false;
}

void Atom::showAtom()
{
	cout<<setw(6)<<"ATOM  "<<setw(5)<<atomIndex<<" "<<setw(4)<<name<<setw(1)<<alternate<<setw(3)<<residueName<<setw(1)<<" "<<setw(1)<<chain<<setw(4)<<residueIndex<<setw(4)<<"    "<<setw(8)<<fixed<<setprecision(3)<<getX()<<setw(8)<<fixed<<setprecision(3)<<getY()<<setw(8)<<fixed<<setprecision(3)<<getZ()<<endl;
}

void Atom::saveAtom(ofstream &file)
{
	file<<setw(6)<<"ATOM  "<<setw(5)<<atomIndex<<" "<<setw(4)<<name<<setw(1)<<alternate<<setw(3)<<residueName<<setw(1)<<" "<<setw(1)<<chain<<setw(4)<<residueIndex<<setw(4)<<"    "<<setw(8)<<fixed<<setprecision(3)<<getX()<<setw(8)<<fixed<<setprecision(3)<<getY()<<setw(8)<<fixed<<setprecision(3)<<getZ()<<endl;
}

		 /*double dist(Atom atom)
		 {
			 double tempX, tempY, tempZ;

			 tempX = x - atom.getX();
			 tempX = tempX*tempX;
			 tempY = y - atom.getY();
			 tempY = tempY*tempY;
			 tempZ = z - atom.getZ();
			 tempZ = tempZ*tempZ;

			 return sqrt(tempX+tempY+tempZ);
		 }*/
