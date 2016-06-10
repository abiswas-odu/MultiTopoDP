#ifndef PROTEIN_H
#define PROTEIN_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "Residue.h"
using namespace std;

class Protein
{
	private: string name;//define protein name
		 vector<Residue> residueContainer;//define residue container
	public:
		 //Constructor
		 Protein()
		 {
		 }

		 void setName(string n)
		 {
			 name = n;
		 }

		 //accessor
		 //save the protein into file
		 void saveProtein(string str)
		 {
			 ofstream outputFile;
			 outputFile.open(str.c_str());

			 for(int i = 0; i < residueContainer.size(); i++)
				 residueContainer[i].saveResidue(outputFile);
			 outputFile.close();
		 }

		 string getName() const {return name;}

		 vector<Residue> getResidueContainer() const {return residueContainer;}

		 ~Protein()
		 {
		 }
};
#endif
