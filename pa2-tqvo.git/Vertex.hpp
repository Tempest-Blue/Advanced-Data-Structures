// Kieth Vo <tqvo>
#ifndef VERTEX_HPP
#define VERTEX_HPP
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
#include "Edge.hpp"

class Vertex
{
public:
  // Default constructor
  Vertex(){}
  
  // Overloaded constructor
  Vertex(string newName) 
  {
    name = newName; 
    parent = nullptr;
    distance = std::numeric_limits<int>::max();
  }

  // Returns the name of the Vertex
  string getName() const {return name;}
  
  // Returns the parent of the Vertex
  Vertex* getParent() const {return parent;}

  // Returns the distance of the Vertex
  int getDistance() const {return distance;}
  
  // Points to a new parent Vertex
  // @param Vertex* - The new parent vertex
  void setParent(Vertex* newParent) {parent = newParent;}

  // Sets the new distance from the source
  // @param int - The new distance
  void setDistance(int newDistance) {distance = newDistance;}

  // Adds an edge to the adjacency vector of this vertex
  // @param Edge* - The edge to add to the list
  void addEdge(Edge* newEdge) {adjList.push_back(newEdge);}

  // Adds an edge to the cheap adjacency vector of this vertex
  // @param Edge* - The edge to add to the list
  void addCheapEdge(Edge* newEdge) {cheapAdjList.push_back(newEdge);}

  // Returns the adjacency vector
  vector<Edge*> getEdges() {return adjList;}

  // Checks a vector to see if a vertex exists.
  // Then if it does exist then the vertex is
  // returned. If it doesn't exist then a 
  // nullptr is returned.
  // @param vector<Vertex*> The list to check
  Vertex* checkList(vector<Vertex*> someList)
  {
    for (unsigned int i = 0; i < someList.size(); ++i)
      {
	if (this->getName() == someList[i]->getName())
	  {
	    return someList[i];
	  }
      }
    return nullptr;
  }

  // Finds the parent of a vertex in order to
  // see if there is a cycle.
  Vertex* findParent()
  {
    if (this->getParent() == nullptr)
      {
	return this;
      }
    else
      {
	return this->getParent()->findParent();
      }
  }

  // Updates all adjacent vertices' distance values
  void updateDistance()
  {
    Vertex* temp;
    Edge* tempEdge;
    for(unsigned int i = 0; i < adjList.size(); ++i)
      {
	if(adjList[i]->getV1() == this)
	  {
	    temp = adjList[i]->getV2();
	  }
	if (adjList[i]->getV2() == this)
	  {
	    temp = adjList[i]->getV1();
	  }
	tempEdge = adjList[i];
	if ((this->getDistance() + tempEdge->getTime()) < temp->getDistance())
	  {
	    temp->setDistance(this->getDistance() + tempEdge->getTime());
	  }
      }
  }

  // Updates all cheap adjacent vertices' distance values
  void updateCheapDistance()
  {
    Vertex* temp;
    Edge* tempEdge;
    for(unsigned int i = 0; i < cheapAdjList.size(); ++i)
      {
	if(cheapAdjList[i]->getV1() == this)
	  {
	    temp = cheapAdjList[i]->getV2();
	  }
	if (cheapAdjList[i]->getV2() == this)
	  {
	    temp = cheapAdjList[i]->getV1();
	  }
	tempEdge = cheapAdjList[i];
	if ((this->getDistance() + tempEdge->getTime()) < temp->getDistance())
	  {
	    temp->setDistance(this->getDistance() + tempEdge->getTime());
	  }
      }
  }
private:
  string name;                // The name of the vertex
  int distance;               // The distance from the source
  Vertex* parent;             // Pointer to the parent vertex of this vertex
  vector<Edge*> adjList;      // The adjacency vector of this vertex
  vector<Edge*> cheapAdjList; // The adjacency vector for the cheap cost network
};

#endif
