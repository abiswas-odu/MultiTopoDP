#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>
#include <ctime>
#include "MRC.h"
#include "Graph.h"
#include "SequenceNode.h"
#include "Ultility.h"
#include "StickNode.h"
#include "Record.h"
#include "Set.h"
#include "Path.h"
#include "sort.h"
using namespace std;

//Generate top K pathes
vector<vector<pair<int, int> > > Graph::topK(int K, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer, vector<double> &traceWeightContainer)
{
	int rowTemp, columnTemp;

	rowTemp = sequenceNodeContainer.size();//get the number of row of graph
	columnTemp = stickNodeContainer.size();//get the number of column of graph

	vector<Path> candidateContainer;

	int diff;
	diff = rowTemp - columnTemp/2;

	//initialize condidate container
	for(int i = rowTemp-1; i >= rowTemp-1-diff; i--)
		for(int j = 0; j < columnTemp; j++)
		{
			vector<Record> recordContainerTemp;
			recordContainerTemp = array[i][j].getContainer();

			for(int k = 0; k < recordContainerTemp.size(); k++)
			{
				int numberTemp = 1;
				numberTemp <<= (columnTemp/2);
				numberTemp--;
				//if(recordContainerTemp[k].getPath().getCardinality() == columnTemp/2)
				if(recordContainerTemp[k].getPath().getValue() == numberTemp)
				{
					Path pathTemp;//trace, record, RNode, CNode, H, w
					pair<int, int> currentNode, nextNode; 
					currentNode.first = i;
					currentNode.second = j;
					pathTemp.addTrace(currentNode);//set up trace
					pathTemp.setCNode(currentNode);//set up CNode
					pathTemp.setWeight(recordContainerTemp[k].getF());//set up w
					nextNode = recordContainerTemp[k].getPrev();
					pathTemp.setRNode(nextNode);//set up RNode
					Record nextRecord;
					Set currentSet;
					currentSet = recordContainerTemp[k].getPath();
					currentSet.remove(array[i][j].getStickID());
					vector<Record> nextRecordContainerTemp;
					nextRecordContainerTemp = array[nextNode.first][nextNode.second].getContainer();
					for(int m = 0; m < nextRecordContainerTemp.size(); m++)
					{
						if(currentSet.equal(nextRecordContainerTemp[m].getPath()))
						{
							nextRecord = nextRecordContainerTemp[m];
							break;
						}
					}
					pathTemp.setRecord(nextRecord);//set up record
					candidateContainer.push_back(pathTemp);//push path into candidate container
				}
			}
		}

	insertionSort(candidateContainer);//sort candidates in the container

	//showCandidate(candidateContainer);//show candidate after initialization

	//pathToTrace(1, candidateContainer[0]);

	//output top K minimum pathes
	int rank;
	rank = 1;
	vector<vector<pair<int, int> > >  topKContainer;
	generateTopK(K, rank, columnTemp,  candidateContainer, topKContainer, traceWeightContainer);

	return topKContainer;
}

//show path while trace back the path from the end of the graph
void Graph::showCandidate(vector<Path> candidateContainer) const
{
	cout<<"==========Candidate==============="<<endl;
	for(int i = 0; i < candidateContainer.size(); i++)
	{
		candidateContainer[i].show();
	}
	cout<<"=================================="<<endl;
}

//generate top K traces
void Graph::generateTopK(int K, int &rank, int columnTemp, vector<Path> &candidateContainer, vector<vector<pair<int, int> > > &topKContainer, vector<double> &traceWeightContainer)
{
	if(candidateContainer.empty() || rank > K)
		return;

	Path top;
	top = candidateContainer[0];

	vector<pair<int, int> > traceTemp;
	traceTemp = pathToTrace(rank, top, traceWeightContainer);//output current path

	//traceTemp.reverse();//reverse the order of the trace

	topKContainer.push_back(traceTemp);//save trace to trace container

	candidateContainer.erase(candidateContainer.begin());//pop up the first

	explore(top, columnTemp, candidateContainer);//generate more candidate by cutting head edge before coincide node

	insertionSort(candidateContainer);//sort candidate container

	generateTopK(K, ++rank, columnTemp, candidateContainer, topKContainer, traceWeightContainer);
}

//check if the node has been in the node container
bool Graph::inH(const pair<int, int> &currentNode, const vector<pair<int, int> > &nodeContainer)
{
	for(int i = 0; i < nodeContainer.size(); i++)
	{
		if(currentNode.first == nodeContainer[i].first && currentNode.second == nodeContainer[i].second)
			return true;
	}

	return false;
}

void Graph::explore(Path path, int columnTemp, vector<Path> &candidateContainer)
{
	path.addHeadEdge(path.getRNode());//add next node into head edge container, which cut the edge between current node and next node

	vector<pair<int, int> > inner;
	vector<double> weightIn;
	inner = array[path.getCNode().first][path.getCNode().second].getInner();//get the inner nodes of the coincide node
	weightIn = array[path.getCNode().first][path.getCNode().second].getInWeight();

	Record minRecord;//save the record which matches the path and has minimum F value plus edge weight
	double minWeight;//save the edge weight
	minWeight = 9999.0;
	pair<int, int> minNode;//save the node which includes the minimum record
	minRecord.setF(9999.0);
	bool getCandidate = false;
	//check all the inner nodes for the coincide node
	for(int i = 0; i < inner.size(); i++)
	{
		//check the corresponding record if the node is not in the head edge container
		if(!inH(inner[i], path.getH()))
		{
			vector<Record> recordContainerTemp;
			recordContainerTemp = array[inner[i].first][inner[i].second].getContainer();
			for(int j = 0; j < recordContainerTemp.size(); j++)
			{
				Record recordTemp;
				recordTemp = recordContainerTemp[j];
				if(recordTemp.getPath().equal(path.getRecord().getPath()))
				{
					if((recordTemp.getF()+weightIn[i]) < (minRecord.getF()+minWeight))
					{
						minRecord = recordTemp;
						minWeight = weightIn[i];
						minNode = inner[i];
						getCandidate = true;
					}
				}
			}
		}
	}

	if(getCandidate)//create candidate path and insert it into candidate container if candidate was found
	{
		Path candidatePath;
		candidatePath.setTrace(path.getTrace());//set candidate trace to be the trace of current path
		candidatePath.setRecord(minRecord);//set candidate record to be the minimum record
		candidatePath.setRNode(minNode);//set candidate RNode to be the minimum node
		candidatePath.setCNode(path.getCNode());//set candidate CNode to be the CNode of current path
		candidatePath.setH(path.getH());//set candidate head edge to be all the edges which have been cut
		for(int i = 0; i < inner.size(); i++)
		{
			if(inner[i].first == path.getRNode().first && inner[i].second == path.getRNode().second)
			{
				candidatePath.setWeight(path.getWeight()+(minRecord.getF()+minWeight-path.getRecord().getF()-weightIn[i]));//set candidate weight to be the sum of original weight and difference between two pathes
				break;
			}
		}

		candidateContainer.push_back(candidatePath);//push the candidate path into candidate container
	}

	Path nextPath;
	//set trace
	nextPath.setTrace(path.getTrace());//copy the path trace to next path trace
	nextPath.addTrace(path.getRNode());//add RNode of path into next path trace

	//return if the start node is arrived
	if(nextPath.getTrace().size() == columnTemp/2)
		return;

	//set CNode
	nextPath.setCNode(path.getRNode());//set RNode of path to be the CNode of next path
	//set Weight
	nextPath.setWeight(path.getWeight());//set weight of path to be the weight of next path
	//set H
	nextPath.clearHeadEdge();

	Record nextRecordTemp;//save next record
	pair<int, int> nextNodeTemp;//save next RNode

	//set RNode
	nextNodeTemp = path.getRecord().getPrev();
	nextPath.setRNode(nextNodeTemp);

	Set nextPathTemp;
	nextPathTemp = path.getRecord().getPath();
	nextPathTemp.remove(array[path.getRNode().first][path.getRNode().second].getStickID());

	vector<Record> nextRecordContainer;
	nextRecordContainer = array[nextNodeTemp.first][nextNodeTemp.second].getContainer();//get record container for RNode
	for(int i = 0; i < nextRecordContainer.size(); i++)
	{
		if(nextRecordContainer[i].getPath().equal(nextPathTemp))
		{
			nextRecordTemp = nextRecordContainer[i];
			break;
		}
	}

	//set Record
	nextPath.setRecord(nextRecordTemp);

	explore(nextPath, columnTemp, candidateContainer);//recursive checking by cut the edge of path one by one
}

//convert path to trace, path include part trace and part path set, convert path set to real trace and combine these two parts to get the real full trace
vector<pair<int, int> > Graph::pathToTrace(int rank, Path path, vector<double> &traceWeightContainer)
{
	pair<int, int> currentNode;
	currentNode = path.getRNode();

	Record recordTemp;
	recordTemp = path.getRecord();

	vector<pair<int, int> > traceContainerTemp;

	traceBack(recordTemp, currentNode, traceContainerTemp);//get trace starting from the nexe node

	vector<pair<int, int> > pathTrace;
	pathTrace = path.getTrace();//get the second part trace

	//combine two parts of traces
	vector<pair<int, int> > combineTrace;
	for(int i = 0; i < pathTrace.size(); i++)
	{
		combineTrace.push_back(pathTrace[i]);
	}

	for(int i = 0; i < traceContainerTemp.size(); i++)
	{
		combineTrace.push_back(traceContainerTemp[i]);
	}

	cout<<"-------------------------------"<<endl;
	cout<<"Rank: "<<rank<<" Weight: "<<path.getWeight()<<endl;
	traceWeightContainer.push_back(path.getWeight());
	showPath(combineTrace);

	return combineTrace;
}

void Graph::trace(const vector<SequenceNode> &sequenceNodeContainer, const  vector<StickNode> &stickNodeContainer)
{
	int rowTemp, columnTemp;

	rowTemp = sequenceNodeContainer.size();
	columnTemp = stickNodeContainer.size();

	vector<Record> minContainer;
	vector<pair<int, int> > nodeContainer;
	Record minimum;
	minimum.setF(9999.0);

	int diff;
	diff = rowTemp - columnTemp/2;

	//check last row to last row - diff to get miminum f value records
	for(int k = rowTemp-1; k >= rowTemp-1-diff; k--)
	{
	for(int i = 0; i < columnTemp; i++)
	{
		vector<Record> containerTemp;
		containerTemp = array[k][i].getContainer();
		for(int j = 0; j < containerTemp.size(); j++)
		{
			Set pathTemp;
			pathTemp = containerTemp[j].getPath();
			if(pathTemp.getCardinality() != columnTemp/2)//make sure path go through all the sticks
				continue;
			double temp1, temp2;
			temp1 = round(containerTemp[j].getF(), 4);
			temp2 = round(minimum.getF(), 4);

			if(temp1 < temp2)
			{
				minContainer.clear();
				minContainer.push_back(containerTemp[j]);
				minimum = containerTemp[j];
				nodeContainer.clear();
				pair<int, int> nodeTemp;
				nodeTemp.first = k;
				nodeTemp.second = i;
				nodeContainer.push_back(nodeTemp);
			}
			else if(temp1 == temp2)
			{
				minContainer.push_back(containerTemp[j]);
				pair<int, int> nodeTemp;
				nodeTemp.first = k;
				nodeTemp.second = i;
				nodeContainer.push_back(nodeTemp);
			}
		}
	}
	}

	cout<<"============================"<<endl;
	cout<<"There are "<<minContainer.size()<<" minimum path ..."<<endl;
	for(int i = 0; i < minContainer.size(); i++)
	{
		cout<<"ID: "<<array[nodeContainer[i].first][nodeContainer[i].second].getID()<<endl;
		minContainer[i].show();

		vector<pair<int, int> > nodeContainerTemp;
		traceBack(minContainer[i], nodeContainer[i], nodeContainerTemp);
		showPath(nodeContainerTemp);
		cout<<"---------------------"<<endl;
	}
}

void Graph::showPath(const vector<pair<int, int> > &nodeContainer) const
{
	cout<<"Path: "<<endl;
	for(int i = nodeContainer.size()-1; i >= 0; i--)
	{
		array[nodeContainer[i].first][nodeContainer[i].second].showNode();
	}

	cout<<endl;

	cout<<"SSEs correspondance (sequence, sticks): "<<endl;
	for(int i = nodeContainer.size()-1; i >= 0; i--)
	{
		array[nodeContainer[i].first][nodeContainer[i].second].show();
	}

	cout<<endl;

	cout<<"TopoDP:"<<endl;
	for(int i = nodeContainer.size()-1; i >= 0; i--)
	{
		array[nodeContainer[i].first][nodeContainer[i].second].showTopoDP();
	}

	cout<<endl;
}

void Graph::traceBack(Record record, pair<int, int> node, vector<pair<int, int> > &nodeContainer)
{
	nodeContainer.push_back(node);

	//return if only one element in the path, which is the start point of path
	if(record.getPath().getCardinality() == 1)
		return;

	//get next node
	Record recordNext;
	pair<int, int> nodeNext;
	nodeNext = record.getPrev();

	Set setCurrent;
	setCurrent = record.getPath();
	setCurrent.remove(array[node.first][node.second].getStickID());

	vector<Record> recordContainer;
	recordContainer = array[nodeNext.first][nodeNext.second].getContainer();

	for(int  i = 0; i < recordContainer.size(); i++)
	{
		Set setTemp;
		setTemp = recordContainer[i].getPath();

		if(setTemp.equal(setCurrent))
		{
			recordNext = recordContainer[i];
			break;
		}
	}

	traceBack(recordNext, nodeNext, nodeContainer);
}

void Graph::table(const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	//get row and column of graph
	int rowTemp, columnTemp;
	rowTemp = sequenceNodeContainer.size();
	columnTemp = stickNodeContainer.size();

	int diff;
	diff = sequenceNodeContainer.size()-stickNodeContainer.size()/2;

	//initialize the graph record for first row to diffth row
	for(int j = 0; j <= diff; j++)
	{
		for(int i = 0; i < columnTemp; i++)
		{
			Record recordTemp;//create record
			if(array[j][i].getType() != "N")
			{
				Set setTemp(array[j][i].getStickID());//set value for set
				recordTemp.setPath(setTemp);//set path
				recordTemp.setF(0);//set f value
				array[j][i].addFirstRecord(recordTemp);
			}
		}
	}

	cout<<"Total "<<(rowTemp-1)*columnTemp<<" nodes except first row ..."<<endl;

	int startTime, endTime;
	startTime = clock();
	//update tables for other nodes
	for(int i = 1; i < rowTemp; i++)
	{
		for(int j = 0; j < columnTemp; j++)
		{
			int startNodeTime, endNodeTime;
			startNodeTime = clock();

			if(array[i][j].getType() == "N")
				continue;
			vector<pair<int, int> > inner;
			vector<double> weightIn;
			inner = array[i][j].getInner();
			weightIn = array[i][j].getInWeight();
			vector<bool> hashTable(1<<(columnTemp/2), false);//initialize hash table
			vector<int> hashTableIndex(1<<(columnTemp/2), -1);//save the index of record in the current node
			Set setTemp(array[i][j].getStickID());//get set for the stick of the current column
			for(int m = 0; m < inner.size(); m++)
			{
				//create a set for end node of edge by the column index to check if this column has been gone through
				int startRow, startColumn;
				startRow = inner[m].first;
				startColumn = inner[m].second;
				vector<Record> recordContainerTemp;//get table of start node
				recordContainerTemp = array[startRow][startColumn].getContainer();
				for(int n = 0; n < recordContainerTemp.size(); n++)
				{
					Set temp, tempU;
					temp = recordContainerTemp[n].getPath();
					if(!temp.contains(setTemp))
					{
						tempU = temp.unions(setTemp);
						double fTemp;
						fTemp = recordContainerTemp[n].getF() + weightIn[m];
						pair<int, int> prev;
						prev.first = startRow;
						prev.second = startColumn;

						Record recordTemp;
						recordTemp.setPath(tempU);
						recordTemp.setF(fTemp);
						recordTemp.setPrev(prev);

						if(!hashTable[tempU.getValue()])
						{
							array[i][j].addFirstRecord(recordTemp);
							hashTable[tempU.getValue()] = true;
							hashTableIndex[tempU.getValue()] = array[i][j].getRecordNum()-1;
						}
						else
						{
							int hashTableIndexTemp;
							hashTableIndexTemp = hashTableIndex[tempU.getValue()];
							array[i][j].updateRecord(hashTableIndexTemp, recordTemp);
						}
					}
				}
			}
			//array[i][j].recordSort();//sort record container for the target node

			endNodeTime = clock();

			//show how many percent has been processed
			cout<<"--Table generation has finished "<<double((i-1)*columnTemp+j+1)/(rowTemp-1)/columnTemp*100<<"%, took "<<double(endNodeTime-startNodeTime)/CLOCKS_PER_SEC<<" seconds ..."<<endl;
		}
	}

	endTime = clock();

	cout<<"Generating table took "<<double(endTime-startTime)/CLOCKS_PER_SEC<<" seconds ..."<<endl;

	//showTable();//output table information
}

/*void Graph::readParameter(double &radiiAlpha, double &radiiBeta)
{
	ifstream file("./parameterGraph");
	string str;
	stringstream stream;

	//read radiiAlpha
	getline(file, str);
	stream<<str;
	stream>>str;
	stream>>radiiAlpha;
	stream.str("");

	//read radiiBeta
	getline(file, str);
	stream<<str;
	stream>>str;
	stream>>radiiBeta;
	stream.str("");

	file.close();
}*/

void Graph::buildUpdate(vector<Point> &cliqueCenterContainerCopy, vector<vector<int> > &tracePathsCopy,
		const MRC &mrc, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer,
		int traceOutput,double gap,double penalty,double secondaryPenalty)
{
	cout<<"\n---------Build Loop Weight----------"<<endl;
	vector<Point> voxelContainer, voxelContainerDelete;//save the voxel points and voxel points after deleting
	voxelContainer = mrc.getPoints();

	cout<<"\nThere are "<<voxelContainer.size()<<" voxels in the skeleton ..."<<endl;

	cout<<"\nDelete voxels around sticks within the cylinder ..."<<endl;
	double radiiAlpha=2.5, radiiBeta=2.5;
	//readParameter(radiiAlpha, radiiBeta);
	cout<<"Radii alpha: "<<radiiAlpha<<" Radii beta: "<<radiiBeta<<endl;

	//delete points around stick
	for(int j = 0; j < voxelContainer.size(); j++)
	{
		bool mark = true;
		for(int i = 0; i < stickNodeContainer.size(); i++)
		{
			vector<Point> stickPointContainer;
			stickPointContainer = stickNodeContainer[i].getContainer();
			if(stickPointContainer.size() <= 1)
				continue;
			for(int k = 0; k < stickPointContainer.size()-1; k++)
			{
				Point stickStart, stickEnd;
				stickStart = stickPointContainer[k];//get start point
				stickEnd = stickPointContainer[k+1];//get end point
				//stickStart = stickNodeContainer[i].getStart();
				//stickEnd = stickNodeContainer[i].getEnd();

				double range;
				Point projectPoint;
				range = linePointIntersection(stickStart, stickEnd, voxelContainer[j], projectPoint);//<0 or > 1, not in cylinder, otherwise, point in cylinder
				double distToLine;
				distToLine = voxelContainer[j].dist(projectPoint);
				double rangeCutOff;
				if(stickNodeContainer[i].getType() == "H")//delete voxel for helix stick
					rangeCutOff = radiiAlpha;
				else if(stickNodeContainer[i].getType() == "S")//delete voxel for sheet stick
					rangeCutOff = radiiBeta;
				if(range >= 0 && range <= 1 && distToLine < rangeCutOff)
				{
					mark = false;
					break;
				}
			}
			if(!mark)//break out if voxel point is delete
				break;
		}

		if(mark)
			voxelContainerDelete.push_back(voxelContainer[j]);
	}

	voxelContainer.clear();//clear the voxel container

	cout<<"\nAfter delete point, there are "<<voxelContainerDelete.size()<<" left ..."<<endl;

	//save voxel point after deletion
	cout<<"\nSave left voxel points into ./other/densityDelete.pdb ..."<<endl;
	savePoint(voxelContainerDelete, "./other/densityDelete.pdb");

	vector<vector<Point> > voxelCluster;//save the clusters

	//cluster skeleton points and save the center points to ./other/clusterCenter.pdb
	cout<<"\nPut left voxel points into clusters and save this central points into ./other/clusterCenter.pdb ..."<<endl;
	peakClustering(voxelContainerDelete, voxelCluster, stickNodeContainer[0].getStart(), 2*mrc.getApixX());

	voxelContainerDelete.clear();//clear the voxel after deletion

	vector<vector<double> > cDist(voxelCluster.size(), vector<double>(voxelCluster.size(), 9999.0));//cenroid to centroid of cluters distances

	vector<vector<int> > adjMtrx, clx, clxAdjMtrx;//adjancy list, data structure where cliques will be stored and the adjacency matrix for the cliques

	cout<<"\nBuild adjancy list by clusters ..."<<endl;
	//build adjancy list and calculate corresponding dist
	buildAdjMtrx(voxelCluster, cDist, adjMtrx, mrc.getApixX(), voxelCluster.size(), 1.1*mrc.getApixX());

	cout<<"Create cliques from clusters ..."<<endl;

	vector<vector<Point> > clxClusters;

	findallCliques(voxelCluster, adjMtrx, clx, clxAdjMtrx, clxClusters);

	voxelCluster.clear();//clear the points in clusters

	cout<<"Generate "<<clxClusters.size()<<" cliques ..."<<endl;

	//save the clique centers to ./other/cliqueCenter.pdb
	cout<<"\nSave cluster center to ./other/cliqueCenter.pdb ..."<<endl;
	saveClusterCenter(clxClusters, "./other/cliqueCenter.pdb");

	//attach the end points of sticks to the cliques
	cout<<"\nAdd side points of sticks to the cliques container, in which each clique only contains one point ..."<<endl;
	vector<vector<Point> > newClusters;//creater new cluster container to save the cliques after add the side points of the sticks into cliques container
	newClusters = clxClusters;
	for(int i = 0; i < stickNodeContainer.size(); i = i+2)
	{
		vector<Point> tmpCluster1, tmpCluster2;
		tmpCluster1.push_back(stickNodeContainer[i].getStart());
		tmpCluster2.push_back(stickNodeContainer[i].getEnd());
		newClusters.push_back(tmpCluster1);//start point
		newClusters.push_back(tmpCluster2);//end point
	}

	clxClusters.clear();//clear cliques container

	//save the points that after add end points of sticks to the clique to ./other/points.pdb, those points are going to be used to calcuate edge weight and generate loop structure
	cout<<"\nSave the central points into ./other/points.pdb after add side points of sticks into cliques container ..."<<endl;
	saveClusterCenter(newClusters, "./other/points.pdb");
	
	int nClusters = clx.size();//get the original clique number before add the side points of the sticks into the cliques container

	cout<<"Original cliques container size: "<<nClusters<<" Cliques container size after adding side points: "<<newClusters.size()<<endl;

	//re-calculate Adjacency Mtrx and short distances matrix for cliques
	cout<<"\nRe-build adjacency matrix and short distance matrix ..."<<endl;

	cDist.clear();
	cDist = vector<vector<double> > (newClusters.size(), vector<double>(newClusters.size(), 9999.0));

	//update cDist and clxAdjMtrx, which are clique central distance and clique adjacency matrix respectively
	buildAdjMtrx(newClusters, cDist, clxAdjMtrx, mrc.getApixX(), nClusters, 1.1*mrc.getApixX());

	//save the central point of each clique into container
	vector<Point> cliqueCenterContainer;
	for(int i = 0; i < newClusters.size(); i++)
	{
		Point cliqueCenterTemp;
		cliqueCenterTemp = getClusterCenter(newClusters[i]);
		cliqueCenterContainer.push_back(cliqueCenterTemp);
	}

	newClusters.clear();//clear cliques container

	//find trace pths from density map "Skeleton" or "Local Peaks"
	cout<<"\nFind trace paths ..."<<endl;
	vector<vector<int> > tracePaths;
	vector<double> traceLengths;
	
	tracePaths.clear ();
	traceLengths.clear ();
	int graphRowNum, graphColumnNum;//graph row number and column number
	graphRowNum = sequenceNodeContainer.size();//get row number of the graph
	graphColumnNum = stickNodeContainer.size();//get the column number of the graph, stick container contained both positive and negative sticks

	for (int i = 0; i < graphColumnNum; i++){

		//determine the maximum length of a loop could get out from this end
		int maxLength = 0.0;
		for (int j = 0; j < graphRowNum; j++){
			vector<pair<int, int> > outerContainer;
			outerContainer = array[j][i].getOuter();
			for (int k=0; k < outerContainer.size (); k++){
				int nLoop;//number of amino acid between two nodes
				int startIndex, endIndex;
				startIndex = j;
				endIndex = outerContainer[k].first;
				nLoop = sequenceNodeContainer[endIndex].getStart() - sequenceNodeContainer[startIndex].getEnd();
				if (nLoop > maxLength)
					maxLength = nLoop;
			}
		}

		//find trace paths for first end point
		findTracePaths(clxAdjMtrx, cDist, nClusters + i, nClusters, tracePaths, traceLengths, maxLength*3.8);
	}

	//delete empty trace
	/*vector<vector<int> > tracePathsTemp;
	vector<double> traceLengthsTemp;

	for(int i = 0; i < tracePaths.size(); i++)
	{
		//only keep the trace that points number is greater than 1, which is the start point
		if(tracePaths[i].size() > 1)
		{
			tracePathsTemp.push_back(tracePaths[i]);
			traceLengthsTemp.push_back(traceLengths[i]);
		}
	}*/

	//tracePaths.clear();//clear trace container
	//traceLengths.clear();//clear trace length container

	//save the clique center points, trace paths and trace length
	if(traceOutput)
		outputTracePaths(cliqueCenterContainer, tracePaths, traceLengths);

	cliqueCenterContainerCopy = cliqueCenterContainer;//save clique center points
	tracePathsCopy = tracePaths;//save traces

	cout<<"Total "<<tracePaths.size()<<" traces are found ..."<<endl;

	//set weight for each loop
	cout<<"\nSet weight for each loop ..."<<endl;

	cout<<"\nGap value takes "<<gap<<" angstroms ..."<<endl;
	cout<<"\nPenalty takes "<<penalty<<" for the sticks pair that no trace between them ..."<<endl;
	cout<<"\nSecondary penalty takes "<<secondaryPenalty<<" for the sticks pair that gap is less than cutoff ..."<<endl;

	//save the orignal weight, real weight and current weight
	//these values can be used to adjust parameter
	ofstream weightDebugFile("./other/weightDebug.txt");
	if(!weightDebugFile)
	{
		cerr<<"Can not open ./other/weightDebug.txt"<<endl;
	}

	cout<<"\nOutput weight set up information into ./other/weightDebug.txt ..."<<endl;

	weightDebugFile<<setw(8)<<"Row"<<setw(8)<<"Column"<<setw(8)<<"Row"<<setw(8)<<"Column"<<setw(5)<<"T-1"<<setw(5)<<"T-2"<<setw(10)<<"Real"<<setw(10)<<"Ideal"<<setw(10)<<"Original"<<endl;

	//get true topology
	vector<pair<int, int> > trueTopology;
	trueTopology = getTrueTopology(trueTopoFile);

	cout<<"\nSet up weight for each edge ..."<<endl;
	//set up weight for each edge
	for(int i = 0; i < array.size(); i++)
	{
		for(int j = 0; j < array[i].size(); j++)
		{
			vector<pair<int, int> > outerContainer;
			outerContainer = array[i][j].getOuter();
			vector<double> outerWeightContainer;
			outerWeightContainer = array[i][j].getOutWeight();
			pair<int, int> startNode;
			startNode.first = i;
			startNode.second = j;
			for(int k = 0; k < outerContainer.size(); k++)
			{
				pair<int, int> endNode;
				endNode = outerContainer[k];

				double originalWeight;
				originalWeight = outerWeightContainer[k];

				double idealLength;
				//idealLength = sequenceNodeContainer[endNode.first].getStart() - sequenceNodeContainer[startNode.first].getEnd();
				idealLength = sequenceDist(array[startNode.first][startNode.second], array[endNode.first][endNode.second], sequenceNodeContainer, stickNodeContainer);
				//idealLength *= 3.8;//get the loop lenght in sequence

				int startNodeEndPointIndex, endNodeStartPointIndex;
				bool isEven;//even is positive direction, odd is negative direction
				if(startNode.second%2 == 0)
					isEven = true;
				else
					isEven = false;
				if(isEven)
					startNodeEndPointIndex = startNode.second + 1 + nClusters;
				else
					startNodeEndPointIndex = startNode.second - 1 + nClusters;
				if(endNode.second%2 == 0)
					isEven = true;
				else
					isEven = false;
				if(isEven)
					endNodeStartPointIndex = endNode.second + nClusters;
				else
					endNodeStartPointIndex = endNode.second + nClusters;

				double realWeight;
				pair<int, int> tracePair;//save the trace pair in the path, -1 means no trace
				tracePair.first = -1;
				tracePair.second = -1;
				//get real weight and trace pair, in which -1 represents no trace pair is found, -2 represents no full covered path is found
				realWeight = getBestFitting(tracePair, tracePaths, traceLengths, cliqueCenterContainer, startNodeEndPointIndex, endNodeStartPointIndex, idealLength, gap, originalWeight, penalty, secondaryPenalty);
				//cout<<"["<<startNode.first<<","<<startNode.second<<"] ["<<endNode.first<<","<<endNode.second<<"] "<<realWeight<<endl;

				//save trace pair information
				array[i][j].pushTracePair(tracePair);

				//double adjustTemp;
				//adjustTemp = sheetWeightUpdate(startNode, endNode, sequenceNodeContainer, stickNodeContainer);
				//cout<<"["<<startNode.first<<","<<startNode.second<<"] ["<<endNode.first<<","<<endNode.second<<"] "<<adjustTemp<<endl;
				//realWeight += adjustTemp;
				//set up inner weight and outer weight
				array[startNode.first][startNode.second].setWeightOut(k, realWeight);
				array[endNode.first][endNode.second].setWeightIn(startNode, realWeight);

				//output weight information
				weightDebugFile<<setw(8)<<startNode.first<<setw(8)<<startNode.second<<setw(8)<<endNode.first<<setw(8)<<endNode.second<<setw(5)<<tracePair.first<<setw(5)<<tracePair.second<<setw(10)<<realWeight<<setw(10)<<idealLength<<setw(10)<<originalWeight;

				if(isTruePath(trueTopology, startNode, endNode))
					weightDebugFile<<setw(10)<<"TruePath";

				weightDebugFile<<endl;
			}

			cout<<"Setting up finished "<<double(i*j)/((array.size()-1)*array[i].size())*100<<"% ..."<<endl;
		}
	}

	weightDebugFile.close();

	//save trace pair information into ./other/trace.txt
	showTrace();

	//cout<<"\tNumber of Total traces found : "<<tracePaths.size()<<endl<<endl;
	
	showGraph("./other/graphUpdate.txt");
}

void Graph::betaSheet(const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	//predict strand pair
	vector<pair<int, int> > strandPairContainer;
	strandPairContainer = getStrandPair(sequenceNodeContainer);

	//read true topology
	vector<pair<int, int> > trueTopology;
	trueTopology = getTrueTopology(trueTopoFile);

	string parameterStr;
	int addPenaltySheetGap=1, addPenaltySheetDirection=1, addNeighborStrandReward=1, addHelixMatchReward=1, addPenaltyNodeDirection=1;

	ofstream weightDebugFile("./other/weightDebugUpdate.txt");
	if(!weightDebugFile)
	{
		cerr<<"Can not open ./other/weightDebug.txt"<<endl;
	}

	cout<<"\nOutput weight set up information into ./other/weightDebug.txt ..."<<endl;

	weightDebugFile<<setw(8)<<"Row"<<setw(8)<<"Column"<<setw(8)<<"Row"<<setw(8)<<"Column"<<setw(5)<<"Real"<<setw(10)<<"Original"<<endl;

  	//update weight for each edge that include beta strand
	for(int i = 0; i < array.size(); i++)
	{
		for(int j = 0; j < array[i].size(); j++)
		{
			vector<pair<int, int> > outerContainer;
			outerContainer = array[i][j].getOuter();
			vector<double> outerWeightContainer;
			outerWeightContainer = array[i][j].getOutWeight();
			pair<int, int> startNode;
			startNode.first = i;
			startNode.second = j;
			for(int k = 0; k < outerContainer.size(); k++)
			{
				pair<int, int> endNode;
				endNode = outerContainer[k];

				double originalWeight;
				originalWeight = outerWeightContainer[k];

				double realWeight;

				//***********************Adjust weight****************
				double penaltySheetGap = 0;
				if(addPenaltySheetGap)
					penaltySheetGap = sheetGapPenalty(startNode, endNode, sequenceNodeContainer, stickNodeContainer);

				double penaltySheetDirection = 0;
				if(addPenaltySheetDirection)
					penaltySheetDirection = sheetDirectionPenalty(startNode, endNode, sequenceNodeContainer, stickNodeContainer);

				double neighborStrandReward = 0;
				if(addNeighborStrandReward)
					neighborStrandReward = neighborStrand(startNode, endNode, sequenceNodeContainer, stickNodeContainer, originalWeight);

				double helixMatchReward = 0;
				if(addHelixMatchReward)
					helixMatchReward = helixMatch(startNode, endNode, sequenceNodeContainer, stickNodeContainer);

				double penaltyNodeDirection = 0;
				if(addPenaltyNodeDirection)
					penaltyNodeDirection = nodeDirectionPenalty(startNode, endNode, sequenceNodeContainer, stickNodeContainer, originalWeight);
				

				//****************************************************

				realWeight = originalWeight;
				realWeight += penaltySheetGap;
				realWeight += penaltySheetDirection;
				realWeight += neighborStrandReward;
				realWeight += helixMatchReward;
				realWeight += penaltyNodeDirection;

				//set up inner weight and outer weight
				array[startNode.first][startNode.second].setWeightOut(k, realWeight);
				array[endNode.first][endNode.second].setWeightIn(startNode, realWeight);

				//output weight information
				weightDebugFile<<setw(8)<<startNode.first<<setw(8)<<startNode.second<<setw(8)<<endNode.first<<setw(8)<<endNode.second<<setw(10)<<realWeight<<setw(10)<<originalWeight;

				if(penaltySheetDirection > 0)
					weightDebugFile<<setw(10)<<"Direction";

				if(penaltySheetGap > 0)
					weightDebugFile<<setw(10)<<"SheetGap";

				if(isTruePath(trueTopology, startNode, endNode))
					weightDebugFile<<setw(10)<<"TruePath";

				if(neighborStrandReward < 0)
					weightDebugFile<<setw(10)<<"NStrands";

				if(helixMatchReward != 0)
					weightDebugFile<<setw(10)<<"HLength";

				if(penaltyNodeDirection > 0)
					weightDebugFile<<setw(10)<<"NodeDir";

				if(penaltyNodeDirection < 0)
					weightDebugFile<<setw(10)<<"NodeDM";

				weightDebugFile<<endl;
			}
		}
	}

	weightDebugFile.close();
}

double Graph::getSheetPenalty(pair<int, int> start, pair<int, int> end, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	int strandGap;
	strandGap = sheetGap(start, end, stickNodeContainer);

	StickNode startStickNode, endStickNode;
	startStickNode = stickNodeContainer[start.second];
	endStickNode = stickNodeContainer[end.second];
}

/*vector<int> headStrandInSequence(const vector<StickNode> &stickNodeContainer)
{
	for(int i = 0; i < stickNodeContainer.size(); i++)
	{
	}
}*/

//consider the strands between two sheets, these strands usually are small and can not be found sticks
/*void Graph::strandBetweenSheets(pair<int, int> start, pair<int, int> end, const vector<pair<int, int> > &strandPairContainer, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	//consider only beta sheet
	if(stickNodeContainer[start.second].getType() != "S" || stickNodeContainer[end.second].getType() != "S")
		return 0;
	//there is no direction if two sticks are not in same sheet
	if(stickNodeContainer[start.second].getSheetID() != stickNodeContainer[end.second].getSheetID())
		return 0;	
}*/

double Graph::nodeDirectionPenalty(pair<int, int> start, pair<int, int> end, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer, double originalWeight)
{
	if(sequenceNodeContainer[end.first].getStart()-sequenceNodeContainer[start.first].getEnd() > 4)
		return 0;

	Point start1, end1, start2, end2;

	if(stickNodeContainer[start.second].getDirection() == 1)
		end1 = stickNodeContainer[start.second].getEnd();
	else
		end1 = stickNodeContainer[start.second].getStart();

	if(stickNodeContainer[end.second].getDirection() == 1)
	{
		start2 = stickNodeContainer[end.second].getStart();
		end2 = stickNodeContainer[end.second].getEnd();
	}
	else
	{
		start2 = stickNodeContainer[end.second].getEnd();
		end2 = stickNodeContainer[end.second].getStart();
	}

	double distES, distEE;
	distES = end1.dist(start2);
	distEE = end1.dist(end2);

	if(distES > distEE)
		return 150;
	else
	{
		if(originalWeight > 3*3.8)
		       return -3*3.8;
		else if(originalWeight > 2*3.8)
		       return -2*3.8;
		else if(originalWeight > 3.8)
	 	       return -3.8;
	        return 0;
		//return -50;	
	}
}

//set extral reward for the helix that stick has same length with SSE in sequence
double Graph::helixMatch(pair<int, int> start, pair<int, int> end, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	double temp = 0;

	temp += helixNodeMatch(start, sequenceNodeContainer, stickNodeContainer);
	temp += helixNodeMatch(end, sequenceNodeContainer, stickNodeContainer);

	return temp;
}

double Graph::helixNodeMatch(pair<int, int> start, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	double temp = 0;
	if(stickNodeContainer[start.second].getType() == "H" && sequenceNodeContainer[start.first].getType() == "H")
	{
		if(sequenceNodeContainer[start.first].getLength() >= 13)
		{
			double stickLength, sequenceLength;
			stickLength = stickNodeContainer[start.second].getLength();
			sequenceLength = sequenceNodeContainer[start.first].getLengthAngstrom();
			if(stickLength*0.85 < sequenceLength && stickLength*1.2 > sequenceLength)
				temp -= 5;
		}
	}

	return temp;
}

double Graph::neighborStrand(pair<int, int> start, pair<int, int> end, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer, double originalWeight)
{
	//consider only beta sheet
	if(stickNodeContainer[start.second].getType() != "S" || stickNodeContainer[end.second].getType() != "S")
		return 0;
	//there is no direction if two sticks are not in same sheet
	if(stickNodeContainer[start.second].getSheetID() != stickNodeContainer[end.second].getSheetID())
		return 0;	

	double reward = 0;

	int diff = 0;

	if(end.first - start.first == 1 && sequenceNodeContainer[end.first].getStart()-sequenceNodeContainer[start.first].getEnd() <= 4)
		diff = 1;
	else
		return 0;

	int stickGap;
	stickGap = sheetGap(start, end, stickNodeContainer);

	if(stickGap == 1)
	{
		//int sequenceGap;
		//sequenceGap = sequenceNodeContainer[end.first].getStart()-sequenceNodeContainer[start.first].getEnd();

		if(originalWeight > 3*3.8)
		       return -3*3.8;
		else if(originalWeight > 2*3.8)
		       return -2*3.8;
		else if(originalWeight > 3.8)
	 	       return -3.8;
	        return 0;
		//return -50;
	}
	else
		return 0;
}

//check the position of strand turn for continuous strand in sequence
vector<pair<int, int> > Graph::getStrandPair(const vector<SequenceNode> &sequenceNodeContainer)
{
	vector<vector<int> > partContainer;
	vector<int> part;
	for(int i = 0; i < sequenceNodeContainer.size(); i++)
	{
		if(sequenceNodeContainer[i].getType() == "S")
			part.push_back(i);
		else
		{
			if(part.size() <= 1)
				part.clear();
			else
			{
				partContainer.push_back(part);
				part.clear();
			}
		}
		if(i == sequenceNodeContainer.size() -1)
		{
			if(part.size() <= 1)
				part.clear();
			else
			{
				partContainer.push_back(part);
				part.clear();
			}
		}
	}

	for(int i = 0; i < partContainer.size(); i++)
	{
		cout<<i<<": ";
		for(int j = 0; j < partContainer[i].size(); j++)
		{
			cout<<partContainer[i][j]<<" ";
		}
		cout<<endl;
	}

	vector<pair<int, int> > strandPairContainer;
	for(int i = 0; i < partContainer.size(); i++)
	{
		int centerIndex;
		centerIndex = partContainer[i].size()/2-1;
		int diff;
		diff = sequenceNodeContainer[partContainer[i][centerIndex+1]].getStart() - sequenceNodeContainer[partContainer[i][centerIndex]].getEnd();
		if(diff <= 4 && diff > 1)
		{
			pair<int, int> pairTemp(partContainer[i][centerIndex], partContainer[i][centerIndex+1]);
			strandPairContainer.push_back(pairTemp);
		}
	}

	for(int j = 0; j < strandPairContainer.size(); j++)
		cout<<strandPairContainer[j].first<<" "<<strandPairContainer[j].second<<endl;

	//exit(0);

	return strandPairContainer;
}

//set penalty for the direction of strand that is not reasonable to go along with the sequence
double Graph::sheetDirectionPenalty(pair<int, int> start, pair<int, int> end, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	//consider only beta sheet
	if(stickNodeContainer[start.second].getType() != "S" || stickNodeContainer[end.second].getType() != "S")
		return 0;
	//there is no direction if two sticks are not in same sheet
	if(stickNodeContainer[start.second].getSheetID() != stickNodeContainer[end.second].getSheetID())
		return 0;	

	int loopLength, strandLength1, strandLength2;
	loopLength = sequenceDist(array[start.first][start.second], array[end.first][end.second], sequenceNodeContainer, stickNodeContainer);
	strandLength1 = sequenceNodeContainer[start.first].getLengthAngstrom();
	strandLength2 = sequenceNodeContainer[end.first].getLengthAngstrom();

	if(loopLength > strandLength1 && loopLength > strandLength2)
		return 0;

	Point startPoint, endPoint, startPoint2, endPoint2;

	if(stickNodeContainer[start.second].getDirection() > 0)
	{
		endPoint = stickNodeContainer[start.second].getEnd();
	}
	else
	{
		endPoint = stickNodeContainer[start.second].getStart();
	}

	if(stickNodeContainer[end.second].getDirection() > 0)
	{
		startPoint2 = stickNodeContainer[end.second].getStart();
		endPoint2 = stickNodeContainer[end.second].getEnd();
	}
	else
	{
		startPoint2 = stickNodeContainer[end.second].getEnd();
		endPoint2 = stickNodeContainer[end.second].getStart();
	}

	double distES, distEE;//distance for end-start, end-end
	distES = endPoint.dist(startPoint2);
	distEE = endPoint.dist(endPoint2);

	int sequenceGap;//sequence gap
	sequenceGap = 1;
	if(end.first - start.first > 1)
	{
		for(int i = start.first+1; i < end.first; i++)
			if(sequenceNodeContainer[i].getType() == "S" && sequenceNodeContainer[i].getLength() > 2)
				sequenceGap++;
	}

	double penaltyTemp = 0;

	if(distES < distEE)
	{
		if(sequenceGap%2 == 0)//set penalty for even gap
			penaltyTemp = 30;
	}
	else
	{
		if(sequenceGap%2 == 1)//set penalty for odd gap
			penaltyTemp = 30;
	}

	return penaltyTemp;
}

//get the gap penalty if the stick gap are too wild
double Graph::sheetGapPenalty(pair<int, int> start, pair<int, int> end, const vector<SequenceNode> &sequenceNodeContainer,  const vector<StickNode> &stickNodeContainer)
{
	//consider only beta sheet
	if(stickNodeContainer[start.second].getType() != "S" || stickNodeContainer[end.second].getType() != "S")
		return 0;
	//there is no penalty if two sticks are not in same sheet
	if(stickNodeContainer[start.second].getSheetID() != stickNodeContainer[end.second].getSheetID())
		return 0;

	//get the gap in sequence
	int diff = 0;

	if(end.first - start.first == 1)
	{
		diff = 1;
	}
	else
	{
		for(int i = start.first; i < end.first; i++)
		{
			if(sequenceNodeContainer[i].getType() == "S")
				diff++;
		}
	}

	int stickGap;
	stickGap = sheetGap(start, end, stickNodeContainer);

	//if(stickGap > diff)
		//cout<<"Gap: "<<start.first<<" "<<start.second<<" "<<end.first<<" "<<end.second<<" "<<diff<<" "<<stickGap<<endl;

	if(stickGap <= diff)
		return 0;
	else
		return (stickGap-diff)*50;

}

//get the number of SSE that the gap between two sticks can contain
int Graph::sheetGap(pair<int, int> start, pair<int, int> end, const vector<StickNode> &stickNodeContainer)
{
	double distanceTemp = 0;
	distanceTemp = distanceBetweenLines(stickNodeContainer[start.second].getStart(), stickNodeContainer[start.second].getEnd(), stickNodeContainer[end.second].getStart(), stickNodeContainer[end.second].getEnd());//get distance between two sticks

	return int(distanceTemp/4.5+0.5);
}

//adjust weight if some stick miss within sheet
double Graph::sheetWeightUpdate(pair<int, int> start, pair<int, int> end, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	if(sequenceNodeContainer[start.first].getType() != sequenceNodeContainer[end.first].getType())
	{
		double adjustTemp = 0;

		if((end.first - start.first) > 1)
		{
			for(int i = start.first+1; i < end.first; i++)
			{
				if(sequenceNodeContainer[i].getType() == "S")
					adjustTemp += sequenceNodeContainer[i].getLength()*3.5;
				if(sequenceNodeContainer[i].getType() == "H")
					adjustTemp += sequenceNodeContainer[i].getLength()*1.5;
			}

			for(int i = start.first; i < end.first-1; i++)
			{
				adjustTemp += (sequenceNodeContainer[i+1].getStart() - sequenceNodeContainer[i].getStart())*3.8;
			}
		}


		return -adjustTemp;
	}

	for(int i = start.first; i <= end.first; i++)
	{
		if(sequenceNodeContainer[i].getType() != "S")//it is not in sheet if any alpha helix SSE exist between two nodes
			return 0;
	}

	int diff;
	diff = end.first - start.first;
	if(diff <= 1)
		return 0;

	double distanceTemp;
	distanceTemp = distanceBetweenLines(stickNodeContainer[start.second].getStart(), stickNodeContainer[start.second].getEnd(), stickNodeContainer[end.second].getStart(), stickNodeContainer[end.second].getEnd());//get distance between two sticks

	double angleTemp;
	angleTemp = angleBetweenLines(stickNodeContainer[start.second].getStart(), stickNodeContainer[start.second].getEnd(), stickNodeContainer[end.second].getStart(), stickNodeContainer[end.second].getEnd());//get distance between two sticks

	if(angleTemp > 90)
		angleTemp = 180 - angleTemp;

	//double adjustTemp = 0;

	/*if(distanceTemp < diff*4.5+2.0)
	{
		//remove the length of SSE betwen start and end nodes
		for(int i = start.first+1; i < end.first; i++)
		{
			adjustTemp += sequenceNodeContainer[i].getLength()*3.5;
		}

		//remove the length of loops between start and end nodes
		for(int i = start.first; i < end.first-1; i++)
		{
			adjustTemp += (sequenceNodeContainer[i+1].getStart() - sequenceNodeContainer[i].getStart())*3.8;
		}

		return adjustTemp;
	}
	else
		return 0;*/

	int diffTemp;
	diffTemp = fabs(int(distanceTemp/4.5+0.5) - diff);
	cout<<diffTemp<<endl;
	if(angleTemp < 45 && diffTemp < 2)//in same sheet
	{
		return diffTemp*20;
	}

	if(angleTemp < 45 && diffTemp >= 2)
		return diffTemp*50;

	if(angleTemp > 45)//in different sheet
	{
		double adjustTemp = 0;

		for(int i = start.first+1; i < end.first; i++)
		{
			adjustTemp += sequenceNodeContainer[i].getLength()*3.5;
		}

		for(int i = start.first; i < end.first-1; i++)
		{
			adjustTemp += (sequenceNodeContainer[i+1].getStart() - sequenceNodeContainer[i].getStart())*3.8;
		}

		return -adjustTemp;
	}

	return 0;
}

//check if the edge from start node to end node is involved in the topology
bool Graph::isTruePath(const vector<pair<int, int> > & topology, pair<int, int> startNode, pair<int, int> endNode)
{
	for(int i = 0; i < topology.size()-1; i++)
	{
		if(topology[i].first == startNode.first && topology[i].second == startNode.second && topology[i+1].first == endNode.first && topology[i+1].second == endNode.second)
			return true;
	}

	return false;
}

double Graph::getBestFitting(pair<int, int> &tracePair, const vector<vector<int> > &tracePaths, const vector<double> &traceLengths, const vector<Point> &cliqueCenterContainer, int startIndex, int endIndex, double idealLength, double gap, double originalWeight, double penalty, double secondaryPenalty)
{
	double bestFittingWeight = 9999.0;//save best fitting weight
	bool foundBest = false;

	//get the trace whose length is less than or equal to ideal length
	for(int i = 0; i < tracePaths.size(); i++)
	{
		if(tracePaths[i].size() >= 2)
		{
			if(tracePaths[i][0] == startIndex && tracePaths[i][tracePaths[i].size()-1] == endIndex && idealLength >= traceLengths[i])
			{
				double weightTemp;
				weightTemp = idealLength - traceLengths[i];
				if(weightTemp < bestFittingWeight)
				{
					foundBest = true;
					bestFittingWeight = weightTemp;
					tracePair.first = i;
					tracePair.second = i;
				}
			}
		}
	}

	//represents that no full covered path is found
	if(!foundBest)
	{
		tracePair.first = -2;
		tracePair.second = -2;
	}

	//cout<<"After full path tracing ..."<<endl;

	//find the trace that only include one stick end point
	vector<int> startContainer, endContainer;
	for(int i = 0; i < tracePaths.size(); i++)
	{
		if(tracePaths[i][0] == startIndex && tracePaths[i][tracePaths[i].size()-1] != endIndex)
			startContainer.push_back(i);
		if(tracePaths[i][0] == endIndex && tracePaths[i][tracePaths[i].size()-1] != startIndex)
			endContainer.push_back(i);
	}

	//cout<<"After start point tracing and end point tracing ..."<<endl;

	for(int i = 0; i < startContainer.size(); i++)
		for(int j = 0; j < endContainer.size(); j++)
		{
			//if(tracePaths[startContainer[i]].size() == 1 && tracePaths[endContainer[j]].size() == 1)//if both traces only contain side point, it is ilegal trace pair because there is no trace points between two side points
				//continue;
			Point temp1, temp2;
			temp1 = cliqueCenterContainer[tracePaths[startContainer[i]][tracePaths[startContainer[i]].size()-1]];
			temp2 = cliqueCenterContainer[tracePaths[endContainer[j]][tracePaths[endContainer[j]].size()-1]];
			double distTemp, weightTemp;
			distTemp = temp1.dist(temp2);
			if(distTemp > gap)//gap need less than cutoff
			{
				if(!foundBest)
				{
					tracePair.first = -3;
					tracePair.second = -3;
				}
				continue;
			}
			else
			{
				weightTemp = idealLength - (traceLengths[startContainer[i]] + traceLengths[endContainer[j]] + distTemp);
				if(weightTemp < 0)//path length longer than ideal length
				{
					if(!foundBest)
					{
						tracePair.first = -4;
						tracePair.second = -4;
					}
					continue;
				}
				else
				{
					if(weightTemp + secondaryPenalty < bestFittingWeight)
					{
						foundBest = true;
						bestFittingWeight = weightTemp + secondaryPenalty;//consider secondary penalty
						tracePair.first = startContainer[i];
						tracePair.second = endContainer[j];
					}
				}
			}
	/*if(tracePair.first == 41 && tracePair.second == 44)
	{
		cout<<"Trace 41 and Tracd 44 ..."<<endl;
		cout<<weightTemp<<endl;
		cout<<idealLength<<" "<<traceLengths[startContainer[i]]<<" "<<traceLengths[endContainer[j]]<<" "<<distTemp<<endl;
		cout<<bestFittingWeight<<endl;
	}*/
		}


	if(foundBest)
		return bestFittingWeight;
	//else if(tracePair.first == -4)
		//return originalWeight;
	else
		return originalWeight + penalty;//penalty for no path connection
}

void Graph::outputTracePaths(const vector<Point> &cliqueCenterContainer, const vector<vector<int> > &tracePaths, const vector<double> &traceLengths)
{
	system("mkdir ./other/Trace");

	cout<<"\nOutput all trace paths into folder ./other/Trace ..."<<endl;

	string dir;
	dir = "./other/Trace/index.txt";
	ofstream indexFile(dir.c_str());

	for(int i = 0; i < tracePaths.size(); i++)
	{
		stringstream stream;
		stream<<"./other/Trace/Trace"<<i<<".pdb";
		vector<Point> container;

		indexFile<<setw(5)<<i;

		for(int j = 0; j < tracePaths[i].size(); j++)
		{
			container.push_back(cliqueCenterContainer[tracePaths[i][j]]);
			indexFile<<setw(5)<<tracePaths[i][j];
		}

		savePoint(container, stream.str());

		indexFile<<setw(10)<<traceLengths[i]<<endl;
	}

	indexFile.close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *		find continueous traces in the density map starting from a particular point....starting point,
 *		2 dimension matrix have distances b/w each pair of clusters
 *		sDist is a matrix has the shortest distance b/w each pair of clusters
 */
void Graph::findTracePaths(vector<vector<int> > &adjMtrx, vector<vector<double> > &sDist, int startPnt,	int nClusters, vector<vector<int> > &paths, vector<double> &pathsCosts,	double maxLngth){

	pair<int, int>	*open;			//open set represetns clusters that are visited already and the number of children in the stack
	int nOpen;						//number of active elements in open
	pair<int, int>	*stack;			//clusters being processed and the parent for each cluster
	int nStack;						//number of active elements in the stack (also the index of the top of the stack)
	double				*openLength;	//the length of the trace so far for each node in open

	bool *visitFlag;
	pair<int, int> tmp, top;
	vector<int> onePath;
	double pathLength = 0.0;
	int i;
	int nChildMax = 0;			//max number of chilren for a node


	//get maximum number of children for a node
	for (i=0; i<adjMtrx.size (); i++)
		if (adjMtrx[i].size () > nChildMax)
			nChildMax = adjMtrx[i].size ();

	//initialize data structures
	stack = new pair<int, int> [adjMtrx.size ()];
	open = new pair<int, int> [adjMtrx.size () * nChildMax];
	openLength = new double [adjMtrx.size() * nChildMax];
	visitFlag = new bool [adjMtrx.size ()];
	nStack = 0;
	nOpen = 0;

	for (i=0; i<adjMtrx.size (); i++)
		visitFlag[i] = false;


	//push the starting point (the indx of starting point) to the stack
	tmp.first = startPnt;
	tmp.second = -1;				//no parent

	//stack.push_back (tmp);
	stack[nStack++] = tmp;

	while (nStack!=0){
		//pop the top of the stack and put its children (those clusters are satisfying threshold distance)
		top = stack[nStack-1];

		//cout<<top.first +1;			//cluster being processed

		//decrement number of neighbors in the stack for its parent
		if (top.second != -1){		//if it is not the start pnt
			open[top.second].second--;
			//cout<<"  PathLength= "<<openLength[nOpen-1];
			//cout<<"     ( parent: "<<open[top.second].first+1<<"  p.Indx= "<<top.second<<"      #children: "<<open[top.second].second<<" ).";
		}

		//cout<<endl;

		//stack.pop_back ();
		nStack--;

		//initialize number of children and push to open
		top.second = 0;
		//open.push_back (top);
		open[nOpen++] = top;

		visitFlag[top.first] = true;		//set visit flag

		//calculate the length of the path so far
		if (nOpen > 1){
			//get distance b/w last two clusters in open list
			int	openLastIndx = nOpen-1,
					addedClusterIndx = open[openLastIndx].first,
					clusterIndx = open[openLastIndx-1].first;

			//distance b/w centroids of the last two clusters in open list
			pathLength =	openLength[openLastIndx-1] + sDist[clusterIndx][addedClusterIndx];

			//openLength.push_back (pathLength);
			openLength [nOpen-1] = pathLength;
		}
		else
			//openLength.push_back (0.0);				//path length for first node
			openLength [nOpen-1] = 0.0;

		//don't continue with this node if you already exceeded the max length allowed
		if (pathLength > maxLngth && top.first < nClusters){
			//cout<<" The Path exceeds the maximum length allowd which is : "<<maxLngth<<endl;
			visitFlag[open[nOpen-1].first] = false;
			//open.pop_back ();
			nOpen--;
			//openLength.pop_back ();
			continue;
		}

		//if the cluster we r working on is one of SS ends. then do not generate its children b/s this would be a trace end
		if ( top.first < nClusters || top.first == startPnt){
			//generate children (neighbors) for the cluster
			for (i=0 ; i<adjMtrx[top.first].size(); i++){
				//cout<<"  "<<adjMtrx[top.first][i]+1<<" ";

				if (!visitFlag[adjMtrx[top.first][i]]){
					tmp.first  = adjMtrx[top.first][i];
					tmp.second = nOpen-1;			//save the index of the parent in open set
					//stack.push_back (tmp);
					stack[nStack++] = tmp;					//push neighbor to stack
					open[tmp.second].second++;				//increment number of neighbors in stack
					//cout<<"pushed to stack.   parentIndx= "<<tmp.second<<" ( "<<open[tmp.second].first+1<<" )."<<endl;
				}
				//else
				//	cout<<" found in open..."<<endl;
			}
		}


		//if we reach a leaf node...with no neighbor in the stack or SS end
		if (open[nOpen-1].second<1){
			//cout<<"  a leaf node has been reached "<<open[nOpen-1].first+1<<" .... "<<endl;
			onePath.clear();
			for (i=0; i<nOpen-1; i++)
				onePath.push_back(open[i].first);
			//push last cluster
			onePath.push_back (open[i].first );

			//save the path and its cost
			paths.push_back (onePath);
			pathsCosts.push_back (openLength[i]);

			//delete all nodes in open set with 0 neighbors
			i=nOpen-1;
			while (i>=0 && open[i].second <=0){
				//cout<<"       "<<open[i].first +1<<" deleted from open"<<endl;
				visitFlag[open[i].first] = false;
				//open.pop_back ();
				nOpen--;
				//openLength.pop_back ();
				i--;
			}
			//cout<<"    Open contents"<<endl;
			//for (i=0; i<nOpen;i++)
			//	cout<<"("<<open[i].first+1 <<","<<open[i].second <<") ";
			//cout<<endl;
		}
	}

	delete [] open;
	delete [] openLength;
	delete [] stack;
	delete [] visitFlag;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Bron-Kerbosc algorithm to find all cliques in a graph
void Graph::BronKerbosch(vector<vector<int> > &adjMtrx, vector<vector<int> > &clx, vector<int> R,vector<int> P,vector<int> X){

	if (P.empty() && X.empty ()){
		clx.push_back(R);
	}
	//choose a pivot node from P U X
	//I'll choose the node with maximum number of neighbors
	int u= -1;		//the index of the pivot node
	int maxN = 0;		//maximum number of neighbors
	int i,j,k;

	for (i=0; i<P.size(); i++){
		if (adjMtrx[P[i]].size() > maxN){
			u = P[i];
			maxN = adjMtrx[P[i]].size ();
		}
	}
	for (i=0; i<X.size(); i++){
		if (adjMtrx[X[i]].size() > maxN){
			u=X[i];
			maxN = adjMtrx[X[i]].size();
		}
	}

	//cout<<setw(cntr)<<" "<<"U choosen to be cluster# : "<<u+1<<endl;
	bool found = false;
	i=0;
	while (i<P.size ()){
		vector<int> tmpP, tmpX, tmpR;
		int v = P[i];
		found = false;

		if (!found){
			tmpR = R;
			tmpR.push_back(v);				//R U v
			//P intesect. N(v)
			for (j=0; j<P.size(); j++){
				for (k=0; k<adjMtrx[v].size(); k++){
					if (P[j] == adjMtrx[v][k]){
						tmpP.push_back (P[j]);
						break;
					}
				}
			}
			//X intesect. N(v)
			for (j=0; j<X.size(); j++){
				for (k=0; k<adjMtrx[v].size(); k++){
					if (X[j] == adjMtrx[v][k]){
						tmpX.push_back (X[j]);
						break;
					}
				}
			}

			BronKerbosch(adjMtrx, clx, tmpR, tmpP, tmpX);

			//update P and X
			X.push_back (v);

			P.erase (P.begin () + i);
			i--;

		}
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//finding cliques in a graph...given adjacency mtrx
//we use Bron and Kerbosch algorithm
//we keep iterately form cliques untill we reach a point that no more cliques with 3 elements or more can be formed
//nClusters is the number of initial clusters without clusters for SS ends
//adjMtrx is the initial adjacency Matrix b/w initial clusters
void Graph::findallCliques(vector<vector<Point> > inClusters,					//the coordinate points of initial clusters
					vector<vector<int> > adjMtrx,						//adjacency matrix of initial clusters
					vector<vector<int> > &clx,						//final cliques
					vector<vector<int> > &clxAdjMtrx,					//final adjacenvy matrix for cliques
					vector<vector<Point> > &clxClusters){				//final coordinate for points form cliques

	int i,j,k,l,m, maxClq;


	//clear previous contents
	clxAdjMtrx.clear ();
	clx.clear ();
	clxClusters.clear();

	//initiate sets
	vector<int>	P,			//the set contains vertices
					X,			//the NOT set
					R;			//the set will contain the maximum clique
	for (i=0; i<adjMtrx.size(); i++)
		P.push_back(i);


	//find cliques and save the indices of cluster form cliques in clx
	BronKerbosch(adjMtrx, clx, R,P,X);

	//delete cliques with only 1 or two nodes...
	i=0;
	maxClq=0;
	while (i<clx.size ()){
		if (clx[i].size () > maxClq)
			maxClq=clx[i].size ();			//get the size of largest clique
		//delete small cliques (of size 1 or 2)
		if (clx[i].size() < 3){
			clx.erase (clx.begin () + i);
			i--;
		}
		i++;
	}

	if (clx.size ()){
		//merge cliques
		//merge any two cliques of size 3...if they share 2 nodes
		//merge any two cliques of size 4...if they share 3 nodes
		//merge any two cliques of size 5 ..if they share 4 nodes and so on
		int sharedNodes,
				diff,							//the different cluster
				clqSze;

		int ** clxArr;						//array version of cliques (faster to deal with)
		int *nEntries;						//number of entries (clusters) in each clique;
		int nClxArr = clx.size ();				//number of cliques

		bool *isValidClq;						//a flag is true if the clique is valid
		bool *processedBefore;					//did this clique merged with another clique before

		int	**tmpClq;						//new produced clique from merging two prev. cliques
		int *tmpClqEntries;					//number of entries in the tmp cliq vector
		bool foundSimilar,						//flag is set if any two cliques found to be very similar
				contMerging = true;				//keep mergning if u still have cliques to merge

		//copy cliques data into clxArr
		clxArr = new int *[nClxArr];
		nEntries = new int[nClxArr];

		for (i=0; i<nClxArr; i++){
			nEntries[i] = clx[i].size ();
			clxArr[i] = new int [nEntries[i]];
			for (j=0; j<nEntries[i]; j++)
				clxArr[i][j] = clx[i][j];
		}

		clqSze=3;
		while (contMerging && clqSze < 9){

			//cout<<"clqSze= "<<clqSze<<endl;

			//initiate flags
			isValidClq = new bool [nClxArr];
			processedBefore = new bool [nClxArr];
			for (i=0; i<nClxArr; i++){
				isValidClq[i] = true;
				processedBefore[i] = false;
			}
			contMerging=false;

			//expected maximum number of merged cliques would be double the original number of cliques
			tmpClq = new int *[10*nClxArr];
			tmpClqEntries = new int [10*nClxArr];

			int tmpClqCntr = 0;		//initiate number of new cliques
			i=0;
			while (i<nClxArr){
				foundSimilar=false;
				bool pushedBefore = false;
				if (nEntries[i] == clqSze && isValidClq[i]){
					j=i+1;
					while (j<nClxArr){
						if (nEntries[j] == clqSze && isValidClq[j]){

							//find the different cluster b.w two cliques if any
							sharedNodes = 0;
							diff = -1;
							bool missing;
							for (l=0; l<nEntries[j];l++){
								missing = true;
								for (k=0; k<nEntries[i]; k++){
									if (clxArr[i][k] == clxArr[j][l]){
										sharedNodes++;
										missing = false;
										break;
									}
								}
								if (missing)
									diff = clxArr[j][l];
							}
							//if two cliques share n-1 clusters
							if (sharedNodes > clqSze-2){
								foundSimilar=true;
								if (diff == -1){			//to avoid any two identical cliques after many merging steps
									if (!pushedBefore){
										//copy the clique into tmpClq vector and update the counter
										tmpClq[tmpClqCntr] = new int [nEntries[i]];
										tmpClqEntries[tmpClqCntr] = nEntries[i];
										for (k=0; k<nEntries[i]; k++)
											tmpClq[tmpClqCntr][k] = clxArr[i][k];
										tmpClqCntr++;

										pushedBefore = true;
										//cout<<"clq# "<<i+1<<" and clq# "<<j+1<<" ARE IDENTICALS.. pushed to new list to clq# "<<tmpClq.size ()<<endl;
									}

									//delete j clique
									isValidClq[j] = false;

								}
								else{
									//copy the clique into tmpClique vector with the different cluster
									tmpClqEntries[tmpClqCntr] = nEntries[i]+1;
									tmpClq[tmpClqCntr] = new int [nEntries[i]+1];
									for (k=0; k<nEntries[i]; k++)
										tmpClq[tmpClqCntr][k] = clxArr[i][k];
									tmpClq[tmpClqCntr][k] = diff;					//add different cluster
									tmpClqCntr++;

									contMerging = true;
									processedBefore[j] = true;
									//cout<<"clq "<<i+1<<" and clq "<<j+1<<" have "<<sharedNodes<<" shared nodes.. diff is "<<diff+1<<".. merged to clq#"<<tmpClq.size ()<<endl;
								}
							}
						}
						j++;
					}
				}
				if (!foundSimilar && isValidClq[i]){
					if (!processedBefore[i]){
						//add the clique as is
						tmpClqEntries[tmpClqCntr] = nEntries[i];
						tmpClq[tmpClqCntr] = new int [nEntries[i]];
						for (k=0; k<nEntries[i]; k++)
							tmpClq[tmpClqCntr][k] = clxArr[i][k];
						tmpClqCntr++;
						//cout<<"clq "<<i+1<<" has no similar clq.... pushed to clq# "<<tmpClq.size ()<<endl;
					}
				}

				i++;

			}
			clqSze++;
			//cout<<"  Done.."<<endl;

			//re-copy everything to clxArr
			delete [] *clxArr;
			delete [] clxArr;
			delete [] nEntries;

			clxArr = new int *[tmpClqCntr];
			nEntries = new int [tmpClqCntr];
			for (i=0; i<tmpClqCntr; i++){
				clxArr[i] = new int [tmpClqEntries[i]];
				nEntries[i] = tmpClqEntries[i];
				for (j=0; j<tmpClqEntries[i]; j++)
					clxArr[i][j] = tmpClq[i][j];
			}
			nClxArr = tmpClqCntr;

			delete [] *tmpClq;
			delete [] tmpClq;
			delete [] tmpClqEntries;

			delete [] isValidClq;
			delete [] processedBefore;

		}

		//delete any clique completely contained in another clique
		//cout<<"# of cliques = "<<nClxArr<<"  Deleting duplicate cliques... ";

		bool *isValid;;
		isValid = new bool [nClxArr];
		for (i=0; i<nClxArr; i++){
			isValid[i] = true;
		}

		i=0;
		while (i<nClxArr){
			if (isValid[i]){
				j=i+1;
				while (j<nClxArr){
					if (isValid[j]){
						int sharedNodes=0;
						for (k=0;k<nEntries[i]; k++){
							for (l=0; l<nEntries[j]; l++){
								if (clxArr[i][k] == clxArr[j][l])
									sharedNodes++;
							}
						}
						if (sharedNodes == nEntries[i]){
							isValid[i] = false;
							break;
						}
						if (sharedNodes == nEntries[j]){
							isValid[j] = false;
						}
					}
					j++;
				}
			}
			i++;
		}

		vector<vector< int> > tmpCliques;
		for (i=0; i<nClxArr; i++){
			vector<int> tmpEntry;
			if (isValid[i]){
				for (j=0; j<nEntries[i]; j++)
					tmpEntry.push_back(clxArr[i][j]);
				tmpCliques.push_back (tmpEntry);
			}

		}
		clx = tmpCliques;
		tmpCliques.clear ();
		//cout<<"Done."<<endl;
	}

	//resolve shared clusters....if a cluster appear in two cliques....keep it in only one of them according to the closest distances
	//cout<<"# of cliques = "<<clx.size()<<" Deleting completely shared cliques... ";
	float dist;
	for (i=0; i<clx.size (); i++){
		for (j=i+1; j<clx.size (); j++){
			k=0;
			while (k<clx[i].size ()){
				l=0;
				while (l<clx[j].size ()){
					bool deleted=false;
					//cout<<i+1<<"-"<<k+1<<" with "<<j+1<<"-"<<l+1<<endl;
					if (clx[i][k] == clx[j][l]){
						//cout<<"   found a shared cluster "<<clx[i][k]+1<<endl;
						//find to which clique this cluster is closest
						float minDist = 99999.0;
						int x, p;
						for (m=0; m<clx[i].size (); m++){
							if (m!= k){
								for (p=0; p<inClusters[clx[i][m]].size (); p++){
									for (x=0; x<inClusters[clx[i][k]].size (); x++){
										dist = inClusters[clx[i][m]][p].dist(inClusters[clx[i][k]][x]);
										if ( dist < minDist)
											minDist = dist;
									}
								}
							}
						}
						deleted = false;
						for (m=0; m<clx[j].size (); m++){
							if (m!=l){
								for (p=0; p<inClusters[clx[j][m]].size (); p++){
									for (x=0; x<inClusters[clx[j][l]].size (); x++){
										dist = inClusters[clx[j][m]][p].dist(inClusters[clx[j][l]][x]);
										if ( dist < minDist){
											//keep it in clique j..delete it from clique i
											clx[i].erase (clx[i].begin () + k);
											k--;
											deleted = true;
											break;
										}
									}
									if (deleted)
										break;
								}
							}
							if (deleted)
								break;
						}

						if (!deleted){
							clx[j].erase (clx[j].begin () + l);
							l--;
							//cout<<"clq# "<<j+1<<" size now = "<<clx[j].size ()<<endl;
						}
					}
					if (deleted)
						break;
					l++;
				}
				k++;
			}
		//	getchar();
		}
	}
	//delete empty clusters
	i=0;
	while (i<clx.size ()){
		if (clx[i].size () == 0){
			clx.erase (clx.begin () + i);
			i--;
		}
		i++;
	}

	//print valid cliques

	//copy all other clusters that were not part of cliques
	vector<int> tmpClx(1);
	bool found;
	for (k=0; k<adjMtrx.size (); k++){
		found = false;
		for (i=0; i<clx.size (); i++){
			for (j=0;j<clx[i].size (); j++){
				if (clx[i][j] == k){
					found = true;
					break;
				}
			}
			if (found)
				break;
		}
		if (!found){
			tmpClx[0] = k;
			clx.push_back (tmpClx);
		}
	}

	//merge clusters (points form clusters) to form cliques
	vector<Point> nClq;		//new clique
	Point nCentroid;
	for (i=0; i<clx.size(); i++){
		nClq.clear ();
		//nCentroid.setX(0.0);
		//nCentroid.setY(0.0);
		//nCentroid.setZ(0.0);

		for (j=0; j<clx[i].size(); j++){
			for (k=0; k<inClusters[clx[i][j]].size (); k++)			//consider all points in cluster, Kamal's program need remove last point, which is centroid point of the cluster
				nClq.push_back(inClusters[clx[i][j]][k]);

			//add the centroid of this cluster to the new centroid to be calculated
			//nCentroid.setX(nCentroid.getX()+inClusters[clx[i][j]][k].getX());
			//nCentroid.setY(nCentroid.getY()+inClusters[clx[i][j]][k].getY());
			//nCentroid.setZ(nCentroid.getZ()+inClusters[clx[i][j]][k].getZ());
		}
		//new centroid
		//nCentroid.setX(nCentroid.getX()/clx[i].size());
		//nCentroid.setY(nCentroid.getY()/clx[i].size());
		//nCentroid.setZ(nCentroid.getZ()/clx[i].size());

		//save new centroid
		//nClq.push_back (nCentroid);

		clxClusters.push_back (nClq);
	}

	clxAdjMtrx.resize (clx.size ());		//adjacency matrix for clx...neighbors are the indices of cliques not the initial clusters
	// build adjacency matrix for cliques
	for (i=0; i<clx.size (); i++){
		for (j=0; j<clx[i].size (); j++){
			for (l=0; l<adjMtrx[clx[i][j]].size (); l++){
				for (k=i+1; k<clx.size (); k++){
					for (m=0; m<clx[k].size (); m++){
						if ( clx[i][j] != clx[k][m] && adjMtrx[clx[i][j]][l] == clx[k][m]){
							clxAdjMtrx[i].push_back (k);
							clxAdjMtrx[k].push_back (i);
							break;
						}
					}
				}
			}
		}
	}
	//remove duplicates in clxAdjMtrx
	for (i=0; i<clxAdjMtrx.size (); i++){
		for (j=0; j<clxAdjMtrx[i].size (); j++){
			k=j+1;
			while (k<clxAdjMtrx[i].size ()){
				if (clxAdjMtrx[i][j] == clxAdjMtrx[i][k]){
					clxAdjMtrx[i].erase (clxAdjMtrx[i].begin () + k);
					k--;
				}
				k++;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Graph::buildAdjMtrx(vector<vector<Point> > &clusters, vector<vector<double> > &cDist, vector<vector<int> > &adjMtrx, double apix, int nClustersOriginal, double continuatyTHR){

	int i,k;
	int nClusters = clusters.size ();

	Point cPnt, nPnt;			//the two points we are measuring distance b/w...in MAP indexing system

	//
	//		find minimum distance b/w each pair of clusters
	//
	adjMtrx.clear ();
	adjMtrx.resize (nClusters);			//the size is : the initial clusters and the clusters for SSs' ends

	cout<<"Adjancy matrix size: "<<nClusters<<" original size: "<<nClustersOriginal<<endl;
	cout<<"Apix: "<<apix<<" continuatyTHR: "<<continuatyTHR<<endl;
	double dist, distTHR;
	bool cont;
	for (i=0; i<nClusters-1; i++){
		for (k= i+1; k<nClusters; k++){
			if (k >= nClustersOriginal)
				distTHR = 4.0*apix;//original value 3.0
			else
				distTHR = continuatyTHR;

			cont = true;
			for (int j=0; j<clusters[i].size (); j++){
				cPnt = clusters[i][j];
				for (int l=0; l<clusters[k].size (); l++){
					nPnt = clusters[k][l];
					//dist  = cPnt.dist(nPnt)/apix;
					dist  = cPnt.dist(nPnt);
					if (dist <= distTHR){
						adjMtrx[i].push_back (k);
						adjMtrx[k].push_back (i);
						cont = false;
						break;
					}
				}
				if (!cont)
					break;
			}
			//find the distance b/w centroids
			Point temp1, temp2;
			temp1 = getClusterCenter(clusters[i]);
			temp2 = getClusterCenter(clusters[k]);
			cDist[i][k] = cDist[k][i] = temp1.dist(temp2);
		}
	}

	//cout<<"Building adjacency matrix is done.."<<endl;

}

void Graph::build(string file, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	cout<<"Create graph with Kamal's TopoDP_May2012 output ..."<<endl;
	cout<<"Total "<<sequenceNodeContainer.size()<<" SSEs in sequence and "<<stickNodeContainer.size()/2<<" sticks in stick sequence"<<endl;
	cout<<"==========================="<<endl;

	//allocate nodes for graph
	for(int i = 0; i < sequenceNodeContainer.size(); i++)
	{
		vector<GraphNode> rowContainer;

		for(int j = 0; j < stickNodeContainer.size(); j++)
		{
			GraphNode temp;
			temp.set(sequenceNodeContainer, stickNodeContainer, i, j);
			rowContainer.push_back(temp);
		}

		array.push_back(rowContainer);
	}

	ifstream edgeFile(file.c_str());
	if(!edgeFile)
	{
		cout<<"Can not open "<<file<<" ..."<<endl;
		exit(1);
	}

	int aRow, aColumn, bRow, bColumn;
	double weight;
	string str;

	cout<<"Graph size is ("<<array.size()<<","<<array[0].size()<<")"<<endl;

	while(getline(edgeFile, str))
	{
		aRow = atof(str.substr(0,10).c_str());
		aColumn = atof(str.substr(10, 10).c_str());
		bRow = atof(str.substr(20, 10).c_str());
		bColumn = atof(str.substr(30, 10).c_str());
		weight = atof(str.substr(40, 10).c_str());
		//if(aRow >= sequenceNodeContainer.size()) continue;
		//if(aColumn >= stickNodeContainer.size()) continue;
		//if(bRow >= sequenceNodeContainer.size()) continue;
		//if(bColumn >= stickNodeContainer.size()) continue;
		//stringstream stream;
		//stream<<str;
		//stream>>aRow>>aColumn>>bRow>>bColumn>>weight;
		//cout<<aRow<<" "<<aColumn<<" "<<bRow<<" "<<bColumn<<" "<<weight<<endl;
		pair<int, int> aNode, bNode;
		aNode.first = aRow-1;
		aNode.second = aColumn;
		bNode.first = bRow-1;
		bNode.second = bColumn;
		//cout<<"Set up two node ..."<<endl;
		cout<<str<<endl;
		cout<<aNode.first<<","<<aNode.second<<" "<<bNode.first<<","<<bNode.second<<endl;
		//cout<<array[aNode.first][aNode.second].getType()<<" "<<array[bNode.first][bNode.second].getType()<<endl;
		//cout<<"Create edge between two nodes ..."<<endl;
		array[aNode.first][aNode.second].addOuter(bNode);
		array[aNode.first][aNode.second].addWeightOut(weight);
		array[bNode.first][bNode.second].addInner(aNode);
		array[bNode.first][bNode.second].addWeightIn(weight);
		//cout<<"Set up node ["<<aRow-1<<","<<aColumn<<"] and ["<<bRow-1<<","<<bColumn<<"]"<<endl;
	}

	showGraph("./other/graph.txt");
}

void Graph::type(const vector<pair<int, int> > &trueTopology)
{
	bool miss = false;
	cout<<"\n-------------True Topology---------------"<<endl;
	for(int i = 0; i < trueTopology.size(); i++)
		if(array[trueTopology[i].first][trueTopology[i].second].getType() == "N")
		{
			cout<<"["<<trueTopology[i].first<<","<<trueTopology[i].second<<"] not exist"<<endl;
			miss = true;
		}
	if(miss)
	{
		cout<<"-----------------------------------------"<<endl;
		exit(1);
	}
	else
	{
		cout<<"True topology is involved in graph ..."<<endl;
		cout<<"-----------------------------------------"<<endl;
	}
}

void Graph::build(const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	cout<<"Total "<<sequenceNodeContainer.size()<<" SSEs in sequence and "<<stickNodeContainer.size()/2<<" sticks in stick sequence"<<endl;
	cout<<"============================="<<endl;

	//allocate nodes for graph
	for(int i = 0; i < sequenceNodeContainer.size(); i++)
	{
		vector<GraphNode> rowContainer;

		for(int j = 0; j < stickNodeContainer.size(); j++)
		{
			GraphNode temp;
			temp.set(sequenceNodeContainer, stickNodeContainer, i, j);
			rowContainer.push_back(temp);
		}

		array.push_back(rowContainer);
	}

	//generate node information to ./other/node.txt
	showNode();

	int diff;//save N-M, N is number of sequence node, M is number of stick node without considering direction

	diff = sequenceNodeContainer.size() - stickNodeContainer.size()/2;

	for(int i = 0; i < sequenceNodeContainer.size()-1; i++)
	{
		for(int j = 0; j < stickNodeContainer.size(); j++)
		{
			for(int m = i+1; m <= i+1+diff; m++)
			{
				for(int n = 0; n < stickNodeContainer.size(); n++)
				{
					if(m < sequenceNodeContainer.size())//make m valid
					{
						if(array[i][j].getType() != "N" && array[m][n].getType() != "N")//both nodes have to be valid
						{
							if(!(array[i][j].getType() == array[m][n].getType() && array[i][j].getColumnIndex() == array[m][n].getColumnIndex()))//avoid take same stick with different direction
							{
								set(array[i][j], array[m][n], sequenceNodeContainer, stickNodeContainer);
							}
						}
					}
				}
			}
		}
	}

	showGraph("./other/graph.txt");//generate graph and saved in ./other/graph.txt
}

//set up weight for possible each edge
void Graph::set(GraphNode &a, GraphNode &b, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	double distStick, distSequence;

	distStick = stickDist(a, b, sequenceNodeContainer, stickNodeContainer);
	distSequence = sequenceDist(a, b, sequenceNodeContainer, stickNodeContainer);

	//cout<<"----------------"<<endl;
	//cout<<setw(10)<<a.getRow()<<setw(10)<<a.getColumn()<<setw(10)<<b.getRow()<<setw(10)<<b.getColumn()<<setw(10)<<distStick<<setw(10)<<distSequence<<endl;
	//cout<<"------------------"<<endl;

	if(distSequence < 0)//do not set up the edge if there an invalid loop length
		return;

	//cout<<distStick<<" "<<distSequence<<" | "<<distSequence-distStick<<endl;
	if((distStick - distSequence) <= 0)
	{
		//if(distStick < 0.001)
			//cout<<"distStick: "<<distStick<<" distSequence: "<<distSequence<<endl;
		pair<int, int> aNode, bNode;
		aNode.first = a.getRow();
		aNode.second = a.getColumn();
		bNode.first = b.getRow();
		bNode.second = b.getColumn();
		a.addOuter(bNode);//set outer
		//cout<<"["<<a.getRow()<<","<<a.getColumn()<<","<<a.getType()<<a.getColumnIndex()<<"] ["<<b.getRow()<<","<<b.getColumn()<<","<<b.getType()<<b.getColumnIndex()<<"] "<<"Dist: "<<distStick<<" Sequence: "<<distSequence<<" Diff: "<<distSequence-distStick<<endl;
		a.addWeightOut(distSequence-distStick);//set outer weight
		b.addInner(aNode);//set inner
		b.addWeightIn(distSequence-distStick);//set inner weight
	}
}

//get distance between two sticks
double Graph::stickDist(const GraphNode &a, const GraphNode &b, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	StickNode aTemp, bTemp;

	aTemp = stickNodeContainer[a.getColumn()];
	bTemp = stickNodeContainer[b.getColumn()];

	Point startPoint, endPoint;

	//direction
	//1, stick points has same order as the sequence
	//-1, stick points has negative order with the sequence
	if(aTemp.getDirection() > 0)
	{
		startPoint = aTemp.getEnd();
	}
	else
	{
		startPoint = aTemp.getStart();
	}

	if(bTemp.getDirection() > 0)
	{
		endPoint = bTemp.getStart();
	}
	else
	{
		endPoint = bTemp.getEnd();
	}

	return startPoint.dist(endPoint);
}

double Graph::nAADiffAngstrom(const GraphNode &a, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)//check the length difference by nAA between SSE length in sequence and the stick length with unit angstrom
{
	int lengthSeq;
	double lengthStick;

	lengthSeq = sequenceNodeContainer[a.getRow()].getLength();//get the sequence length by nAA
	lengthStick = stickNodeContainer[a.getColumn()].getLength();//get the stick length

	if(a.getType() == "H")//alpha helix
		return (lengthSeq*1.5 - lengthStick)/2;
	else if(a.getType() == "S")//beta sheet
		return (lengthSeq*3.5 - lengthStick)/2;

	return 0;
}

int Graph::nAADiff(const GraphNode &a, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)//check the length difference by nAA between SSE length in sequence and the stick length
{
	int lengthSeq, lengthStick;

	lengthSeq = sequenceNodeContainer[a.getRow()].getLength();//get the sequence length by nAA
	lengthStick = stickNodeContainer[a.getColumn()].getLength();//get the stick length

	int n;

	n = 0;

	if(a.getType() == "H")//alpha helix
		n = int((lengthSeq*1.5 - lengthStick)/2/1.5+0.5);
	else if(a.getType() == "S")//beta sheet
		n = int((lengthSeq*3.5 - lengthStick)/2/3.5+0.5);

	return n;
}

//get the distance by sequence
double Graph::sequenceDist(const GraphNode &a, const GraphNode &b, const vector<SequenceNode> &sequenceNodeContainer, const vector<StickNode> &stickNodeContainer)
{
	double distTemp = 0;
	vector<SequenceNode> tempContainer;

	//save the sequence node into container to get the segment between node a and node b (includeing a and b)
	//node a is before node b in sequence
	for(int i = a.getRow(); i <= b.getRow(); i++)
	{
		tempContainer.push_back(sequenceNodeContainer[i]);
	}

	//cout<<a.getRow()<<" "<<b.getRow()<<" "<<tempContainer.size()<<endl;

	//get loop length between node a and node b
	for(int i = 0; i < tempContainer.size()-1; i++)
	{
		int startTemp, endTemp;
		startTemp = tempContainer[i].getEnd();
		endTemp = tempContainer[i+1].getStart();
		//distTemp += (endTemp-startTemp+4)*3.8;
		int lengthTemp;
		lengthTemp = endTemp-startTemp;//if the SSE length in sequence equals to the stick length
		lengthTemp += 3;//add 3 for the loop to avoid the sequence length less than stick length from the error

		/*if(lengthTemp < 3)
			lengthTemp += 2;
		if(lengthTemp < 7)
			lengthTemp += 2;*/

		double distTempMed;
		distTempMed = lengthTemp*3.8+nAADiffAngstrom(a, sequenceNodeContainer, stickNodeContainer)+nAADiffAngstrom(b, sequenceNodeContainer, stickNodeContainer);//consider the length different between the SSE length in sequence and stick length


		//if(distTempMed < 0)//if the loop length is less than zero by considering the difference between the SSE length in sequence and stick length, return this negative to represent an invalid assignment was done, need ignore it
			//return distTempMed;

		distTemp += distTempMed;
	}

	//get helix length and sheet length if there are more SSEs between node a and node b in sequence
	if(tempContainer.size() > 2)
	{
		for(int i = 1; i < tempContainer.size()-1; i++)
		{
			int startTemp, endTemp;

			startTemp = tempContainer[i].getStart();
			endTemp = tempContainer[i].getEnd();

			if(tempContainer[i].getType() == "H")//helix takes nAA*1.5 angstrom
			{
				distTemp += (endTemp-startTemp)*1.5;
			}
			else if(tempContainer[i].getType() == "S")//sheet takes nAA*3.5 angstrom
			{
				distTemp += (endTemp-startTemp)*3.5;
			}
		}
	}

	//cout<<"["<<a.getRow()<<","<<a.getColumn()<<"] ["<<b.getRow()<<","<<b.getColumn()<<"] "<<distTemp<<endl;

	return distTemp;
}

void Graph::showTable() const
{
	int rowTemp, columnTemp;
	rowTemp = array.size();//get SSEs number in sequence
	columnTemp = array[0].size();//get column number

	ofstream file("./other/table.txt");
	if(!file)
	{
		cerr<<"Can not create ./other/table.txt"<<endl;
		exit(1);
	}

	cout<<"\nGenerate table information in ./other/table.txt ..."<<endl;

	for(int i = 0; i < rowTemp; i++)
	{
		int maxRecord = 0;//max record for node

		for(int j = 0; j < columnTemp; j++)
		{
			if(array[i][j].getRecordNum() > maxRecord)
				maxRecord = array[i][j].getRecordNum();
		}

		//cout<<"Max record: "<<maxRecord<<endl;

		int width = columnTemp*2;//path width, allocate 5 to f value and 7 to prev, plus one position for delimitor, total width is width+15 

		for(int j = 0; j < columnTemp; j++)
		{
			file<<setw(1)<<"("<<setw(3)<<array[i][j].getRow()<<setw(1)<<","<<setw(3)<<array[i][j].getColumn()<<setw(1)<<")"<<setw(1)<<array[i][j].getType()<<setw(1)<<"("<<setw(3)<<array[i][j].getRowIndex()<<setw(1)<<","<<setw(3)<<array[i][j].getColumnIndex()<<")";
			if(width+15 > 20)
				file<<setw(width+15-19)<<"|";
			else
				file<<"|";
		}
		file<<endl;

		for(int j = 0; j < maxRecord; j++)
		{
			for(int m = 0; m < columnTemp; m++)
			{
				vector<Record> recordContainerTemp;
				recordContainerTemp = array[i][m].getContainer();

				if(j < recordContainerTemp.size())
				{
					Record outputRecord;
					outputRecord = recordContainerTemp[j];
					Set setTemp;
					setTemp = outputRecord.getPath();
					//vector<int> setContainer;
					string setContainer;
					//setContainer = setTemp.getContainer();
					setContainer = setTemp.dec2bin();
					double fTemp;
					fTemp = outputRecord.getF();
					pair<int, int> prev;
					prev = outputRecord.getPrev();

					stringstream stream;
					stream<<"{";
					/*for(int k = 0; k < setContainer.size(); k++)
					{
						stream<<setw(3)<<setContainer[k]<<setw(1)<<" ";
					}*/
					stream<<setContainer;
					stream<<"}";

					file<<setw(width+2)<<stream.str()<<setw(5)<<setprecision(1)<<fixed<<fTemp<<setw(1)<<"["<<setw(2)<<prev.first<<setw(1)<<","<<setw(2)<<prev.second<<setw(1)<<"]"<<setw(1)<<"|";

				}
				else
					file<<setw(width+15)<<"|";
			}
			file<<endl;
		}

		for(int j = 0; j < columnTemp; j++)
		{
			if(width+15 > 20)
				file<<setw(width+15)<<"-----------------";
			else
				file<<setw(20)<<"--------------";
		}
		file<<endl;
	}
}

void Graph::showTrace() const
{
	int rowTemp, columnTemp;
	rowTemp = array.size();
	columnTemp = array[0].size();

	ofstream file("./other/trace.txt");
	if(!file)
	{
		cerr<<"Can not create ./other/trace.txt"<<endl;
		exit(1);
	}

	cout<<"\nGenerate trace pair information in ./other/trace.txt ..."<<endl;

	for(int i = 0; i < rowTemp; i++)
	{
		int maxOuter = 0;

		for(int j = 0; j < columnTemp; j++)
		{
			if(array[i][j].getOuterNum() > maxOuter)
				maxOuter = array[i][j].getOuterNum();
		}

		//cout<<"Row: "<<i<<" maxOuter: "<<maxOuter<<endl;

		for(int j = 0; j < columnTemp; j++)
		{
			file<<setw(1)<<"("<<setw(3)<<array[i][j].getRow()<<setw(1)<<","<<setw(3)<<array[i][j].getColumn()<<setw(1)<<")"<<setw(1)<<array[i][j].getType()<<setw(1)<<"("<<setw(3)<<array[i][j].getRowIndex()<<setw(1)<<","<<setw(3)<<array[i][j].getColumnIndex()<<")|";
		}
		file<<endl;

		for(int j = 0; j < maxOuter; j++)
		{
			for(int m = 0; m < columnTemp; m++)
			{
				vector<pair<int, int> > outerTemp;
				vector<pair<int, int> > tracePairTemp;

				outerTemp = array[i][m].getOuter();
				tracePairTemp = array[i][m].getTracePair();
				if(j < outerTemp.size())
				{
					file<<setw(1)<<"("<<setw(3)<<outerTemp[j].first<<setw(1)<<","<<setw(3)<<outerTemp[j].second<<setw(1)<<")"<<setw(2)<<"= "<<setw(3)<<tracePairTemp[j].first<<"->"<<setw(3)<<tracePairTemp[j].second<<setw(1)<<"|";
				}
				else
				{
					file<<setw(19)<<""<<setw(1)<<"|";
				}
			}
			file<<endl;
		}

		for(int j = 0; j < columnTemp; j++)
			file<<setw(20)<<"--------------";
		file<<endl;
	}
}

void Graph::showGraph(string name) const
{
	int rowTemp, columnTemp;
	rowTemp = array.size();
	columnTemp = array[0].size();

	ofstream file(name.c_str());
	if(!file)
	{
		cerr<<"Can not create ./other/graph.txt"<<endl;
		exit(1);
	}

	cout<<"\nGenerate graph information in ./other/graph.txt ..."<<endl;

	for(int i = 0; i < rowTemp; i++)
	{
		int maxOuter = 0;

		for(int j = 0; j < columnTemp; j++)
		{
			if(array[i][j].getOuterNum() > maxOuter)
				maxOuter = array[i][j].getOuterNum();
		}

		//cout<<"Row: "<<i<<" maxOuter: "<<maxOuter<<endl;

		for(int j = 0; j < columnTemp; j++)
		{
			file<<setw(1)<<"("<<setw(3)<<array[i][j].getRow()<<setw(1)<<","<<setw(3)<<array[i][j].getColumn()<<setw(1)<<")"<<setw(1)<<array[i][j].getType()<<setw(1)<<"("<<setw(3)<<array[i][j].getRowIndex()<<setw(1)<<","<<setw(3)<<array[i][j].getColumnIndex()<<")|";
		}
		file<<endl;

		for(int j = 0; j < maxOuter; j++)
		{
			for(int m = 0; m < columnTemp; m++)
			{
				vector<pair<int, int> > outerTemp;
				vector<double> weightOutTemp;

				outerTemp = array[i][m].getOuter();
				weightOutTemp = array[i][m].getOutWeight();
				if(j < outerTemp.size())
				{
					file<<setw(1)<<"("<<setw(3)<<outerTemp[j].first<<setw(1)<<","<<setw(3)<<outerTemp[j].second<<setw(1)<<")"<<setw(2)<<"= "<<setw(8)<<weightOutTemp[j]<<setw(1)<<"|";
				}
				else
				{
					file<<setw(19)<<""<<setw(1)<<"|";
				}
			}
			file<<endl;
		}

		for(int j = 0; j < columnTemp; j++)
			file<<setw(20)<<"--------------";
		file<<endl;
	}
}

void Graph::showNode() const
{
	int rowTemp, columnTemp;
	rowTemp = array.size();
	columnTemp = array[0].size();

	ofstream file("./other/node.txt");
	if(!file)
	{
		cerr<<"Can not create ./other/node.txt"<<endl;
		exit(1);
	}

	cout<<"\nGenerate node information in ./other/node.txt ..."<<endl;
	cout<<"Formate:"<<endl;
	cout<<setw(30)<<"(Row, Column)"<<endl;
	cout<<setw(30)<<"Type"<<endl;
	cout<<setw(30)<<"(RowIndex, ColumnIndex)"<<endl;
	cout<<setw(30)<<"----------------"<<endl;

	for(int i = 0; i < rowTemp; i++)
	{
		for(int j = 0; j < columnTemp; j++)
		{
			stringstream stream;
			stream<<"("<<i<<","<<j<<")";
			file<<setw(15)<<stream.str();
		}
		file<<endl;

		for(int j = 0; j < columnTemp; j++)
		{
			stringstream stream;
			stream<<array[i][j].getType();
			file<<setw(15)<<stream.str();
		}
		file<<endl;

		for(int j = 0; j < columnTemp; j++)
		{
			stringstream stream;
			stream<<"("<<array[i][j].getRowIndex()<<","<<array[i][j].getColumnIndex()<<")";
			file<<setw(15)<<stream.str();
		}
		file<<endl;

		file<<setw(15)<<"------------"<<endl;
	}

	file.close();
}

//save path for topology
void Graph::savePath(const vector<Point> &cliqueCenterContainer, const vector<vector<int> > &tracePaths, const vector<pair<int, int> > &topology, string dir)
{
	stringstream streamTemp;
	streamTemp<<"mkdir "<<dir;
	system(streamTemp.str().c_str());

	for(int i = 0; i < topology.size()-1; i++)
	{
		pair<int, int> startNode, endNode;
		startNode = topology[i];
		endNode = topology[i+1];

		vector<pair<int, int> > outerContainer;
		outerContainer = array[startNode.first][startNode.second].getOuter();

		int indexTemp = -1;

		for(int j = 0; j < outerContainer.size(); j++)
		{
			if(endNode.first == outerContainer[j].first && endNode.second == outerContainer[j].second)
			{
				indexTemp = j;
				break;
			}
		}

		if(indexTemp == -1)
			cerr<<"Miss trace paths is found in "<<dir<<" ..."<<endl;
		else
		{
			pair<int, int> tracePairTemp;
			tracePairTemp = array[startNode.first][startNode.second].getTracePair(indexTemp);
			if(tracePairTemp.first< 0)//no trace path between the start node and the end node
				continue;
			else if(tracePairTemp.first == tracePairTemp.second)//trace that cover both stick side points
			{
				vector<int> tracePathTemp;
				tracePathTemp = tracePaths[tracePairTemp.first];
				vector<Point> tracePointContainer;
				for(int k = 0; k < tracePathTemp.size(); k++)
					tracePointContainer.push_back(cliqueCenterContainer[tracePathTemp[k]]);
				stringstream stream;
				stream<<dir<<"/tracePath"<<i<<".pdb";
				savePoint(tracePointContainer, stream.str());
			}
			else if(tracePairTemp.first != tracePairTemp.second)//path combined with two traces
			{
				vector<int> tracePathTemp1, tracePathTemp2;
				tracePathTemp1 = tracePaths[tracePairTemp.first];
				tracePathTemp2 = tracePaths[tracePairTemp.second];
				vector<Point> tracePointContainer;
				for(int k = 0; k < tracePathTemp1.size(); k++)
					tracePointContainer.push_back(cliqueCenterContainer[tracePathTemp1[k]]);
				for(int k = tracePathTemp2.size()-1; k >= 0; k--)
					tracePointContainer.push_back(cliqueCenterContainer[tracePathTemp2[k]]);
				stringstream stream;
				stream<<dir<<"/tracePath"<<i<<".pdb";
				savePoint(tracePointContainer, stream.str());
			}
		}
	}
}
