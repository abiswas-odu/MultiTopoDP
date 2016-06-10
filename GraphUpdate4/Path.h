#ifndef PATH_H
#define PATH_H
#include <vector>
#include "Record.h"
using namespace std;

class Path
{
	private: vector<pair<int, int> > trace;//trace from end to current node (or coincide node)
		 Record record;//corresponding record in next node
		 pair<int, int> RNode, CNode;//RNode represents next node before coincide node, CNode represents current node (or coincide node)
		 vector<pair<int, int> > HContainer;//head edge that should be removed temporarily
		 double weight;//weight for whole path
	public:
		 Path()
		 {
		 }

		 Path(vector<pair<int, int> > t, Record r, pair<int, int> rn, pair<int, int> cn, vector<pair<int, int> > h, double w):trace(t),record(r), RNode(rn), CNode(cn), HContainer(h), weight(w){}

		 //set path
		 void set(vector<pair<int, int> > t, Record r, pair<int, int> rn, pair<int, int> cn, vector<pair<int, int> > h, double w);

		 //set trace
		 void setTrace(vector<pair<int, int> > t);

		 //set record for next node
		 void setRecord(Record r);

		 //set node for next node before coincide node
		 void setRNode(pair<int, int>);

		 //set node for coincide node
		 void setCNode(pair<int, int>);

		 //set head edge container
		 void setH(vector<pair<int, int> >);

		 //set weight for path
		 void setWeight(double);

		 //add node into trace
		 void addTrace(pair<int, int>);

		 //add node into 
		 void addHeadEdge(pair<int, int>);

		 //reset head edge
		 void clearHeadEdge();

		 //overload operator "<"
		 bool operator<(const Path &);

		 vector<pair<int, int> > getTrace() const {return trace;}//return trace
		 Record getRecord() const {return record;}//return record for next node before coincide node
		 pair<int, int> getRNode() const {return RNode;}//return next node
		 pair<int, int> getCNode() const {return CNode;}//return current node (coincide node)
		 vector<pair<int, int> > getH() const {return HContainer;}//return head edge
		 double getWeight() const {return weight;}//return path weight

		 void show() const;//show path
};
#endif
