#include <vector>
#include "SequenceNode.h"
#include "StickNode.h"
#include "GraphNode.h"
using namespace std;

void GraphNode::set(const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer, int r, int c)
{
	row = r;
	column = c;
	rowIndex = sequenceNodeContainer[r].getIndex();
	columnIndex = stickNodeContainer[c].getIndex();
	if(sequenceNodeContainer[r].getType() == stickNodeContainer[c].getType())
	{
		type = sequenceNodeContainer[r].getType();
		int start, end;
		start = sequenceNodeContainer[r].getStart();
		end = sequenceNodeContainer[r].getEnd();
		double sequenceLength, stickLength;
		stickLength = stickNodeContainer[c].getLength();
		//Eliminate the assignment of a short SSEs on the protein sequence to a long SSEs in the volume map or vice versa
		if(type == "H")
		{
			//cout<<"Check H"<<endl;
			sequenceLength = (end-start)*1.5;//each amino acide in helix takes 1.5 angstrom
			if(sequenceLength*0.4 >= stickLength || stickLength*0.4 >= sequenceLength)
				type = "N";
		}
		else if(type == "S")
		{
			sequenceLength = (end-start)*3.5;//each amino acid in strand takes 3.5 angstrom
			if(sequenceLength*0.4 >= stickLength || stickLength*0.4 >= sequenceLength)
				type = "N";
		}
	}
	else
		type = "N";
}

void GraphNode::setInner(vector<pair<int, int> > i)
{
	inner = i;
}

void GraphNode::setOuter(vector<pair<int, int> > o)
{
	outer = o;
}

void GraphNode::setWeightIn(vector<double> i)
{
	weightIn = i;
}

void GraphNode::setWeightIn(pair<int, int> inNode, double w)
{
	bool found = false;
	int indexTemp;

	for(int i = 0; i < inner.size(); i++)
		if(inner[i].first == inNode.first && inner[i].second == inNode.second)
		{
			found = true;
			indexTemp = i;
			break;
		}

	if(!found)
	{
		cerr<<"Can not find corresponding index during weight updating ..."<<endl;
		return;
	}

	weightIn[indexTemp] = w;
}

void GraphNode::setWeightOut(vector<double> o)
{
	weightOut = o;
}

void GraphNode::setWeightOut(int i, double w)
{
	weightOut[i] = w;
}

void GraphNode::pushTracePair(pair<int, int> p)
{
	tracePair.push_back(p);
}

void GraphNode::addInner(pair<int, int> i)
{
	inner.push_back(i);
}

void GraphNode::addOuter(pair<int, int> o)
{
	outer.push_back(o);
}

void GraphNode::addWeightIn(double i)
{
	weightIn.push_back(i);
}

void GraphNode::addWeightOut(double o)
{
	weightOut.push_back(o);
}

void GraphNode::addFirstRecord(const Record r)
{
	container.push_back(r);
}

void GraphNode::updateRecord(int recordIndex, const Record r)
{
	if(container[recordIndex].getF() > r.getF())
	{
		container[recordIndex] = r;
	}
}

//sort the record by F value
void GraphNode::recordSort()
{
	insertionSort(container);
}

void GraphNode::repeatPath() const
{
	if(container.size() < 3)
		return;

	for(int i = 0; i < container.size()-1; i++)
	{
		for(int j = i+1; j < container.size(); j++)
		{
			Set setA, setB;
			setA = container[i].getPath();
			setB = container[j].getPath();

			if(setA.equal(setB))
			{
				cout<<"Record "<<i<<" and "<<j<<" have repeat path ..."<<endl;
			}
		}
	}
}

void GraphNode::showNode() const
{
	cout<<"["<<row<<", "<<column<<"] ";
}

void GraphNode::show() const
{
	cout<<"["<<type<<rowIndex<<", "<<type<<columnIndex<<", ";
	if(column%2 == 0)
		cout<<1<<"] ";
	else
		cout<<-1<<"] ";
}

void GraphNode::showTopoDP() const
{
	cout<<"["<<row+1<<", "<<column<<"] ";
}
