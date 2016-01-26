// Kieth Vo <tqvo>
#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include "Vertex.hpp"
#include "Edge.hpp"
using namespace std;

class Graph
{
public:
  // Graph default constructor
  Graph(){}
  
  // Checks if the file is empty
  bool is_empty(std::ifstream& pFile)
  {
    return pFile.peek() == std::ifstream::traits_type::eof();
  }

  // This function creates a graph. It takes in a string
  // which is the name of a file that we can read from.
  // The file is opened read and the vertices are made and
  // inserted into a vector of Vertex pointers. The edges
  // are also created and inserted into a vector of Edge
  // pointers. 
  // @param string - The name of the file.
  void createGraph(string fileName)
  {
    ifstream inFile;
    inFile.open(fileName);
    if (is_empty(inFile))
    {
	std::cerr << "Empty file or does not exist. Please use another file."
		  << endl;
	exit(0);
    }
    string vertex;
    string adjVertex;
    int cost;
    int time;
    string line;
    int numberOfLines = 0;
    while(getline(inFile, line) && line != "")
      {
 	++numberOfLines;
      }
    inFile.close();
    inFile.open(fileName);
    for(int i = 0; i < numberOfLines; i++)
      {
	inFile >> vertex;
	inFile >> adjVertex;
	inFile >> cost;
	inFile >> time;
	if ((cost < 0) || (time < 0))
	{
		std::cerr << "No negative weight edges allowed." << endl;
		exit(0);
	}
	Vertex* v1 = new Vertex(vertex);
	Vertex* v2 = new Vertex(adjVertex);
	if (v1->checkList(vertexList) == nullptr && 
	    v2->checkList(vertexList) == nullptr )
	  {
	    Edge* newEdge = new Edge(v1,v2,cost,time);
	    v1->addEdge(newEdge);
	    v2->addEdge(newEdge);
	    vertexList.push_back(v1);
	    vertexList.push_back(v2);
	    edgeList.push_back(newEdge);
	  }
	else if (v1->checkList(vertexList) != nullptr && 
		 v2->checkList(vertexList) == nullptr)
	  {
	    v1 = v1->checkList(vertexList);
	    Edge* newEdge = new Edge(v1,v2,cost,time);
	    v1->addEdge(newEdge);
	    v2->addEdge(newEdge);
	    vertexList.push_back(v2);
	    edgeList.push_back(newEdge);
	  }
	else if (v1->checkList(vertexList) == nullptr && 
		 v2->checkList(vertexList) != nullptr)
	{
	  v2 = v2->checkList(vertexList);
	  Edge* newEdge = new Edge(v1,v2,cost,time);
	  v1->addEdge(newEdge);
	  v2->addEdge(newEdge);
	  vertexList.push_back(v1);
	  edgeList.push_back(newEdge);
	}
	else if (v1->checkList(vertexList) != nullptr && 
		 v2->checkList(vertexList) != nullptr)
	  {
	    v1 = v1->checkList(vertexList);
	    v2 = v2->checkList(vertexList);
	    Edge* newEdge = new Edge(v1,v2,cost,time);
	    v1->addEdge(newEdge);
	    v2->addEdge(newEdge);
	    edgeList.push_back(newEdge);
	  }
      }
    inFile.close();
  }
  
  // Finds the edges in a MST using a sorted edge list.
  // The sorted edge list was created from the original
  // edge list using the sortEdges() function. The sorted
  // edge list allows us to find the smallest edge first. 
  // Then we can check each vertex to see if they are in
  // a cycle using the findParent() function in the 
  // Vertex class.
  void Kruskal()
  {
    Edge* tempEdge;
    int i = 0;
    while (mstList.size() < vertexList.size() - 1)
      {
	tempEdge = sortedEdgeList[i];
	if (tempEdge->getV1()->findParent() != 
	    tempEdge->getV2()->findParent())
	  {
	    mstList.push_back(sortedEdgeList[i]);
	    tempEdge->getV1()->setParent(tempEdge->getV2());
	    tempEdge->getV1()->addCheapEdge(sortedEdgeList[i]);
	    tempEdge->getV2()->addCheapEdge(sortedEdgeList[i]);
	  }
	++i;
      }
    tempEdge = nullptr;
    delete tempEdge;
  }

  // Sorts a vector of Edge pointers by using a priority
  // queue. The priority queue has a helper function
  // called edgeCompare. Edgecompare allows the priority
  // queue to sort the edges based on the weight.
  // Then we push the edges into a vector called sorted
  // EdgeList.
  void sortEdges()
  {
    priority_queue<Edge*, vector<Edge*>, edgeCompare> pq;
    for (unsigned int i = 0; i < edgeList.size(); ++i)
      {
	pq.push(edgeList[i]);
      }
    for (unsigned int j = 0; j < edgeList.size(); ++j)
      {
	sortedEdgeList.push_back(pq.top());
	pq.pop();
      }
  }

  // A helper function which allows a priority queue to
  // sort a vector of edges based on weight.
  struct edgeCompare
  {
    bool operator()(const Edge* edgeOne, const Edge* edgeTwo) const
    {
      return edgeOne->getCost() > edgeTwo->getCost();
    }
  };

  // Uses Dijsktra's algorithm in order to find the shortest
  // path tree. Then all the distances are summed up and the
  // result is returned.
  int Dijsktra()
  {
    int result = 0;
    for (unsigned int i = 0; i < vertexList.size(); ++i)
      {
	vertexList[i]->setDistance(0);
	Vertex* temp;
	while (sptList.size() != vertexList.size())
	  {
	    temp = findVertex();
	    sptList.push_back(temp);
	    temp->updateDistance();
	  }
	while (!sptList.empty())
	  {
	    result += sptList.back()->getDistance();
	    sptList.pop_back();
	  }
	for (unsigned int i = 0; i < vertexList.size(); ++i)
	  {
	    vertexList[i]->setDistance(std::numeric_limits<int>::max());
	  }
      }
    return result;
  }
  
  // Does the same thing as the Dijsktra function except this
  // function is done on the minimum cost spanning tree.
  int cheapDijsktra()
  {
    int result = 0;
    for (unsigned int i = 0; i <vertexList.size(); ++i)
      {
	vertexList[i]->setDistance(0);
	Vertex* temp;
	while (sptList.size() != vertexList.size())
	  {
	    temp = findVertex();
	    sptList.push_back(temp);
	    temp->updateCheapDistance();
	  }
	while (!sptList.empty())
	  {
	    result += sptList.back()->getDistance();
	    sptList.pop_back();
	  }
	for (unsigned int i = 0; i < vertexList.size(); ++i)
	  {
	    vertexList[i]->setDistance(std::numeric_limits<int>::max());
	  }
      }
    return result;
  }

  // Returns the next Vertex to be checked in Dijsktra's algorithm
  Vertex* findVertex()
  {
    int temp = std::numeric_limits<int>::max();
    Vertex* tempVertex = nullptr;
    for (unsigned int i = 0; i < vertexList.size(); ++i)
      {
	if (vertexList[i]->checkList(sptList) == nullptr &&
	    vertexList[i]->getDistance() < temp)
	  {
	    temp = vertexList[i]->getDistance();
	    tempVertex = vertexList[i];
	  }
      }
    return tempVertex;
  }

  // Calculates the cost using the vector which
  // holds all the edge in the graph.
  int totalCost() 
  {
    int totalCost = 0;
    for (unsigned int i = 0; i < edgeList.size(); ++i)
      {
	totalCost += edgeList[i]->getCost();
      }
    return totalCost;
  }

  // Calculates the cost using the vector which holds the edges 
  // used in the minimum spanning tree.The edges were obtained
  // from Kruskal's algorithm.
  int totalCheapCost()
  {
    int totalCheapCost = 0;
    for (unsigned int i = 0; i < mstList.size(); ++i)
      {
	totalCheapCost += mstList[i]->getCost();
      }
    return totalCheapCost;
  }

protected:
  vector<Vertex*> vertexList;   // The vertices in graph
  vector<Edge*> edgeList;       // The edges in graph
  vector<Edge*> sortedEdgeList; // Edges in the graph sorted by weight
  vector<Edge*> mstList;        // Edges used in the MST
  vector<Vertex*> sptList;      // The vertices in the shortest path
};

#endif

