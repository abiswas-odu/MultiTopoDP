#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "Geometry.h"
#include "Residue.h"
#include "Atom.h"
using namespace std;

void Residue::read(vector<string> stringContainer)
{
	atomContainer.clear();
	for(int i = 0; i < stringContainer.size(); i++)
	{
		Atom atomTemp;
		atomTemp.read(stringContainer[i]);
		atomContainer.push_back(atomTemp);
	}

	name = atomContainer[0].getResidueName();
	index = atomContainer[0].getResidueIndex();

}

double Residue::dResidue(Residue residue)
{
	double xTemp, yTemp, zTemp;

	xTemp = atomContainer[1].getX() - residue.atomContainer[1].getX();
	yTemp = atomContainer[1].getY() - residue.atomContainer[1].getY();
	zTemp = atomContainer[1].getZ() - residue.atomContainer[1].getZ();

	return sqrt(xTemp*xTemp+yTemp*yTemp+zTemp*zTemp);
}

void Residue::setName(string n)
{
	name = n;
}

void Residue::setIndex(int i)
{
	index = i;
}

int Residue::atomNumber(string str)
{
	if(str == "GLY")
		return 4;
	else if(str == "ALA")
		return 5;
	else if(str == "VAL")
		return 7;
	else if(str == "ILE")
		return 8;
	else if(str == "LEU")
		return 8;
	else if(str == "SER")
		return 6;
	else if(str == "THR")
		return 7;
	else if(str == "ASP")
		return 8;
	else if(str == "ASN")
		return 8;
	else if(str == "GLU")
		return 9;
	else if(str == "GLN")
		return 9;
	else if(str == "LYS")
		return 9;
	else if(str == "ARG")
		return 11;
	else if(str == "CYS")
		return 6;
	else if(str == "MET")
		return 8;
	else if(str == "PHE")
		return 11;
	else if(str == "TYR")
		return 12;
	else if(str == "TRP")
		return 14;
	else if(str == "HIS")
		return 10;
	else if(str == "PRO")
		return 7;
	return 0;
}

void Residue::saveResidue(ofstream &file)
{
	for(int i = 0; i < atomContainer.size(); i++)
		atomContainer[i].saveAtom(file);
}

bool Residue::atomExist(string atomTemp)
{
	if(atomTemp.size() != 4)
	{
		cout<<"Atom "<<atomTemp<<" size is error!"<<endl;
		exit(1);
	}
	for(int i = 0; i < atomContainer.size(); i++)
		if(atomTemp == atomContainer[i].getName())
			return true;
	return false;
}

int Residue::atomExistNumber(string atomTemp)
{
	if(atomTemp.size() != 4)
	{
		cout<<"Atom "<<atomTemp<<" size is error!"<<endl;
		exit(1);
	}
	for(int i = 0; i < atomContainer.size(); i++)
		if(atomTemp == atomContainer[i].getName())
			return i;
	return -1;
}

Atom Residue::stringToAtom(string atomTemp)
{
	if(atomTemp.size() != 4)
	{
		cout<<"Atom "<<atomTemp<<" size is error!"<<endl;
		exit(1);
	}
	for(int i = 0; i < atomContainer.size(); i++)
		if(atomTemp == atomContainer[i].getName())
			return atomContainer[i];

	Atom atom;
	return atom;
}

bool Residue::chiExist(string str1, string str2, string str3, string str4)
{
	if(!atomExist(str1))
		return false;
	if(!atomExist(str2))
		return false;
	if(!atomExist(str3))
		return false;
	if(!atomExist(str4))
		return false;
	return true;
}

double Residue::chiAngle(string str1, string str2, string str3, string str4)
{
	int a1,a2,a3,a4;//represent the atom index
	a1 = atomExistNumber(str1);
	a2 = atomExistNumber(str2);
	a3 = atomExistNumber(str3);
	a4 = atomExistNumber(str4);

	return getTorsionAngle(atomContainer[a1], atomContainer[a2], atomContainer[a3], atomContainer[a4]);
}

Residue Residue::operator= (const Residue &r)
{
	name = r.name;
	atomContainer = r.atomContainer;
	index = r.index;
	return *this;
}

Atom Residue::getAtom(int i)
{
	Atom temp;

	temp = atomContainer[i];

	return temp;
}

double Residue::getChi1()
{
	if(name == "GLY")
		return 999.0;
	else if(name == "ALA")
		return 999.0;
	else if(name == "VAL")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG1"))
			return chiAngle(" N  ", " CA ", " CB ", " CG1");
	}
	else if(name == "ILE")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG1"))
			return chiAngle(" N  ", " CA ", " CB ", " CG1");
	}
	else if(name == "LEU")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	else if(name == "SER")
	{
		if(chiExist(" N  ", " CA ", " CB ", " OG "))
			return chiAngle(" N  ", " CA ", " CB ", " OG ");
	}
	else if(name == "THR")
	{
		if(chiExist(" N  ", " CA ", " CB ", " OG1"))
			return chiAngle(" N  ", " CA ", " CB ", " OG1");
	}
	else if(name == "ASP")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	else if(name == "ASN")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	else if(name == "GLU")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	else if(name == "GLN")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	else if(name == "LYS")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	else if(name == "ARG")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	else if(name == "CYS")
	{
		if(chiExist(" N  ", " CA ", " CB ", " SG "))
			return chiAngle(" N  ", " CA ", " CB ", " SG ");
	}
	else if(name == "MET")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	else if(name == "PHE")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	else if(name == "TYR")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	else if(name == "TRP")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	else if(name == "HIS")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	else if(name == "PRO")
	{
		if(chiExist(" N  ", " CA ", " CB ", " CG "))
			return chiAngle(" N  ", " CA ", " CB ", " CG ");
	}
	return 999.0;
}

double Residue::getChi2()
{
	if(name == "GLY")
		return 999.0;
	else if(name == "ALA")
		return 999.0;
	else if(name == "VAL")
		return 999.0;
	else if(name == "ILE")
	{
		if(chiExist(" CA ", " CB ", " CG1", " CD1"))
			return chiAngle(" CA ", " CB ", " CG1", " CD1");
	}
	else if(name == "LEU")
	{
		if(chiExist(" CA ", " CB ", " CG ", " CD1"))
			return chiAngle(" CA ", " CB ", " CG ", " CD1");
	}
	else if(name == "SER")
		return 999.0;
	else if(name == "THR")
		return 999.0;
	else if(name == "ASP")
	{
		if(chiExist(" CA ", " CB ", " CG ", " OD2"))
			return chiAngle(" CA ", " CB ", " CG ", " OD2");
	}
	else if(name == "ASN")
	{
		if(chiExist(" CA ", " CB ", " CG ", " ND2"))
			return chiAngle(" CA ", " CB ", " CG ", " ND2");
	}
	else if(name == "GLU")
	{
		if(chiExist(" CA ", " CB ", " CG ", " CD "))
			return chiAngle(" CA ", " CB ", " CG ", " CD ");
	}
	else if(name == "GLN")
	{
		if(chiExist(" CA ", " CB ", " CG ", " CD "))
			return chiAngle(" CA ", " CB ", " CG ", " CD ");
	}
	else if(name == "LYS")
	{
		if(chiExist(" CA ", " CB ", " CG ", " CD "))
			return chiAngle(" CA ", " CB ", " CG ", " CD ");
	}
	else if(name == "ARG")
	{
		if(chiExist(" CA ", " CB ", " CG ", " CD "))
			return chiAngle(" CA ", " CB ", " CG ", " CD ");
	}
	else if(name == "CYS")
		return 999.0;
	else if(name == "MET")
	{
		if(chiExist(" CA ", " CB ", " CG ", " SD "))
			return chiAngle(" CA ", " CB ", " CG ", " SD ");
	}
	else if(name == "PHE")
	{
		if(chiExist(" CA ", " CB ", " CG ", " CD1"))
			return chiAngle(" CA ", " CB ", " CG ", " CD1");
	}
	else if(name == "TYR")
	{
		if(chiExist(" CA ", " CB ", " CG ", " CD1"))
			return chiAngle(" CA ", " CB ", " CG ", " CD1");
	}
	else if(name == "TRP")
	{
		if(chiExist(" CA ", " CB ", " CG ", " CD2"))
			return chiAngle(" CA ", " CB ", " CG ", " CD2");
	}
	else if(name == "HIS")
	{
		if(chiExist(" CA ", " CB ", " CG ", " ND1"))
			return chiAngle(" CA ", " CB ", " CG ", " ND1");
	}
	else if(name == "PRO")
		return 999.0;
	return 999.0;
}

double Residue::getChi3()
{
	if(name == "GLY")
		return 999.0;
	else if(name == "ALA")
		return 999.0;
	else if(name == "VAL")
		return 999.0;
	else if(name == "ILE")
		return 999.0;
	else if(name == "LEU")
		return 999.0;
	else if(name == "SER")
		return 999.0;
	else if(name == "THR")
		return 999.0;
	else if(name == "ASP")
		return 999.0;
	else if(name == "ASN")
		return 999.0;
	else if(name == "GLU")
	{
		if(chiExist(" CB ", " CG ", " CD ", " OE2"))
			return chiAngle(" CB ", " CG ", " CD ", " OE2");
	}
	else if(name == "GLN")
	{
		if(chiExist(" CB ", " CG ", " CD ", " NE2"))
			return chiAngle(" CB ", " CG ", " CD ", " NE2");
	}
	else if(name == "LYS")
	{
		if(chiExist(" CB ", " CG ", " CD ", " CE "))
			return chiAngle(" CB ", " CG ", " CD ", " CE ");
	}
	else if(name == "ARG")
	{
		if(chiExist(" CB ", " CG ", " CD ", " NE "))
			return chiAngle(" CB ", " CG ", " CD ", " NE ");
	}
	else if(name == "CYS")
		return 999.0;
	else if(name == "MET")
	{
		if(chiExist(" CB ", " CG ", " SD ", " CE "))
			return chiAngle(" CB ", " CG ", " SD ", " CE ");
	}
	else if(name == "PHE")
		return 999.0;
	else if(name == "TYR")
		return 999.0;
	else if(name == "TRP")
		return 999.0;
	else if(name == "HIS")
		return 999.0;
	else if(name == "PRO")
		return 999.0;
	return 999.0;
}

double Residue::getChi4()
{
	if(name == "LYS")
	{
		if(chiExist(" CG ", " CD ", " CE ", " NZ "))
			return chiAngle(" CG ", " CD ", " CE ", " NZ ");
	}
	else if(name == "ARG")
	{
		if(chiExist(" CG ", " CD ", " NE ", " CZ "))
			return chiAngle(" CG ", " CD ", " NE ", " CZ ");
	}
	return 999.0;
}

