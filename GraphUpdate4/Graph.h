#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include "MRC.h"
#include "GraphNode.h"
#include "SequenceNode.h"
#include "StickNode.h"
#include "Path.h"

class Graph
{
	private: vector<vector<GraphNode> > array;
	public:
		 Graph(){}

		 vector<vector<pair<int, int> > > topK(int K, const vector<SequenceNode> &, const vector<StickNode> &, vector<double> &);//check the top K path

		 void generateTopK(int, int &, int, vector<Path> &, vector<vector<pair<int, int> > > &, vector<double> &);//generate top K pathes

		 void explore(Path, int, vector<Path> &);//explore each path by cutting the edge one by one

		 bool inH(const pair<int, int> &, const vector<pair<int, int> > &);//check if the node in the node container

		 void traceBack(Record, pair<int, int>, vector<pair<int, int> > &);//track the minimum for a node
		 void trace(const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer);//trace the minimum path

		 void table(const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer);//create record table, has to be called after build the graph

		 void readParameter(double &, double &);//read parameter in the graph operations

		 void outputTracePaths(const vector<Point> &, const vector<vector<int> > &, const vector<double> &);//output trace paths in ./other/Trace

		 double getBestFitting(pair<int, int>  &, const vector<vector<int> > &, const vector<double> &, const vector<Point> &, int, int, double, double, double, double, double);//get best fitting trace length, trace pair int the path, trace point index container, trace lengths, trace points container, start point index, end point index, ideal path length, gap value, original weight, penalty for the case that no trace between sticks, secondary penalty for the gap that is less than cutoff

		 //find all the path from one end point of stick to the other end point of stick
		 void findTracePaths(vector<vector<int> > &,                      //adjacency matrix for clusters graph
				      vector<vector<double> > &,                                                         //matrix has centroid-centroid distances b/w each pair of clusters
				      int,                                                           //the starting point where you want to start ur tracing
				      int,                                                  //number of clusters without clusters for SS ends
				      vector<vector<int> > &,                            //trace paths found for density map
				      vector<double> &,                                     //parallel vector with paths contains the cost for each trace path
				      double maxLngth = 500.0);

		 void BronKerbosch(vector<vector<int> > &adjMtrx, vector<vector<int> > &clx, vector<int> R,vector<int> P,vector<int> X);//Bron-Kerbosc algorithm to find all cliques in a graph

		 void findallCliques(vector<vector<Point> > inClusters, vector<vector<int> > adjMtrx, vector<vector<int> > &clx, vector<vector<int> > &clxAdjMtrx, vector<vector<Point> > &clxClusters);//generate cliques

		 void buildAdjMtrx(vector<vector<Point> > &clusters, vector<vector<double> > &cDist, vector<vector<int> > &adjMtrx, double apix, int nClustersOriginal, double continuatyTHR);//build adjancy matrix

		 void buildUpdate(vector<Point> &, vector<vector<int> > &, const MRC &mrc, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer, int traceOutput);//build graph, set weight, inner, outer, row, column, rowIndex, columnIndex to each node by taking skeleton as reference

		 void betaSheet(const vector<SequenceNode> &, const vector<StickNode> &);//adjust the edge weight which include beta strand

		 double getSheetPenalty(pair<int, int>, pair<int, int>, const vector<SequenceNode> &, const vector<StickNode> &);

		 double nodeDirectionPenalty(pair<int, int>, pair<int, int>, const vector<SequenceNode> &,  const vector<StickNode> &, double);

		 double helixMatch(pair<int, int>, pair<int, int>, const vector<SequenceNode> &, const vector<StickNode> &);

		 double helixNodeMatch(pair<int, int>, const vector<SequenceNode> &, const vector<StickNode> &);

		 double neighborStrand(pair<int, int>, pair<int, int>, const vector<SequenceNode> &, const vector<StickNode> &, double);
		 vector<pair<int, int> > getStrandPair(const vector<SequenceNode> &);

		 double sheetDirectionPenalty(pair<int, int>, pair<int, int>, const vector<SequenceNode> &, const vector<StickNode> &);

		 double sheetGapPenalty(pair<int, int>, pair<int, int>, const vector<SequenceNode> &, const vector<StickNode> &);

		 int sheetGap(pair<int, int>, pair<int, int>, const vector<StickNode> &);

		 double sheetWeightUpdate(pair<int, int>, pair<int, int>, const vector<SequenceNode> &, const vector<StickNode> &);

		 bool isTruePath(const vector<pair<int, int> > &, pair<int, int>, pair<int, int>);//check if the edge is in the topology

		 //create the graph with Kamal's TopoDP_May2012's output
		 void build(string, const vector<SequenceNode> &, const vector<StickNode> &);

		 void type(const vector<pair<int, int> > &trueTopology);//check if any true topology node is set as invalid in graph
		 void build(const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer);//build graph, set weight, inner, outer, row, column, rowIndex, columnIndex to each node

		 void set(GraphNode &, GraphNode &, const vector<SequenceNode> &, const vector<StickNode> &);//set weight, inner, outer for two related nodes

		 double nAADiffAngstrom(const GraphNode &, const vector<SequenceNode> &, const vector<StickNode> &);//set up the loop length by the number of amino acid

		 int nAADiff(const GraphNode &, const vector<SequenceNode> &, const vector<StickNode> &);//set up the loop length by the number of amino acid

		 double sequenceDist(const GraphNode &, const GraphNode &, const vector<SequenceNode> &, const vector<StickNode> &);//check the distance between two nodes in sequence

		 double stickDist(const GraphNode &, const GraphNode &, const vector<SequenceNode> &, const vector<StickNode> &);//check the distance between two nodes in stick sequence

		 GraphNode getNode(pair<int, int> n) const
		 {
			 return array[n.first][n.second];
		 }

		 GraphNode getNode(int r, int c) const
		 {
			 return array[r][c];
		 }

		 void showTrace() const;
		 void showGraph(string) const;
		 void showNode() const;
		 void showTable() const;
		 void showPath(const vector<pair<int, int> > &) const;
		 void showCandidate(vector<Path>) const;
		 vector<pair<int, int> > pathToTrace(int, Path, vector<double> &);//show the whole path

		 void savePath(const vector<Point> &cliqueCenterContainer, const vector<vector<int> > &tracePaths, const vector<pair<int, int> > &topology, string dir);//save the trace path with pdb format
};
#endif
