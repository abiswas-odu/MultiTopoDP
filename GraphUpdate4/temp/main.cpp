#include <iostream>
#include "Set.h"
using namespace std;

int main(int argc, char *argv[])
{
	Set set1, set2;

	set1.insert(10);
	set1.insert(8);
	set1.insert(0);
	set1.insert(13);

	cout<<"Set 1:"<<endl;
	set1.show();

	set2.insert(8);
	set2.insert(10);
	set2.insert(15);

	cout<<"Set 2:"<<endl;
	set2.show();

	Set set;
	set = set1.unions(set2);

	cout<<"Union of set 1 and set 2: "<<endl;
	set.show();
	
	set = set1.intersections(set2);

	cout<<"Intersection of set 1 and set 2:"<<endl;
	set.show();

	return 0;
}
