#ifndef ULTILITY_H
#define ULTILITY_H
#include <string>
#include <vector>
#include "Point.h"
#include "SequenceNode.h"
#include "StickNode.h"
using namespace std;

double distanceBetweenLines(Point, Point, Point, Point);
double angleBetweenLines(Point, Point, Point, Point);
double adjust(int, const vector<pair<int, int> > &, const vector<SequenceNode> &);
void sheetAdjust(const vector<vector<pair<int, int> > > &, vector<double> &, const vector<SequenceNode> &);
void getTrace(const vector<pair<int, int> > &);
void extendLine(Point, Point, Point &, Point &, double);//extend line to specific length
void saveClusterCenter(vector<vector<Point> >, string);//save cluster centers to a file
void savePoint(vector<Point>, string);//save points to a file
bool withinRange(Point, Point, double);//check if two points are in the range
int getCentralPoint(const vector<Point> &, const Point &, const vector<bool> &);//get the index of the closest point to current point
bool isAllMarked(vector<bool>);//check if all marks have been set
void clusterRecursion(const vector<Point> &, vector<vector<Point> > &, vector<bool> &, Point, double);//recursion function to cluster points
Point getClusterCenter(const vector<Point> &);//get the central point of the cluster
void peakClustering(const vector<Point> &, vector<vector<Point> > &, Point, double);//cluster points and output the central points of clusters into ./other/cluster.pdb
double linePointIntersection(Point, Point, Point, Point &);
double round(double, int);
void showSequence(const vector<SequenceNode> &container);
void readSequence(vector<SequenceNode> &container, string name);
void showSticks(const vector<StickNode> &container);
void readSticks(vector<StickNode> &container, string name, string type);
vector<pair<int, int> > getTrueTopology(string);//read true topology from ./trueTopology
bool isSameTopology(const vector<pair<int, int> > &, const vector<pair<int, int> > &);//compare two topologies
vector<pair<int, int> > reverseTopology(const vector<pair<int, int> > &);//reverse pair order in topology
void saveTopology(const vector<vector<pair<int, int> > > &topologyContainer, const vector<double> &traceWeightContainer,  const vector<pair<int, int> > &trueTopology, string rank);//save topologies into rank file
void showSSELength(const vector<SequenceNode> &, const vector<StickNode> &, const vector<pair<int, int> > &);//show the length in both sequence and sticks
#endif
