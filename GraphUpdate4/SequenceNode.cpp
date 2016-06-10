#include <iostream>
#include <iomanip>
#include <string>
#include "SequenceNode.h"
using namespace std;

void SequenceNode::setContainer(vector<pair<int, int> > c)
{
	container = c;
}

void SequenceNode::add(pair<int, int> p)
{
	container.push_back(p);
}

void SequenceNode::setType(string t)
{
	type = t;
}

void SequenceNode::setIndex(int i)
{
	index = i;
}

void SequenceNode::show() const
{
	cout<<type<<index<<endl;
	for(int i = 0; i < container.size(); i++)
		cout<<setw(10)<<""<<"("<<container[i].first<<", "<<container[i].second<<")"<<endl;
}
