#include "Record.h"
using namespace std;

void Record::setPath(Set p)
{
	path = p;
}

void Record::setF(double v)
{
	f = v;
}

void Record::setPrev(pair<int, int> node)
{
	prev = node;
}

bool Record::operator<(const Record &r)
{
	if((r.getF()-getF()) > 0.00001)
		return true;
	else
		return false;
}

void Record::show()
{
	path.show();
	cout<<"F value: "<<f<<" Prev: ["<<prev.first<<","<<prev.second<<"]"<<endl;
}
