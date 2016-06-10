#ifndef SET_H
#define SET_H
#include <iostream>
#include <vector>
#include "sort.h"
using namespace std;

class Set
{
	private: int value;
	public:
		 Set(){}

		 //initialize set with the stick index
		 Set(int v)
		 {
			 value = stickIndex2Bit(v);
		 }

		 //initialize set with the stick index container
		 Set(vector<int> c)
		 {
			 value = 0;
			 for(int i = 0; i < c.size(); i++)
				 value += stickIndex2Bit(c[i]);
		 }

		 int stickIndex2Bit(int);//convert stick index to bit, 3 -> 1000 -> 8
		 void insert(int);//insert stick index into set

		 void set(vector<int>);//set set by an stick index container
		 void setValue(int v);

		 Set unions(Set a);//combine two sets

		 bool contains(Set a);//check if current set contains part of set a

		 bool equal(Set) const;//check if two sets are equal
		 void remove(int);//remove the stick index

		 int getValue() const {return value;}//get value
		 int getCardinality() const;//get the bit number that was set
		 string dec2bin() const;//convert value to bit string

		 void show() const
		 {
			 cout<<"["<<dec2bin()<<"]"<<endl;
		 }
};
#endif
