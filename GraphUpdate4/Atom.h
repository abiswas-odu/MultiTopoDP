/*Descript the property of the atom*/
#ifndef ATOM_H
#define ATOM_H
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "Point.h"
using namespace std;

class Atom : public Point
{
	private: int atomIndex;//atom index in the protein
		 string name;//atom name
		 string alternate;//alternate location indicator
		 string chain;//chain name
		 string residueName;//residue including the atom
		 int residueIndex;//residue index
	public:		 
		 //Constructor
		 Atom():Point()
		 {
			 atomIndex = 0;
			 name = "CA";//set the name of the atom to be "none"
			 alternate = " ";
			 chain = "A";
			 residueName = "GLY";
			 residueIndex = 0;
		 }

		 Atom(string n, double xt, double yt, double zt):Point(xt, yt, zt)
		 {
			 name = n;//set the name
			 atomIndex = 0;
			 alternate = " ";
			 chain = "A";
			 residueName = "GLY";
			 residueIndex = 0;
		 }

		 Atom(double xt, double yt, double zt):Point(xt, yt, zt)
		 {
			 name = "CA";
			 atomIndex = 0;
			 alternate = " ";
			 chain = "A";
			 residueName = "GLY";
			 residueIndex = 0;
		 }

		 //Mutator
		 //read the name, coordinates, charge, sigma and epsilong from the file
		 void read(string lineStr);

		 //shift atom from atom a to b
		 void shift(Atom a, Atom b);

		 void shiftToOrigin(Atom a);

		 void shiftBack(Atom a);

		 Atom operator=(const Atom &right);

		 Atom operator-(const Atom &right);

		 Atom operator+(const Atom &right);

		 //set the name
		 void setName(string n);

		 //set the residue name
		 void setResidueName(string n);

		 //set the alternate location indicator
		 void setAlternate(string s);

		 //set the chain name
		 void setChain(string c);

		 //set the atom index
		 void setAtomIndex(int i);

		 //set the residue index
		 void setResidueIndex(int i);

		 //check if the atom is H
		 bool hydrogen(string s);

		 //Accessor
		 //show the atom
		 void showAtom();

		 void saveAtom(ofstream &file);

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

		 //get the name of the atom
		 string getName() {return name;}

		 //get the alternate location indicator
		 string getAlternate(){return alternate;}

		 //get the chain name
		 string getChain(){return chain;}

		 //get the residue name
		 string getResidueName(){return residueName;}

		 //get the atom index
		 int getAtomIndex(){return atomIndex;}

		 //get the residue index
		 int getResidueIndex(){return residueIndex;}

		 //Deconstructor
		 ~Atom()
		 {
		 }
};
#endif
