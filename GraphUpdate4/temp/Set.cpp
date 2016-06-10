#include "Set.h"
#include "sort.h"
using namespace std;

bool Set::insert(int num)
{
	//return flase, if number is already in the set
	if(find(num))
		return false;

	container.push_back(num);//push number into set

	insertionSort(container);//sort numbers in the set

	return true;
}

Set Set::unions(Set a)
{
	Set temp;

	for(int i = 0; i < container.size(); i++)
		temp.insert(container[i]);

	for(int i = 0; i < a.container.size(); i++)
		temp.insert(a.container[i]);

	return temp;
}

Set Set::intersections(Set a)
{
	Set temp;

	for(int i = 0; i < a.container.size(); i++)
	{
		if(find(a.container[i]))
			temp.insert(a.container[i]);
	}

	return temp;
}

bool Set::contains(Set a)
{
	for(int i = 0; i < a.container.size(); i++)
	{
		if(!find(a.container[i]))
			return false;
	}

	return true;
}

bool Set::find(int num) const
{
	for(int i = 0; i < container.size(); i++)
	{
		if(num == container[i])
			return true;
	}

	return false;
}
