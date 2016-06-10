#ifndef SETCLASS_H
#define SETCLASS_H
#include <iostream>
#include <vector>
#include "sort.h"
using namespace std;

class Set
{
	private: vector<int> container;
	public:
		 Set(){}

		 Set(vector<int> c):container(c)
		 {
			 insertionSort(container);
		 }

		 bool insert(int);//insert number into set, false, if number has been in the set, true, if number is not in set

		 Set unions(Set a);//get the union of two sets

		 Set intersections(Set a);//get the intersection of two sets

		 bool contains(Set a);//check if current set contains set a

		 bool find(int) const;//check if number is already in the set

		 void show() const
		 {
			 cout<<"[";
			 for(int i = 0; i < container.size(); i++)
				 cout<<container[i]<<" ";
			 cout<<"]"<<endl;
		 }
};
#endif
