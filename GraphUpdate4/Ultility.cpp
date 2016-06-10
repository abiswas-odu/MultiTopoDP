#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include "Ultility.h"
#include "Vectors.h"
#include "SequenceNode.h"
#include "StickNode.h"
#include "dirFile.h"
#include "Point.h"
using namespace std;

double distanceBetweenLines(Point a, Point b, Point c, Point d)
{
	Vectors v1(a, b), v2(c, d);//define two lines
	Vectors PP(b, c);

	double distTemp;

	Vectors crossV;
	crossV = v1.cross(v2);

	double crossVLength;
	crossVLength = crossV.getLength();

	if(crossVLength <= 0.01)
	{
		distTemp = v1.cross(PP).getLength();
		distTemp /= v1.getLength();
	}
	else
	{
		distTemp = fabs((crossV.dot(PP))/crossV.getLength());
	}

	return distTemp;
}

double angleBetweenLines(Point a, Point b, Point c, Point d)
{
	Vectors v1(a, b), v2(c, d);

	return v1.getAngleDegree(v2);
}

//calculate the adjust value for a strand
double adjust(int i, const vector<pair<int, int> > &trace, const vector<SequenceNode> &sequenceNodeContainer)
{
	//i > j, trace from larger index
	//consider the strands in same sheet
	if(sequenceNodeContainer[trace[i].first].getType() != "S")
		return 0.0;
	if((trace[i].first - trace[i-1].first) <= 1)
		return 0.0;
	double temp = 0;

	for(int k = trace[i].first; k > trace[i-1].first+1; k--)
	{
		int start, end;
		start = k;
		end = k-1;
		if(sequenceNodeContainer[end].getType() != "S")//only consider the loop between strands
			break;
		//if((sequenceNodeContainer[start].getStart()-sequenceNodeContainer[end].getEnd()) > 7)//the strands should be in same sheet
			//break;
		temp += sequenceNodeContainer[end].getLength()*3.5;//length for each amino acid on beta strand
	}

	return temp;
}

//strands in same sheet may have same distance to previous sticks, the weight should be adjusted
void sheetAdjust(const vector<vector<pair<int, int> > > &traceContainer, vector<double> &traceWeightContainer, const vector<SequenceNode> &sequenceNodeContainer)
{
	for(int i = 0; i < traceContainer.size(); i++)
	{
		vector<pair<int, int> > traceTemp;
		traceTemp = reverseTopology(traceContainer[i]);
		for(int j = traceTemp.size()-1; j > 0; j--)
		{
			double temp;
			temp = adjust(j, traceTemp, sequenceNodeContainer);
			//if(temp > 1)
				//cout<<"Trace: ["<<traceTemp[j].first<<","<<traceTemp[j].second<<"] ["<<traceTemp[j-1].first<<", "<<traceTemp[j-1].second<<"]: "<<temp<<endl;
			traceWeightContainer[i] -= temp;
		}
	}
}

//get all the trace between SSEs by the topology
//all the points and traces are read from the files which are generated by Kamal's code
void getTrace(const vector<pair<int, int> > &topology)
{
	//create folder to save the trace points and stick points
	DFCDirectory("./other/Point");

	//read all clique central point from cliqueCenter.txt
	vector<Point> pointContainer;

	ifstream cliqueCenterFile("./input/cliqueCenter.txt");

	if(!cliqueCenterFile)
	{
		cerr<<"Can not open ./input/cliqueCenter.txt ..."<<endl;
		exit(1);
	}

	string str;

	while(getline(cliqueCenterFile, str))
	{
		stringstream stream;
		stream<<str;
		double xTemp, yTemp, zTemp;
		stream>>xTemp>>yTemp>>zTemp;
		Point pointTemp(xTemp, yTemp, zTemp);
		pointContainer.push_back(pointTemp);
	}

	cliqueCenterFile.close();

	//save clique cenral point
	ofstream cliqueFile("./other/Point/clique.pdb");
	for(int i = 0; i < pointContainer.size(); i++)
		pointContainer[i].save(cliqueFile);
	cliqueFile.close();

	//get the trace points
	vector<vector<Point> > traceContainer;
	ifstream tracePathFile("./input/tracePath.txt");

	if(!tracePathFile)
	{
		cerr<<"Can not open tracePath.txt"<<endl;
		exit(1);
	}

	while(getline(tracePathFile, str))
	{
		vector<Point> traceTemp;
		char *strTemp;
		strTemp = new char [str.size()+1];
		strcpy(strTemp, str.c_str());

		char *pch;
		pch = strtok(strTemp, " ");
		
		while(pch != NULL)
		{
			traceTemp.push_back(pointContainer[atoi(pch)]);
			pch = strtok(NULL, " ");
		}

		traceContainer.push_back(traceTemp);
	}

	tracePathFile.close();

	//save each trace points
	for(int i = 0; i < traceContainer.size(); i++)
	{
		stringstream stream;
		stream<<"./other/Point/trace"<<i<<".pdb";
		ofstream traceTempFile(stream.str().c_str());
		for(int j = 0; j < traceContainer[i].size(); j++)
		{
			traceContainer[i][j].save(traceTempFile, j);
		}
		traceTempFile.close();
	}

	vector<vector<Point> > topologyTraceContainer;
	ifstream weightListFile("./input/weightList.txt");

	if(!weightListFile)
	{
		cerr<<"Can not open weightList.txt"<<endl;
		exit(1);
	}

	while(getline(weightListFile, str))
	{
		stringstream stream;
		stream<<str;

		int startFirst, startSecond, endFirst, endSecond, startTrace, endTrace;
		double weight;
		stream>>startFirst>>startSecond>>endFirst>>endSecond>>weight>>startTrace>>endTrace;
		for(int i = 0; i < topology.size()-1; i++)
		{
		}
	}
}

//extend line by the length expected
void extendLine(Point a, Point b, Point &aTemp, Point &bTemp, double l)
{
	double ratio;
	ratio = l/a.dist(b);

	Point c;
	c.set((a.getX()+b.getX())/2, (a.getY()+b.getY())/2, (a.getZ()+b.getZ())/2);

	bTemp.set(c.getX()+(b.getX()-c.getX())*ratio, c.getY()+(b.getY()-c.getY())*ratio, c.getZ()+(b.getZ()-c.getZ())*ratio);

	aTemp.set(c.getX()+(a.getX()-c.getX())*ratio, c.getY()+(a.getY()-c.getY())*ratio, c.getZ()+(a.getZ()-c.getZ())*ratio);
}

//save cluster centers to a specific file
void saveClusterCenter(vector<vector<Point> > cluster, string str)
{
	vector<Point> centerPointContainer;

	for(int i = 0; i < cluster.size(); i++)
	{
		Point center;
		center = getClusterCenter(cluster[i]);
		centerPointContainer.push_back(center);
	}

	savePoint(centerPointContainer, str);
}

//save points to a specific file
void savePoint(vector<Point> points, string str)
{
	ofstream file(str.c_str());

	for(int i = 0; i < points.size(); i++)
	{
		points[i].save(file, 0, 0);
	}

	file.close();
}

//check if two points are in the specific range
bool withinRange(Point a, Point b, double r)
{
	double temp;
	temp = a.dist(b);
	//roundValue(temp);
	temp = round(temp, 3);

	if(temp <= r)
		return true;

	return false;
}

//get closest point to current point that has not been marked
int getCentralPoint(const vector<Point> &pointContainer, const Point &prevCentralPoint, const vector<bool> &mark)
{
	double minDist = 999;
	int minIndex = -1;

	for(int i = 0; i < pointContainer.size(); i++)
	{
		if(mark[i])//only check the points that are marked
		  continue;
		
		double tempDist;
		tempDist = prevCentralPoint.dist(pointContainer[i]);

		//roundValue(tempDist);
		tempDist = round(tempDist, 3);

		if(tempDist < minDist)
		{
			minDist = tempDist;
			minIndex = i;
		}
	}

	return minIndex;
}

//check if all mark has been set to be true
bool isAllMarked(vector<bool> container)
{
	for(int i = 0; i < container.size(); i++)
		if(!container[i])
			return false;

	return true;
}

//recursion function to cluster points
void clusterRecursion(const vector<Point> &pointContainer, vector<vector<Point> > &clusterContainer, vector<bool> &mark, Point prevCentralPoint, double r)
{
	//return if all points are clustered
	if(isAllMarked(mark))
		return;

	//get central point
	Point centralPoint;
	int tempIndex;

	tempIndex = getCentralPoint(pointContainer, prevCentralPoint, mark);//get the closest point to previous central point as new central point

	centralPoint = pointContainer[tempIndex];
	mark[tempIndex] = true;

	//create new cluster
	vector<Point> cluster;

	cluster.push_back(centralPoint);//push central point to cluster

	//create cluster
	int i;
	for(i = 0; i < pointContainer.size(); i++)
	{
		//push valid point to cluster
		if(withinRange(centralPoint, pointContainer[i], r) && !mark[i])//within range and unmarked
		{
			//set new central point
			double tempx, tempy, tempz;

			tempx = (centralPoint.getX()*cluster.size()+pointContainer[i].getX())/(cluster.size()+1);
			tempy = (centralPoint.getY()*cluster.size()+pointContainer[i].getY())/(cluster.size()+1);
			tempz = (centralPoint.getZ()*cluster.size()+pointContainer[i].getZ())/(cluster.size()+1);

			//push point to cluster
			cluster.push_back(pointContainer[i]);
			mark[i] = true;//set mark to true for this point

			//reset central point
			Point centralPointTemp;
			centralPointTemp.set(tempx, tempy, tempz);

			centralPoint = centralPointTemp;

			i = 0;
		}
	}

	clusterContainer.push_back(cluster);
	
	clusterRecursion(pointContainer, clusterContainer, mark, centralPoint, r);
}

//get the center of cluster
Point getClusterCenter(const vector<Point> &cluster)
{
	Point temp;

	for(int i = 0; i < cluster.size(); i++)
	{
		temp.set(cluster[i].getX()+temp.getX(), cluster[i].getY()+temp.getY(), cluster[i].getZ()+temp.getZ());
	}

	temp.set(temp.getX()/cluster.size(), temp.getY()/cluster.size(), temp.getZ()/cluster.size());

	return temp;
}

//cluster points by cutoff r
void peakClustering(const vector<Point> &pointContainer, vector<vector<Point> > &clusterContainer, Point startPoint, double r)
{
	vector<bool> mark;
	for(int i = 0; i < pointContainer.size(); i++)
		mark.push_back(false);

	clusterRecursion(pointContainer, clusterContainer, mark, startPoint, r);

	cout<<"\n================================="<<endl;
	cout<<"Have done clustering of voxel ..."<<endl;
	cout<<"All voxels are clustered to "<<clusterContainer.size()<<" clusters ..."<<endl;
	//for(int i = 0; i < clusterContainer.size(); i++)
		//cout<<i<<" "<<clusterContainer[i].size()<<endl;

	cout<<"Output clusters centers to ./other/clusterCenter.pdb ..."<<endl;
	saveClusterCenter(clusterContainer, "./other/clusterCenter.pdb");
	cout<<"==================================="<<endl;
	/*ofstream centerFile("./other/center.pdb");//save the center points of cluster
	vector<Point> clusterCenterPoints;
	for(int i = 0; i < clusterContainer.size(); i++)
	{
		getClusterCenter(clusterContainer[i]).save(centerFile, 0, 0);
	}
	centerFile.close();*/

	/*for(int i = 0; i < clusterContainer.size(); i++)
	{
		stringstream stream;
		stream<<"./other/Cluster"<<i<<".pdb";
		savePoint(clusterContainer[i], stream.str());
	}*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//given a line and a point, returns the point where these two intersect
//it looks faster than the other one "Coordinate pointLineIntersection(Coordinate p1,Coordinate p2,Coordinate p3)"
double linePointIntersection(Point lineP1, Point lineP2, Point p, Point &intersectionPnt){

	//create the vectors represents the line and a nother vector represents a line from lineP1 and the point p
	Vectors v1(lineP2, lineP1), v2(p,lineP1);

	double dotProd = v1.dot(v2);
	double lineLengthSqr = (lineP2.getX() - lineP1.getX())*(lineP2.getX() - lineP1.getX()) + (lineP2.getY() - lineP1.getY())*(lineP2.getY() - lineP1.getY()) + (lineP2.getZ() - lineP1.getZ())*(lineP2.getZ() - lineP1.getZ());

	double e = dotProd/lineLengthSqr;

	
	intersectionPnt.set(lineP1.getX() + e * (lineP2.getX() - lineP1.getX()), lineP1.getY() + e * (lineP2.getY() - lineP1.getY()), lineP1.getZ() + e * (lineP2.getZ() - lineP1.getZ()));

	/*
	if e is :
		negative	: the intersection point is before point#1 in the line
		0			: the intersection point is on the first point
		b/w (0,1)	: the intersection point is on the line
		> 1			: the intersection point is after the second point in the line
	*/
	return e;
}

//round a double number, precision number is greater than 0
double round(double num, int precision)
{
	double temp;

	double pow10;

	pow10 = pow(10, precision-1);

	temp = fabs(num)*pow10;

	temp = double(int(temp+0.5))/pow10;

	if(num > 0)
		return temp;
	else
		return temp*(-1);
}

void showSticks(const vector<StickNode> &container)
{
	cout<<"\n=========Sticks========"<<endl;
	for(int i = 0; i < container.size(); i++)
		container[i].show();
	cout<<"========================\n"<<endl;
}

void readSticks(vector<StickNode> &container, string name, string type)
{
	if(name.substr(0, 1) == "-")
	{
		cerr<<"No "<<type<<" information will be read ..."<<endl;
		return;
	}

	ifstream file;
	/*file.open(name.c_str());

	if(!file)
	{
		cerr<<"Can not open "<<name<<endl;
		exit(1);
	}*/

	file.open(name.c_str());
	if(!file)
	{
		cerr<<__FILE__<<"/"<<__LINE__<<" Can not open "<<name<<" ..."<<endl;
		exit(1);
	}

	string str;

	int index = 0;
	bool finish = false;

	//read sticks
	while(!file.eof())
	{
		vector<Point> pointContainer;//save points in each stick
		int stickIndex;
		string ID;
		//read single stick
		while(getline(file, str) && str.size() > 5)
		{
			//stop when meet "----------------", which is comment line, content after this line is used to save true topology information
			if(str.substr(0, 1) == "-")
			{
				finish = true;
				break;
			}

			if(str.size() > 10)
			{
				stringstream stream;
				stream<<str;
				double x, y, z;
				stream>>x>>y>>z>>stickIndex>>ID;
				Point pointTemp(x, y, z);
				pointContainer.push_back(pointTemp);
			}
		}

		if(pointContainer.size() > 0)
		{
			StickNode nodePositive(pointContainer, type, index, 1, ID), nodeNegative(pointContainer, type, index, -1, ID);//create two nodes with different direction, 1, stick point order is same as sequence, -1, vice verse
			container.push_back(nodePositive);
			container.push_back(nodeNegative);
			index++;
		}

		if(finish)//do not read the line after "---------"
			break;
	}

	//check each point number in each stick to make sure sticks are legal
	for(int i = 0; i < container.size(); i++)
	{
		vector<Point> pointContainerTemp;
		pointContainerTemp = container[i].getContainer();

		if(pointContainerTemp.size() < 2)
		{
			cout<<"Pleas check file "<<name<<" to make sure the point number in each stick are legal ..."<<endl;
		}
	}
}

void showSequence(const vector<SequenceNode> &container)
{
	cout<<"\n=========Sequence==========="<<endl;
	for(int i = 0; i < container.size(); i++)
		container[i].show();
	cout<<"==============================\n"<<endl;
}

void readSequence(vector<SequenceNode> &container, string name)
{
		ifstream file;
		file.open(name.c_str());

		if(!file)
		{
			cerr<<"Can not open sequence file!"<<endl;
			exit(1);
		}

		//read sequence information
		string str;

		while(getline(file, str))
		{
			if(str.size() < 6 && str.size() > 1)//read type and index, such as "H0", "S1"
			{
				string type;
				type = str.substr(0, 1);
				int index;
				index = atoi(str.substr(1, str.size()-1).c_str());

				//read start index and end index for each SSEs
				vector<pair<int, int> > c;
				while(getline(file, str))
				{
					if(str.size() < 2)//break if read empty line, ready to read next record
						break;
					pair<int, int> p;

					int start, delimitor, end;
					start = str.find("(");
					delimitor = str.find(",");
					end = str.find(")");

					p.first = atoi(str.substr(start+1, delimitor-1-start).c_str());
					p.second = atoi(str.substr(delimitor+1, end-1-delimitor).c_str());
					c.push_back(p);
				}

				SequenceNode node(c, type, index);//create new sequence node

				container.push_back(node);//push sequence node to sequence node container
			}
		}

		//close sequence input file
		file.close();
}

//read true topology from ./trueTopology
vector<pair<int, int> > getTrueTopology(string name)
{	ifstream inputFile(name.c_str());
	if(!inputFile)
	{
		cerr<<"Can not open "<<name<<endl;
		exit(1);
	}

	vector<pair<int, int> > container;

	string str;

	while(getline(inputFile, str))
	{
		stringstream stream;
		stream<<str;
		pair<int, int> temp;
		stream>>temp.first>>temp.second;
		container.push_back(temp);
	}

	inputFile.close();

	cout<<"\nRead true topology: "<<endl;
	for(int i = 0; i < container.size(); i++)
		cout<<"["<<container[i].first<<","<<container[i].second<<"] ";
	cout<<endl;

	return container;
  
}

//compare two topology
bool isSameTopology(const vector<pair<int, int> > &firstTopology, const vector<pair<int, int> > &secondTopology)
{
	if(firstTopology.size() != secondTopology.size())
		return false;

	for(int i = 0; i < firstTopology.size(); i++)
	{
		if(firstTopology[i].first != secondTopology[i].first || firstTopology[i].second != secondTopology[i].second)
			return false;
	}

	return true;
}

//reverse topology order
vector<pair<int, int> > reverseTopology(const vector<pair<int, int> > &topology)
{
	vector<pair<int, int> > temp;

	for(int i = topology.size() -1; i >= 0; i--)
	{
		temp.push_back(topology[i]);
	}

	return temp;
}

//save topologies into file by rank
void saveTopology(const vector<vector<pair<int, int> > > &topologyContainer, const vector<double> &traceWeightContainer, const vector<pair<int, int> > &trueTopology, string rankFile)
{
	ofstream outputFile(rankFile.c_str());

	int trueTopologyIndex = -1;

	for(int i = 0; i < topologyContainer.size(); i++)
	{
		vector<pair<int, int> > firstTopology, secondTopology;
		firstTopology = reverseTopology(topologyContainer[i]);
		secondTopology = trueTopology;
		if(isSameTopology(firstTopology, secondTopology))
		{
			trueTopologyIndex = i+1;
			break;
		}
	}

	outputFile<<"True Topology: ";
	for(int i = 0; i < trueTopology.size(); i++)
	{
		outputFile<<"["<<trueTopology[i].first<<","<<trueTopology[i].second<<"] ";
	}
	outputFile<<endl;

	if(trueTopologyIndex > 0)
		outputFile<<"\nTrue Topology's Rank is: "<<trueTopologyIndex<<endl;
	else
		outputFile<<"\nTrue Topology is out of the Top "<<topologyContainer.size()<< "Topologies ..."<<endl;

	outputFile<<endl;

	for(int i = 0; i < topologyContainer.size(); i++)
	{
		outputFile<<setw(10)<<left<<i+1<<":";
		for(int j = topologyContainer[i].size()-1; j >= 0; j--)
		{
			outputFile<<"["<<topologyContainer[i][j].first<<","<<topologyContainer[i][j].second<<"] ";
		}
		outputFile<<setw(10)<<traceWeightContainer[i];
		outputFile<<endl;
	}

	outputFile.close();
}

void showSSELength(const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer, const vector<pair<int, int> > &trueTopology)
{
	cout<<"\n----------True Topology	SSE Length	Stick Length	Stick Side Length-------"<<endl;
	for(int i = 0; i < trueTopology.size(); i++)
	{
		cout<<"["<<trueTopology[i].first<<","<<trueTopology[i].second<<"]	"<<sequenceNodeContainer[trueTopology[i].first].getLengthAngstrom()<<"/"<<sequenceNodeContainer[trueTopology[i].first].getLengthAngstrom()*0.4<<"	"<<stickNodeContainer[trueTopology[i].second].getLength()<<"/"<<stickNodeContainer[trueTopology[i].second].getLength()*0.4<<"	"<<stickNodeContainer[trueTopology[i].second].getSidePointDistance()<<"/"<<stickNodeContainer[trueTopology[i].second].getSidePointDistance()*0.4<<endl;
	}
	cout<<"------------------------------------------------------------------------------------------"<<endl;
}

