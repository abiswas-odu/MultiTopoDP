#ifndef RECORD_H
#define RECORD_H
#include "Set.h"

class Record
{
	private: Set path;
		 double f;
		 pair<int, int> prev;
	public:
		 Record(){}

		 //set path
		 void setPath(Set);

		 //set f value
		 void setF(double);

		 //set previous node
		 void setPrev(pair<int, int>);

		 //overload operator <
		 bool operator<(const Record &r);

		 Set getPath() const {return path;}
		 double getF() const {return f;}
		 pair<int, int> getPrev() const {return prev;}

		 //void save(ostream &file);
		 void show();
};
#endif
