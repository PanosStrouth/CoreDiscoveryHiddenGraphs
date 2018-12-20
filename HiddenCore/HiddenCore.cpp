#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <unordered_map>
#include <queue>
#include <set>
#include <functional>
#include <algorithm>
#include <math.h>
#include <cstdlib>
using namespace std;

int NumberOfNodes = 0;
bool* inTOP = NULL;
int* state  = NULL;
int* empty = NULL;
int* solid  = NULL;
std::vector<int>* neighbors = NULL;
int numOfSegments = 0;
int numberOfNodesInTOP = 0;
int MaxPotentialDegreeInG = 0;
int* maxPotentialDegrees;
int NumOfPetentialCoreNodes;
int probings = 0;

template <typename T>
struct Segment {
  unsigned long counter;
  std::list<T> e;
  std::list<T> s;
  int startIndex;
  int endIndex;
  Segment()
  {
	counter = 0;
	startIndex = 0;
	endIndex = 0;
  }
};

Segment<int>** segments = NULL;

void fileToGraph(const char* inputFile ,int& NumberOfNodes)
{
	std::ifstream infile_a(inputFile);
	std::ifstream infile_b(inputFile);
	if(!infile_a || !infile_b)
	{
		cout<<"could not open file\n";
		exit;
	}
	else
	{
		std::string line;
		int min = 1;
		int max = 1;

		while (std::getline(infile_a, line))
		{
			std::istringstream iss(line);
			int fromNode,toNode;
			if (!(iss >> fromNode >> toNode)) { break; } // error

			if(fromNode<=min)
				min = fromNode;

			if(toNode<=min)
				min = toNode;

			if(fromNode>=max)
				max = fromNode;

			if(toNode>=max)
				max = toNode;
		}
		NumberOfNodes = max-min+1;

		neighbors = new std::vector<int>[NumberOfNodes];

		while (std::getline(infile_b, line))
		{
			std::istringstream iss(line);
			int fromNode,toNode;
			if (!(iss >> fromNode >> toNode)) { break; } // error

			neighbors[fromNode].push_back(toNode);
		}
	}
}


void allocateSegments(int node)
{
	numOfSegments = log10(NumberOfNodes);

	if(numOfSegments<=1)
	{
		numOfSegments = 1;
		segments[node]=new Segment<int>[1];
		segments[node][0].counter=NumberOfNodes-1;
		segments[node][0].endIndex=NumberOfNodes-1;
		segments[node][0].startIndex = 0;
		return;
	}

	unsigned int segmentlength = NumberOfNodes/numOfSegments;
	unsigned int rest = NumberOfNodes-(segmentlength*numOfSegments);

	segments[node]=new Segment<int>[numOfSegments];

	int startIndex = 0;
	int endIndex = 0;
	for(int i=0;i<(numOfSegments-1);i++)
	{
		startIndex= (i*segmentlength);
		endIndex = (startIndex+segmentlength-1);
		segments[node][i].counter=segmentlength;
		segments[node][i].startIndex=startIndex;
		segments[node][i].endIndex=endIndex;

		if(node>=startIndex && node<=endIndex)
			segments[node][i].counter--;
	}

	segments[node][numOfSegments-1].counter=segmentlength+rest;
	segments[node][numOfSegments-1].startIndex=(numOfSegments-1)*segmentlength;
	segments[node][numOfSegments-1].endIndex=(numOfSegments*segmentlength+rest-1);
	if(node>=segments[node][numOfSegments-1].startIndex && node<=segments[node][numOfSegments-1].endIndex)
		segments[node][numOfSegments-1].counter--;
}


void addProbedNode(int source, int dest, bool probeResult)
{
	int segmentToAdd = 0;
	for(int i = 0;i<numOfSegments;i++)
	{
		if((dest >= segments[source][i].startIndex) && (dest<= segments[source][i].endIndex))
		{
			segmentToAdd = i;
			break;
		}
	}

	segments[source][segmentToAdd].counter--;
	if(probeResult==false)
	{
		segments[source][segmentToAdd].e.push_back(dest);
	}
	else
	{
		segments[source][segmentToAdd].s.push_back(dest);
	}
}


void updateMaxPotentialDegree(int node)
{
	int maxpotentialDegreeForNode = NumberOfNodes-1-empty[node];
	//cout<<"maxpotentialDegreeForNode"<<maxpotentialDegreeForNode<<"\n";

	if(maxpotentialDegreeForNode >= MaxPotentialDegreeInG)
		MaxPotentialDegreeInG = maxpotentialDegreeForNode;

	//cout<<"MaxPotentialDegreeInG"<<MaxPotentialDegreeInG<<"\n";
}


void updateMaxPotentialDegrees(int node,bool result,int K)
{
	if(result==false)
	{
		if(maxPotentialDegrees[node]==1)
		{
			if((NumberOfNodes-1-empty[node]) < K)
			{
				cout<<"node "<<node<<" must be prunned\n";
				maxPotentialDegrees[node]=-1;
				NumOfPetentialCoreNodes--;
			}
		}
		if(NumOfPetentialCoreNodes <= K)
		{
			cout<<"Core "<<K<<" not found after "<<probings<<" probings!\n";
			exit(1);
		}
	}

}


int findDest(int source,int K)
{
	int segmentToSearch = -1;
	for(int i = 0;i<numOfSegments;i++)
	{
		if(segments[source][i].counter>0)
		{
			segmentToSearch = i;
			break;
		}
	}

	if(segmentToSearch==-1)
		return -2;

	int start=segments[source][segmentToSearch].startIndex;
	int end = segments[source][segmentToSearch].endIndex;

	bool *probings = new bool[NumberOfNodes];
	for(int i = 0; i<NumberOfNodes;i++)
	{
		probings[i] = false;
	}

	for(int i = 0; i<numOfSegments;i++)
	{
		for(std::list<int>::iterator it = segments[source][i].e.begin();
			it != segments[source][i].e.end(); ++it)
		{
			probings[(*it)]=true;
		}

		for(std::list<int>::iterator it = segments[source][i].s.begin();
			it != segments[source][i].s.end(); ++it)
		{
			probings[(*it)]=true;
		}
	}


	while(start<=end)
	{
		if(source==start)
		{
			start++;
			continue;
		}

		if((probings[start]==false))
		{
			delete[] probings;

			//This is the Improvement
			if(maxPotentialDegrees[start]==-1)//we don't care for nodes of degree smaller than K
			{
				addProbedNode(source,start,false);
				empty[source]++;
				cout<<"recursion\n";
				start=findDest(source,K);
				if(start==-2)
					return -1;
			}
			return start;
		}
		start++;
	}
	delete[] probings;
	return -1;
}


bool probe(int source,int dest)
{
	probings++;
	std::vector<int>::iterator it;
	it = std::find(neighbors[source].begin(),neighbors[source].end(),dest);
	if(it!=neighbors[source].end())
		return true;
	return false;



}

void increaseStatesByOne()
{
	for(int i=0;i<NumberOfNodes;i++)
		if(state[i]<0)
			state[i]++;
}


bool Batagelj(int **degrees,int n,std::vector<int>** graph,int **positions,std::vector<int>& core,int K)
{
	//printHiddenGraph();
	//int n = TOP.size();
	int* deg= new int[n+1];
	int md = 0;

	for(int i=1;i<=n;i++)
	{
		deg[i]=(*degrees)[i];
		if (deg[i]>=md)
			md=deg[i];
	}

	unsigned int* bin = new unsigned int [md+1];
	for(int i=0;i<=md;i++)
		bin[i] = 0;

	for(int i=1;i<=n;i++)
		bin[deg[i]]++; //at this point bin is just a histogram for degrees

	unsigned int start = 1;

	for(unsigned int d = 0; d<=md; d++)
	{
		unsigned int num = bin[d];
		bin[d] = start;
		start+=num;

	}//at this point bin has the correct values

	unsigned int *pos = new unsigned int [n+1];
	unsigned int *vert = new unsigned int [n+1];

	for(unsigned int v = 1; v<=n; v++)
	{
		pos[v]=bin[deg[v]];
		vert[pos[v]]=v;
		bin[deg[v]]++;
	}//at this point vert and pos arrays are initialized for the first time based on bin. A typical bin sorting
	 //nodes are sorted based on their degree inside vert...from the one smallest till highest degree.

	for(unsigned int d=md; d>=1; d--)
		bin[d]=bin[d-1];

	bin[0] = 1;//	These last 3 lines are to correct the impact of deg[v]++ command inside the last "for"
	for(int i = 1; i<=n; i++)
	{
		int v = vert[i];
		for(std::vector<int>::iterator u = (*graph)[v].begin();u!=(*graph)[v].end();++u)
		{
			if(deg[(*u)] > deg[v])
			{
					int du = deg[(*u)];
					int pu = pos[(*u)];
					int pw = bin[du];
					int w = vert[pw];
					if(w!=(*u))
					{
						pos[(*u)]=pw;
						pos[w] = pu;
						vert[pw]=(*u);
						vert[pu]=w;
					}
					bin[deg[(*u)]]++;
					deg[(*u)]--;
			}
		}
	}


	for(int i= 1;i<=n;i++)
	{
		if(deg[i]>=K)
			core.push_back((*positions)[i]);
	}

	if(core.empty())
		return false;
	else
		return true;


	delete[] pos;
	delete[] vert;
	delete[] deg;
	delete[] bin;

	exit(1);
}


void CoreFind(int K)
{
	//reconstruct Graph from TOP
	std::vector<int>* neighbors;

	int *positions = new int [numberOfNodesInTOP+1];
	int *nicknames = new int [NumberOfNodes];

	int j = 1;
	for(int i=0;i<NumberOfNodes;i++)
	{
		if(inTOP[i]==true)
		{
			positions[j]=i;
			nicknames[i]=j;
			j++;
		}
	}

	std::vector<int>*graph = new std::vector<int> [numberOfNodesInTOP+1];
	int *degrees = new int[numberOfNodesInTOP+1];

	for(int i=1;i<=numberOfNodesInTOP;i++)
	{
		int id = positions[i];
		degrees[i] = 0;

		for(int j = 0; j<numOfSegments;j++)
		{
			for(std::list<int>::iterator it = segments[id][j].s.begin();it!=segments[id][j].s.end();++it)
			{
				if(inTOP[*it]==true)
				{
					graph[i].push_back(nicknames[*it]);
					degrees[i]++;
				}
			}
		}
	}

	std::vector<int> core;
	if(Batagelj(&degrees,numberOfNodesInTOP,&graph,&positions,core,K))
	{
		cout<<"Core "<<K<<" found after "<<probings<<" probings \n";
		for(std::vector<int>::iterator it = core.begin();it!=core.end();++it)
		{
			cout<<(*it)<<" ";
		}
		cout<<"\n";
		core.clear();
	}
	else
	{
		cout<<"Core "<<K<<" not found after "<<probings<<" probings!\n";
	}
}

int nextSource(int prevSource,int K)
{
	int nextSource = prevSource+1;

	while(true)
	{
		if(numberOfNodesInTOP==NumberOfNodes)
		{
			cout<<"All nodes are in TOP\n";
			CoreFind(K);
			exit(1);
		}

		if(nextSource == 0) // a new round starts
		{
			if(MaxPotentialDegreeInG < K)
			{
				CoreFind(K);
				exit(1);
			}
			else
				MaxPotentialDegreeInG = 0;
		}

		//the maxPotentialDegrees[nextSource]==1 condition is added as an improvement
		if(nextSource<NumberOfNodes && inTOP[nextSource]==false && state[nextSource]==0)
		{
			//check if the new selected source node should be pushed to TOP//
			if(empty[nextSource]+solid[nextSource] == (NumberOfNodes - 1))
			{
				if(maxPotentialDegrees[nextSource]==1)
				{
					inTOP[nextSource]=true;
					numberOfNodesInTOP++;
					cout<<"node "<<nextSource<<" is pushed to TOP\n";
					nextSource++;
					continue;
				}
				if(maxPotentialDegrees[nextSource]==-1)
				{
					nextSource++;
					continue;
				}
			}
			else
				return nextSource;
		}

		if(nextSource==NumberOfNodes)
		{
			increaseStatesByOne();
			nextSource = 0;
			continue;
		}

		if(inTOP[nextSource]==true)
		{
			nextSource++;
			continue;
		}

		if(state[nextSource]<0)
		{
			int maxpotentialdegree = NumberOfNodes-1-empty[nextSource];
			if(maxpotentialdegree >= MaxPotentialDegreeInG)
				MaxPotentialDegreeInG = maxpotentialdegree;

			nextSource++;
			continue;
		}
	}
	return -1;
}


void HiddenCore(int K)
{
	cout<<"HiddenCore starts\n";
	int source = 0;

	while(source!=-2)
	{
		//find a dest with a potential Degree larger or equal to K
		int dest= findDest(source,K);
		if(dest==-2)//this should never happen\n";
		{
			cout<<"This should never happen! It happens only in case all probings have been performed\n";
			exit(1);
		}
		else if(dest==-1)//This happens if only source has performed all of its probings and a new source should be chosen
		{
			if(solid[source]+empty[source] == (NumberOfNodes - 1))//This check is redundant if everythink works properly
			{
			  updateMaxPotentialDegrees(source,false,K);
				source=nextSource(source,K);
				continue;
			}
		}
		else // valid dest value is returned
		{
			//check if while checking for dests the maximum possible source node's solid became less than K
			if((NumberOfNodes-1-empty[source])<K)
			{
			    updateMaxPotentialDegrees(source,false,K);
				source=nextSource(source,K);
				continue;
			}
		}
		cout<<"dest"<<dest<<"\n";

		bool probeResult = probe(source,dest);

		addProbedNode(source,dest,probeResult);
		addProbedNode(dest,source,probeResult);

    if(probeResult)
		{
			cout<<source<<"--->"<<dest<<"=solid\n";

			solid[source]++;
			solid[dest]++;
			cout<<"solid[source] "<<solid[source]<<"\n";
			cout<<"solid[dest] "<<solid[dest]<<"\n";

			if((solid[dest]+empty[dest]) == (NumberOfNodes - 1) && (state[dest]==0))
			{
				inTOP[dest]=true;
				numberOfNodesInTOP++;
				cout<<"Node "<<dest<<" is pushed to TOP\n";
			}
			cout<<solid[source]+empty[source]<<"\n";
			if(solid[source]+empty[source] == (NumberOfNodes - 1) && (state[source]==0))
			{
				inTOP[source]=true;
				numberOfNodesInTOP++;
				cout<<"Node "<<source<<" is pushed to TOP\n";
				source = nextSource(source,K);
			}
		}
		else
		{
			cout<<source<<"--->"<<dest<<"=empty\n";

			empty[source]++;
			empty[dest]++;
			state[source]--;
			state[dest]--;

			updateMaxPotentialDegree(source);
			updateMaxPotentialDegree(dest);
			updateMaxPotentialDegrees(source,false,K);
			updateMaxPotentialDegrees(dest,false,K);

			source = nextSource(source,K);
			cout<<"source"<<source<<"\n";
		}
	}
}


int main(int argc, char** argv)
{
	//initialization, Reading the Graph and find the number of Nodes in Graph
	const char * filename =argv[1];

  //READ K FROM TERMINAL
  int K = atoi(argv[2]);
  if( K == 0)
  {
    cout<<"Invalid or zero K given!\n";
    return 0;
  }

	//READ GRAPH FROM FILE
	fileToGraph(filename,NumberOfNodes);

	//INITIALIZE THE NESSECARY MATRIXES
	inTOP = new bool[NumberOfNodes];
	for(int i=0;i<NumberOfNodes;i++)
		inTOP[i]=false;

	empty = new int[NumberOfNodes];
	for(int i=0;i<NumberOfNodes;i++)
		empty[i]=0;

	solid = new int[NumberOfNodes];
	for(int i=0;i<NumberOfNodes;i++)
		solid[i]=0;

	state = new int[NumberOfNodes];
	for(int i=0;i<NumberOfNodes;i++)
		state[i]=0;

	segments = new Segment<int>*[NumberOfNodes];
	for(int i=0;i<NumberOfNodes;i++)
	{
		allocateSegments(i);
	}

	maxPotentialDegrees = new int [NumberOfNodes];

	for(int i=0;i<NumberOfNodes;i++)
	{
		maxPotentialDegrees[i]=1;
	}
	NumOfPetentialCoreNodes=NumberOfNodes;

	int maximum_probes = ((NumberOfNodes-1)*NumberOfNodes)/2;

	HiddenCore(K);

	delete[] empty;
	delete[] solid;
	delete[] state;
	delete[] segments;
	delete[] inTOP;

	return 0;
}
