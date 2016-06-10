#ifndef SEQUENCENODE_H
#define SEQUENCENODE_H
#include <vector>
#include <string>
using namespace std;

class SequenceNode
{
	private: vector<pair<int, int> > container;//start index and end index, may have several pairs which represent the sequence prediction error
		 string type;//secondary structure type, "H" for helix and "S" for strand
		 int index;//node index for specific type, start from 0
	public:
		 SequenceNode(){}

		 SequenceNode(vector<pair<int, int> > c, string t, int i):container(c), type(t), index(i){}

		 void setContainer(vector<pair<int, int> > c);
		 void add(pair<int, int> p);
		 void setType(string);
		 void setIndex(int);

		 int getStart(int i) const {return container[i].first;}
		 int getStart() const {return container[0].first;}
		 int getEnd(int i) const {return container[i].second;}
		 int getEnd() const {return container[0].second;}
		 string getType() const {return type;}
		 int getIndex() const {return index;}
		 int getLength() const {return container[0].second-container[0].first;}
		 double getLengthAngstrom() const {
			 if(type == "H")
				 return (container[0].second-container[0].first)*1.5;
			 if(type == "S")
				 return (container[0].second-container[0].first)*3.5;
			 return 0;
		 }

		 void show() const;
};
#endif
