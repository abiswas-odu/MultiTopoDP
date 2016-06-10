#include <iostream>
#include <cstdlib>
#include "Point.h"
using namespace std;

void extendLine(Point a, Point b, Point &aTemp, Point &bTemp, double l)
{
	double ratio;
	ratio = l/a.dist(b);

	cout<<"Point distance: "<<a.dist(b)<<endl;
	cout<<"Length: "<<l<<endl;
	cout<<"Ratio: "<<ratio<<endl;

	ofstream originalFile("./other/original.pdb");
	a.save(originalFile, 0);
	b.save(originalFile, 1);
	originalFile.close();

	Point c;
	c.set((a.getX()+b.getX())/2, (a.getY()+b.getY())/2, (a.getZ()+b.getZ())/2);

	ofstream centerFile("./other/center.pdb");
	c.save(centerFile);
	centerFile.close();

	bTemp.set(c.getX()+(b.getX()-c.getX())*ratio, c.getY()+(b.getY()-c.getY())*ratio, c.getZ()+(b.getZ()-c.getZ())*ratio);

	aTemp.set(c.getX()+(a.getX()-c.getX())*ratio, c.getY()+(a.getY()-c.getY())*ratio, c.getZ()+(a.getZ()-c.getZ())*ratio);
}

int main(int argc, char *argv[])
{
	system("rm -rf ./other/*");

	Point start, end;
	Point startE, endE;

	start.set(1, 1, 1);
	end.set(10, 5, 6);

	extendLine(start, end, startE, endE, 20);


	ofstream extendFile("./other/extend.pdb");
	startE.save(extendFile, 0);
	endE.save(extendFile, 1);
	extendFile.close();

	return 0;
}
