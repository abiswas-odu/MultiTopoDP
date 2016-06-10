#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include "StickNode.h"
#include "Point.h"
using namespace std;

void StickNode::setContainer(vector<Point> c)
{
	container = c;
}

void StickNode::setType(string t)
{
	type = t;
}

void StickNode::setIndex(int i)
{
	index = i;
}

void StickNode::setDirection(int d)
{
	direction = d;
}

void StickNode::setSheetID(string i)
{
	sheetID = i;
}

double StickNode::getLength() const
{
	double totalLength = 0;

	for(int i = 0; i < container.size()-1; i++)
	{
		totalLength += container[i].dist(container[i+1]);
	}

	return totalLength;
}

double StickNode::getSidePointDistance() const
{
	double totalLength = 0;
	if(container.size() <= 1)
		return 0;
	totalLength = container[0].dist(container[container.size()-1]);

	return totalLength;
}

void StickNode::show() const
{
	cout<<type<<index<<" "<<direction<<" "<<sheetID<<" Num of points: "<<container.size()<<endl;
	for(int i = 0; i < container.size(); i++)
	{
		container[i].show();
	}
}

void StickNode::pdb() const
{
	stringstream stream;

	stream<<"./other/Stick"<<type<<index<<".pdb";

	ofstream outputFile;
	outputFile.open(stream.str().c_str(), ios::out|ios::app);

	if(!outputFile)
	{
		cerr<<"Can not create "<<stream.str()<<endl;
		exit(1);
	}

	for(int i = 0; i < container.size(); i++)
	{
		container[i].save(outputFile, i);
	}

	outputFile.close();
}
