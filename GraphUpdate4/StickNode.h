#ifndef STICKNODE_H
#define STICKNODE_H
#include <string>
#include <vector>
#include "Point.h"
using namespace std;

class StickNode
{
	private: vector<Point> container;//stick points for each SSE
		 string type;//secondary structure type, "H" for helix and "S" for strand
		 int index;//stick node index for specific type, start from 0
		 int direction;//1, if stick points have same direction with sequence, -1, if not
		 string sheetID;//sheet ID of the stick
	public:
		 StickNode(){}

		 StickNode(vector<Point> c, string t, int i, int d, string str):container(c), type(t), index(i), direction(d), sheetID(str){}

		 void setContainer(vector<Point>);
		 void setType(string);
		 void setIndex(int);
		 void setDirection(int);
		 void setSheetID(string);

		 vector<Point> getContainer() const {return container;}
		 Point getStart() const {return container[0];}
		 Point getEnd() const {return container[container.size()-1];}
		 string getType() const {return type;}
		 int getIndex() const {return index;}
		 int getDirection() const {return direction;}
		 string getSheetID() const {return sheetID;}
		 double getLength() const;
		 double getSidePointDistance() const;//get the distance between two side points

		 void show() const;
		 void pdb() const;
};
#endif
