#ifndef GRAPHNODE_H
#define GRAPHNODE_H
#include <vector>
#include <string>
#include <sstream>
#include "Record.h"
#include "SequenceNode.h"
#include "StickNode.h"

class GraphNode
{
	private: int row, column;//save node position in graph, keep these values to check the seuqnce informaiton and sticks information
		 int rowIndex, columnIndex;//save the index in sequence at specific type and index in sticks with specific type
		 string type;//H, helix, S, sheet, N, type not match and no node at all
		 vector<pair<int, int> > tracePair;//save the trace index pair, in which each trace includes the point index in the cluster center point container, single or two traces builds one path between node, the number of the pair matches the outer node number
		 vector<pair<int, int> > inner, outer;//save the inner nodes and outer nodes
		 vector<double> weightIn, weightOut;
		 vector<Record> container;//including path, f value and previous node
	public:
		 GraphNode()
		 {
		 	row = -1;
			column = -1;
			rowIndex = -1;
			columnIndex = -1;
		 }

		 GraphNode(int r, int c):row(r), column(c){}

		 void set(const vector<SequenceNode> &, const vector<StickNode> &, int, int);
		 void setInner(vector<pair<int, int> >);
		 void setOuter(vector<pair<int, int> >);
		 void pushTracePair(pair<int, int>);
		 void setWeightIn(vector<double>);
		 void setWeightIn(pair<int, int> inNode, double w);//udpate weight for which edge is from inNode
		 void setWeightOut(vector<double>);
		 void setWeightOut(int i, double w);//update weight for ith out node

		 void addInner(pair<int, int>);
		 void addOuter(pair<int, int>);
		 void addWeightIn(double);
		 void addWeightOut(double);
		 void addFirstRecord(const Record);
		 void updateRecord(int, const Record);
		 void recordSort();

		 int getRow() const {return row;}
		 int getColumn() const {return column;}
		 int getRowIndex() const {return rowIndex;}
		 int getColumnIndex() const {return columnIndex;}
		 string getType() const {return type;}
		 pair<int, int> getNode() const
		 {
			 pair<int, int> temp;
			 
			 temp.first = row;
			 temp.second = column;

			 return temp;
		 }
		 vector<pair<int, int> > getInner() const {return inner;}//get inners
		 int getInnerNum() const {return inner.size();}//get inner number
		 vector<double> getInWeight() const {return weightIn;}//get inner edge weight
		 vector<pair<int, int> > getOuter() const {return outer;}//get outer
		 vector<pair<int, int> > getTracePair() const {return tracePair;}//get trace pair
		 pair<int, int> getTracePair(int i) const {return tracePair[i];}//get single pair
		 int getOuterNum() const {return outer.size();}//get outer number
		 vector<double> getOutWeight() const {return weightOut;}//get outer weight
		 vector<Record> getContainer() const {return container;}//get record table
		 int getRecordNum() const {return container.size();}//get record number
		 void repeatPath() const;
		 int getStickID() const {return column/2;}//get stick index
		 string getID()//get column ID
		 {
			 stringstream stream;
			 stream<<type<<columnIndex;

			 return stream.str();
		 }
		 void showNode() const;
		 void show() const;
		 void showTopoDP() const;
};
#endif
