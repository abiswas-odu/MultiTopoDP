#include <iostream>
#include <cstdlib>
#include <vector>
#include <sys/stat.h>
#include "MRC.h"
#include "dirFile.h"
#include "SequenceNode.h"
#include "StickNode.h"
#include "Ultility.h"
#include "Graph.h"
using namespace std;

int main(int argc, char *argv[])
{
	//create ./other to save output
	if(argc != 3)
	{
		cout<<"Usage:   topoDP <parameter file> <true topology file>"<<endl;
		exit(1);
	}

	
	mode_t mode = 0755;

	DFCDirectory("./other", mode);

	system("rm -rf ./other/*");

	ifstream parameterFile;
	parameterFile.exceptions(ifstream::failbit);

	try{
		parameterFile.open(argv[1]);
	}
	catch(ifstream::failure e)
	{
		cerr<<__FILE__<<"/"<<__LINE__<<": "<<"Can not open parameter file, "<<e.what()<<endl;
		exit(1);
	}

	string sequenceFile, mrcFile, helixFile, sheetFile;
	string str;
	double gap, penalty, secondaryPenalty;

	int K;//save k value, which is the number of topologies that program will generate
	

	parameterFile>>str>>sequenceFile;
	if(str != "Sequence") //sequence information is required
	{
		cout<<"Sequence parameter not found in parameter file."<<endl;
		exit(1);
	}
	parameterFile>>str>>helixFile;
	if(str != "Helix")//helix information is optional, set up this term to be "-" if no helix file exist
	{
		cout<<"Helix sticks parameter not found in parameter file. Set is to "-" if no helix sticks exist."<<endl;
		exit(1);
	}
	parameterFile>>str>>sheetFile;
	if(str != "Sheet")//sheet information is optional, set up this term to be "-" if no sheet file exist
	{
		cout<<"Sheet sticks parameter not found in parameter file. Set is to "-" if no sheet sticks exist."<<endl;
		exit(1);
	}
	parameterFile>>str>>mrcFile;
	if(str != "MRCFile")//skeleton file
	{
		cout<<"MRC file not found in parameter file."<<endl;
		exit(1);
	}
	parameterFile>>str>>K;
	if(str != "K")//K information is required
	{
		cout<<"Top-K parameter  not found in parameter file."<<endl;
		exit(1);
	}
	parameterFile>>str>>gap;
	if(str != "Gap")//Gap information is required
	{
		cout<<"Gap parameter  not found in parameter file."<<endl;
		exit(1);
	}
	parameterFile>>str>>penalty;
	if(str != "Penalty")//Penalty information is required
	{
		cout<<"Penalty parameter  not found in parameter file."<<endl;
		exit(1);
	}
	parameterFile>>str>>secondaryPenalty;
	if(str != "SecondaryPenalty")//Secondary Penalty information is required
	{
		cout<<"Secondary Penalty parameter  not found in parameter file."<<endl;
		exit(1);
	}


	parameterFile.close();

	//read sequence information
	cout<<"\nRead sequence ..."<<endl;
	vector<SequenceNode> sequenceNodeContainer;

	readSequence(sequenceNodeContainer, sequenceFile);

	showSequence(sequenceNodeContainer);

	//read sticks information
	cout<<"\nRead sticks ..."<<endl;
	vector<StickNode> stickNodeContainer;

	readSticks(stickNodeContainer, helixFile, "H");
	readSticks(stickNodeContainer, sheetFile, "S");

	showSticks(stickNodeContainer);

	//check if N > M, N is SSEs number in sequence, M is SSEs number in sticks
	if(sequenceNodeContainer.size() < stickNodeContainer.size()/2)
	{
		cerr<<"Algorithm limits N > M, please check input file ..."<<endl;
		exit(1);
	}

	//generate pdb file for each stick
	cout<<"Generate pdb files for each stick and saved in ./other ..."<<endl;
	for(int i = 0; i < stickNodeContainer.size(); i = i+2)
	{
		stickNodeContainer[i].pdb();
	}

	//read skeleton of the density map
	MRC mrc;
	mrc.read(mrcFile);//read mrc file
	mrc.pdb("./other/density.pdb");//output voxel point in the skeleton with pdb format

	//read true topology
	cout<<"\nRead true topology..."<<endl;
	vector<pair<int, int> > trueTopology;
	string trueFileName = argv[2];
	trueTopology = getTrueTopology(trueFileName);

	//show SSE length
	showSSELength(sequenceNodeContainer, stickNodeContainer, trueTopology);

	//read output parameter
	int traceOutput=1, topologyTraceOutput=1;

	//build graph
	Graph graph(trueFileName);

	//set up the edge and corresponding weight
	cout<<"\nBuild graph and set weight for each edge ..."<<endl;
	//build graph by reading Kamal's weight
	//graph.build("./input/weightList.txt", sequenceNodeContainer, stickNodeContainer);
	//build graph by naive difference between sequence length and stick side-point distance
	graph.build(sequenceNodeContainer, stickNodeContainer);
	graph.type(trueTopology);//check if the true topology node is set up as invalid
	//build graph by considering the loop alongs skeleton voxel trace
	vector<Point> cliqueCenterContainer;
	vector<vector<int> > tracePaths;
	graph.buildUpdate(cliqueCenterContainer, tracePaths, mrc, sequenceNodeContainer, stickNodeContainer, traceOutput,gap,penalty,secondaryPenalty);
	//update edge weight which includes beta strands
	graph.betaSheet(sequenceNodeContainer, stickNodeContainer);

	//update table information
	cout<<"\nGenerate table for each node of graph ..."<<endl;
	graph.table(sequenceNodeContainer, stickNodeContainer);

	//trace the minimum path
	cout<<"Trace the path of minimum paths ..."<<endl;
	graph.trace(sequenceNodeContainer, stickNodeContainer);

	cout<<"Trace the top K minimum paths ..."<<endl;

	cout<<"\nGenerate top K topologies ...\n"<<endl;
	//generate top K minimum traces
	vector<vector<pair<int, int> > > traceContainer;
	vector<double> traceWeightContainer;
	traceContainer = graph.topK(K, sequenceNodeContainer, stickNodeContainer, traceWeightContainer);

	//adjust the weight of each trace for protein including beta sheet
	//sheetAdjust(traceContainer, traceWeightContainer, sequenceNodeContainer);

	//insertionSort(traceWeightContainer, traceContainer);//sort traces by corresponding weight

	cout<<"\nGet top "<<traceContainer.size()<<" traces and top "<<traceWeightContainer.size()<<" weight ..."<<endl;

	//output top K topology
	cout<<"\nSave top "<<K<<" to ./other/rank.txt ..."<<endl;
	saveTopology(traceContainer, traceWeightContainer, trueTopology, "./other/rank.txt");

	if(topologyTraceOutput)
	{
		system("mkdir ./other/TopologyPath");

		cout<<"\nSave trace path for true topology ..."<<endl;

		//show the clique point index in each path
		/*for(int i = 0; i < tracePaths.size(); i++)
		{
			for(int j = 0; j < tracePaths[i].size(); j++)
			{
				cout<<tracePaths[i][j]<<" ";
			}
			cout<<endl;
		}*/

		graph.savePath(cliqueCenterContainer, tracePaths, trueTopology, "./other/TopologyPath/True");

		//save trace path for top K topologies
		cout<<"\nSave trace path for top "<<K<<" topologies ..."<<endl;
		for(int i = 0; i < traceContainer.size(); i++)
		{
			vector<pair<int, int> > traceTemp;
			traceTemp = reverseTopology(traceContainer[i]);
			stringstream stream;
			stream<<"./other/TopologyPath/Top"<<i+1;
			graph.savePath(cliqueCenterContainer, tracePaths, traceTemp, stream.str());//debug, replace "trueTopology" with "traceTemp", 07/27/2013
		}
	}

	//cout<<"\nGenerate trace between SSEs ..."<<endl;
	//get the traces points for both SSEs and loops
	//getTrace(traceContainer[0]);

	cout<<"\nHave done reading traces ..."<<endl;

	return 0;
}
