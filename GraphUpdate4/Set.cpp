#include <vector>
#include <string>
#include <cstdlib>
#include <bitset>
#include "Set.h"
#include "sort.h"
using namespace std;

void Set::insert(int num)
{
	value += stickIndex2Bit(num);
}

int Set::stickIndex2Bit(int s)
{
	int n;
	n = 1;
	n <<= s;

	return n;
}

void Set::set(vector<int> c)
{
	value = 0;
	for(int i = 0; i < c.size(); i++)
	{
		value += stickIndex2Bit(c[i]);
	}
}

void Set::setValue(int v)
{
	value = v;
}

Set Set::unions(Set a)
{
	Set temp;
	temp.setValue(value+a.value);
	return temp;
}

//check if current set includes part of other set
bool Set::contains(Set a)
{
	return value&(a.value);
}

bool Set::equal(Set s) const
{
	if(value == s.value)
		return true;
	return false;
}

void Set::remove(int stickIndex)
{
	value -= stickIndex2Bit(stickIndex);
	if(value < 0)
	{
		cerr<<"Remove error in the table ..."<<endl;
		exit(1);
	}
}

int Set::getCardinality() const
{
	bitset<64> temp(value);
	return temp.count();
}

string Set::dec2bin() const
{
	string res;
	unsigned long n;

	n = value;

	while(n)
	{
		res.push_back((n&1)+'0');
		n >>= 1;
	}

	if(res.empty())
		res = "0";
	else
	{
		string temp;
		for(int i = res.size()-1; i >= 0; i--)
			temp.push_back(res[i]);
		return temp;
	}

	return res;
}
