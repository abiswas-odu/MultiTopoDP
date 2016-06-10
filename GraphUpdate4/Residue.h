#ifndef RESIDUE_H
#define RESIDUE_H
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "Atom.h"
#include "Vectors.h"
//#include "Rotate.h"
using namespace std;

class Residue
{
	private: string name;//define the residue name
		 vector<Atom> atomContainer;//define the container to store atoms
		 int index;//define the index in the protein chain
	public:
		 //Constructor
		 Residue()
		 {
		 }

		 //mutator
		 void read(vector<string> stringContainer);

		 //check the distance between carbon alpha
		 double dResidue(Residue residue);

		 //set the residue name
		 void setName(string n);

		 //set the index
		 void setIndex(int i);

		 //accessor
		 //get the atom contianer
		 vector<Atom> getAtomContainer(){return atomContainer;}

		 //get the residue name
		 string getName(){return name;}

		 //get the atom number in the residue
		 int atomNumber(string str);

		 //get the index
		 int getIndex(){return index;}

		 //show the residue on the screen
		 //save the residue into file
		 void saveResidue(ofstream &file);

		 //check if the atom exist in the residue
		 bool atomExist(string atomTemp);

		 //check the atom number in the block
		 int atomExistNumber(string atomTemp);

		 //get atom by the atom name
		 Atom stringToAtom(string atomTemp);

		 //check if the four atoms are exist
		 bool chiExist(string str1, string str2, string str3, string str4);

		 //calculate the chi angle
		 double chiAngle(string str1, string str2, string str3, string str4);

                 Residue operator= (const Residue &r);

                 //get atom
                 Atom getAtom(int i);

		 //get the chi1
		 double getChi1();

		 //get the Chi2
		 double getChi2();

		 //get the Chi3
		 double getChi3();

		 //get the Chi4
		 double getChi4();

		 int getSize()
		 {
			 return atomContainer.size();
		 }

		 string getChain()
		 {
			 return atomContainer[0].getChain();
		 }
};
#endif

