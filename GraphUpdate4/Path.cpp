#include <vector>
#include "Record.h"
#include "Path.h"
#include "Set.h"
using namespace std;

void Path::set(vector<pair<int, int> > t, Record r, pair<int, int> rn, pair<int, int> cn, vector<pair<int, int> > h, double w)
{
	trace = t;
	record = r;
	RNode = rn;
	CNode = cn;
	HContainer = h;
	weight = w;
}

void Path::setTrace(vector<pair<int, int> > t)
{
	trace = t;
}

void Path::setRecord(Record r)
{
	record = r;
}

void Path::setRNode(pair<int, int> rn)
{
	RNode = rn;
}

void Path::setCNode(pair<int, int> cn)
{
	CNode = cn;
}

void Path::setH(vector<pair<int, int> > h)
{
	HContainer = h;
}

void Path::setWeight(double w)
{
	weight = w;
}

void Path::addTrace(pair<int, int> t)
{
	trace.push_back(t);
}

void Path::addHeadEdge(pair<int, int> h)
{
	HContainer.push_back(h);
}

void Path::clearHeadEdge()
{
	HContainer.clear();
}

bool Path::operator<(const Path &right)
{
	if(weight < right.weight)
		return true;

	return false;
}

void Path::show() const
{
	//show trace
	cout<<"{";
	for(int i = 0; i < trace.size(); i++)
	{
		cout<<"["<<trace[i].first<<","<<trace[i].second<<"] ";
	}
	cout<<"}";

	//show record
	string setContainer;
	//vector<int> setContainer;
	//setContainer = record.getPath().getContainer();
	setContainer = record.getPath().dec2bin();

	cout<<"| {";
	/*for(int i = 0; i < setContainer.size(); i++)
	{
		cout<<setContainer[i]<<" ";
	}*/
	cout<<setContainer;
	cout<<"} "<<record.getF()<<" ["<<record.getPrev().first<<","<<record.getPrev().second<<"] }";

	//show RNode
	cout<<" | ["<<RNode.first<<","<<RNode.second<<"]";

	//show CNode
	cout<<" | ["<<CNode.first<<","<<CNode.second<<"]";

	//show H
	cout<<" | ";
	for(int i = 0; i < HContainer.size(); i++)
	{
		cout<<"["<<HContainer[i].first<<","<<HContainer[i].second<<"] ";
	}

	//show Weight
	cout<<" | "<<weight<<endl;
}
