
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



struct Node{
	unsigned long id;
	char* probings;
	std::vector<Node> neighbors;
	std::vector<Node> neighborsInTop;
	int state;
	unsigned long degree;
	unsigned long empty;
	unsigned long nextNodeToProbe;
	unsigned long positionInG;
	bool inTop;
	bool inG;
	Node()
	{
		id=0;
		positionInG=0;
		state=0;
		degree=0;
		empty = 0;
		inTop = false;
		inG = true;
		if(this->id==0)
			nextNodeToProbe = 1;
		else
			nextNodeToProbe = 0;
	}


	~Node()
	{
		//delete probings;
		neighbors.clear();
		neighborsInTop.clear();
	}

	void initialize(unsigned long id, unsigned long numberOfNodes)
	{
		this->id=id;
		this->positionInG=id;
		this->probings=new char[numberOfNodes];
		for(unsigned long i = 0;i<numberOfNodes;i++)
			this->probings[i]=' ';
	}


	void insert(unsigned long dest,char probeResult)
	{
		this->probings[dest]=probeResult;
	}

	void update(unsigned long dest,char probeResult)
	{
		this->probings[dest]=probeResult;

		if(probeResult=='s')
		{
			this->degree++;
			cout<<"Node "<<id<<" has degree "<<degree<<"\n";
		}
		else
		{
			this->empty++;
			cout<<"Node "<<id<<" has empty "<<empty<<"\n";
		}
	}

	void decreaseState()
	{
		this->state--;
		cout<<"Node "<<id<<" goes to state "<<this->state<<"\n";
	}

	void increaseState()
	{
		this->state++;
	}

	bool hasNonNegativeState()
	{
		if(this->state<0)
		{
			//cout<<"Node "<<id<<" has state "<<state<<"......skipping this node\n";
			return false;
		}
		cout<<"Node "<<id<<" has 0 state\n";
		return true;
	}

	void setNextNodeToProbe(unsigned long index)
	{
		this->nextNodeToProbe=index;
		if(this->id == this->nextNodeToProbe)
		{
			cout<<"False next node for probing given!\n";
			exit(0);
		}
	}

	bool getNextNodeToProbe(unsigned long& nextIndex,unsigned long numberOfNodes)
	{
		//return the first index that has a non 'e' nor 's' value
		for(unsigned long i = 0;i<numberOfNodes;i++)
		{
			if(this->id==i)
				continue;

			if(this->probings[i]==' ')
			{
				nextIndex= i;
				return true;
			}
		}
		return false;
	}

	bool isDone(unsigned long numberOfNodes)
	{
		if(this->empty+this->degree==(numberOfNodes-1))
		{
			cout<<"Node "<<id<<" is done!!\n";
			return true;
		}
		else
		{
			cout<<"Node "<<id<<" is still not done!!\n";
			return false;
		}
	}

	void addNeighbor(Node x)
	{
		this->neighbors.push_back(x);
		cout<<"Node "<<id<<" added "<<x.id<<" as a new neighbor\n";
	}
};


unsigned int max_potential_solid;
std::vector<Node> top;
std::vector<Node> g;
unsigned long probings = 0;
unsigned long minDegreeInTop;
unsigned long maxDegreeInG;
unsigned long minEmptyInG;
unsigned int NumberOfNodes = 0;
unsigned int NumberOfNodesInTop = 0;
unsigned int K = 0;

void printG(std::vector<Node>& g)
{
	for(std::vector<Node>::iterator it=g.begin();it!=g.end();++it)
		cout<<(*it).id<<"\n";
}

bool operator==(const Node& a,const Node& b)
{
	if(a.id==b.id)
		return true;
	return false;
}

unsigned long numberOfNeighborsInTop(Node& node)
{
	unsigned long neighborsInTop=0;
	cout<<"Checking for node's "<<node.id<<" neighbors in Top\n";
	for(std::vector<Node>::iterator it=node.neighbors.begin();it!=node.neighbors.end();++it)
	{
		cout<<"Neighbor "<<(*it).id<<"\n";
		if((*it).inTop)
		{
			node.neighborsInTop.push_back(*it);
			cout<<"Neighbor in top found: "<<(*it).id<<"\n";
			neighborsInTop++;
		}
	}
	cout<<neighborsInTop<<" neighbors found in top for node "<<node.id<<"\n";
	return neighborsInTop;
}


void updateMinDegreeInTop(Node newNode)//call this function after every push back to TOP list
{
	unsigned long newDegree = newNode.degree;
	if(minDegreeInTop==NumberOfNodes)//this means that newNode is the first element to be inserted in G.
	{
		minDegreeInTop=newDegree;
		cout<<"first min degree in Top is: "<<minDegreeInTop<<"\n";
	}
	else
	{
		if(newDegree<minDegreeInTop)
		{
			cout<<"New node with degree: "<<newDegree<<" inserted...previous min was: "<<minDegreeInTop<<"\n";
			minDegreeInTop=newDegree;

		}
		else if (newDegree>minDegreeInTop)
		{
			cout<<"Something wrong happened\n";
			exit(1);
		}
		else
			cout<<"a new node "<<newNode.id<<" with same degree will be pushed to top\n";
	}
}

void updateMaxDegreeInG()//call this function after every probing but for non done nodes!
{
	maxDegreeInG = 0;
	for(std::vector<Node>::iterator it = g.begin();it!=g.end();it++)
	{
		if((*it).inG)
		{
			if((*it).degree>maxDegreeInG)
			{
				maxDegreeInG=(*it).degree;
				minEmptyInG=(*it).empty;
			}
		}
	}

	cout<<"Max degree in G is: "<<maxDegreeInG<<"\n";
	cout<<"Min empty in G is: "<<minEmptyInG<<"\n";

	if(minDegreeInTop==NumberOfNodes)//Do not trigger max core check the first time
		return;

	unsigned long max_potential_in_G=maxDegreeInG+(NumberOfNodes-1-(maxDegreeInG+minEmptyInG));
	cout<<"Max potential is "<<max_potential_in_G<<"\n";
}


struct Edge{
	unsigned long source;
	unsigned long dest;
	Edge(unsigned long source,unsigned long dest)
	{
		this->source = source;
		this->dest = dest;
	}


};
inline bool operator==(const Edge& lhs, const Edge& rhs){ return ((lhs.source==rhs.source)&&(lhs.dest==rhs.dest)); }

char probe(unsigned long source,unsigned long dest,std::vector<Edge>&Graph)
{
	cout<<"probing node "<<source<<" to node "<<dest<<"\n";
	Edge edge(source,dest);
	probings++;
	if(std::find(Graph.begin(),Graph.end(),edge)!=Graph.end())
		return 's';
	return 'e';
}

void fileToGraph(const char* inputFile ,unsigned int& NumberOfNodes,std::vector<Edge>&Graph)
{
	std::ifstream infile(inputFile);
	if(!infile)
	{
		cout<<"could not open file\n";
		exit;
	}
	else
	{
		std::string line;
		unsigned long min = 1;
		unsigned long max = 1;

		while (std::getline(infile, line))
		{
			std::istringstream iss(line);
			unsigned long fromNode,toNode;
			if (!(iss >> fromNode >> toNode)) { break; } // error

			Edge edge(fromNode,toNode);
			Graph.push_back(edge);

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
	}
}

void updateStates(std::vector<Node>& g)
{
	for(std::vector<Node>::iterator it=g.begin();it!=g.end();++it)
	{
		if((*it).inG)
			(*it).increaseState();
	}
}

bool HasNodesToProbe(std::vector<Node>& vec)
{
	unsigned long counterOfNodeNotInTop = 0;
	std::vector<Node>::iterator jt;
	for(std::vector<Node>::iterator it=vec.begin();it!=vec.end();++it)
	{
		if((*it).inG)
		{
			counterOfNodeNotInTop++;
			jt=it;
		}
	}
	if(counterOfNodeNotInTop==1)
	{
		top.push_back(*jt);
		(*jt).inG=false;
		(*jt).inTop=true;
		return false;
	}
	else if(counterOfNodeNotInTop<=0)
		return false;
	else
		return true;
}


Node& CheckIfSuitableSource(std::vector<Node>::iterator source_it,std::vector<Node>& g,std::vector<Node>& top,unsigned long numberOfNodes)
{
	while(true)
	{
		if(!HasNodesToProbe(g))
		{
			cout<<"All nodes sorted by their degree!\n";
			printG(top);
			cout<<probings<<" probings made\n";
			exit(0);
		}

		if((*source_it).inTop)
		{
			if((*source_it).id!=(*(g.end()-1)).id)//if source_it candidate is not the last element
				source_it++;
			else
			{
				updateStates(g);
				source_it=g.begin();
			}

			continue;
		}


		if((*source_it).hasNonNegativeState()) //check if source is Done and with non negative state any more
		{
			if((*source_it).isDone(numberOfNodes))
			{
				top.push_back((*source_it));
				cout<<"Node "<<(*source_it).id<<" pushed to top and removed from g\n";
				updateMinDegreeInTop((*source_it));
				(*source_it).inG=false;
				(*source_it).inTop=true;
				source_it++;
				updateMaxDegreeInG();
				NumberOfNodesInTop++;
        if(NumberOfNodesInTop == K)
        {
					cout<<"\n\n\n==========Results==========\nHere's the top-"<<K<<" list:\n";
					printG(top);
					cout<<"Top-"<<K<<" nodes found after "<<probings<<" probings\n";
					cout<<"Maximum number of probings: "<<(NumberOfNodes*(NumberOfNodes-1))/2<<"\n";
					exit(0);
				}
			}
			else
				break;
		}
		else //try to find the first zero state source node
		{
			if(source_it==(g.end()-1))
			{
					cout<<"==================================Round finished-STATS====================================\n";
					updateStates(g);
					source_it=g.begin();
			}
			else
				source_it++;
		}
	}
	return (*source_it);
}


Node& getNodeById(std::vector<Node>& g,unsigned long Id)
{
    unsigned long positionInG=-1;

	for(std::vector<Node>::iterator it = g.begin();it!=g.end();++it)
	{
		positionInG++;
		if((*it).id==Id && (*it).inG)
		{
			(*it).positionInG=positionInG;
			return (*it);
		}
	}
}

void gsoe(std::vector<Node>& g,std::vector<Node>& top,std::vector<Edge>&Graph)
{

	unsigned long numberOfNodes = g.size();

	if(numberOfNodes<=0)
		exit;

	std::vector<Node>::iterator source_it = g.begin();

	while(true)
	{
		Node& source = CheckIfSuitableSource(source_it,g,top,numberOfNodes);
		cout<<"Source node changed to:"<<source.id<<"\n";
		bool soe=false;

		while(!soe)
		{
			unsigned long nextNodeId;

			if(!source.getNextNodeToProbe(nextNodeId,numberOfNodes))
				return;

			Node& dest = getNodeById(g,nextNodeId);

			cout<<"Next destination node for probing is "<<dest.id<<"\n";
			if(probe(source.id,dest.id,Graph)=='s')
			{
				cout<<"probing found solid!\n";
				//ADD NEIGHBORS
				source.addNeighbor(dest);
				dest.addNeighbor(source);

				/*UPDATE THE DEGREES AND PLACE 'S' IN THE UNORDERED MAP*/
				source.update(dest.id,'s');
				dest.update(source.id,'s');

				if(source.isDone(numberOfNodes))
				{
					top.push_back(source);
					source.inG=false;
					source.inTop=true;
          NumberOfNodesInTop++;
					source_it++;
					soe=true;
          if(NumberOfNodesInTop == K)
          	return;
				}
				if(dest.isDone(numberOfNodes))
				{
					top.push_back(dest);
					dest.inG=false;
					dest.inTop=true;
          if(NumberOfNodesInTop == K)
          	return;
				}
			}
			else
			{
				cout<<"probing found empty!\n";
				soe=true;

				source.decreaseState();
				dest.decreaseState();

				/*UPDATE THE DEGREES AND PLACE 'e' IN THE UNORDERED MAP*/
				source.update(dest.id,'e');
				dest.update(source.id,'e');

				updateMaxDegreeInG();
			}
		}
	}
}

int main(int argc, char** argv)
{
	//initialization, Reading the Graph and find the number of Nodes in Graph
	std::vector<Edge> Graph;
	const char * filename = argv[1];
  K = atoi(argv[2]);
	if(K == 0)
	{
			cout<<"Invalid or zero K given\n";
		  return 0;
	}
	fileToGraph(filename,NumberOfNodes,Graph);
	unsigned long maximum_probes = ((NumberOfNodes-1)*NumberOfNodes)/2;

	minDegreeInTop = NumberOfNodes;
	maxDegreeInG = 0;

	for(unsigned int i = 0;i<NumberOfNodes;i++)
	{
		Node node;
		node.initialize(i,NumberOfNodes);
		g.push_back(node);
	}

	gsoe(g,top,Graph);
  cout<<"\n\n\n==========Results==========\nHere's the top-"<<K<<" list:\n";
	printG(top);
  cout<<"Top-"<<K<<" nodes found after "<<probings<<" probings\n";
  cout<<"Maximum number of probings: "<<(NumberOfNodes*(NumberOfNodes-1))/2<<"\n";

	return 0;
}
