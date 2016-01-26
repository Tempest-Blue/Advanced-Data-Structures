// Kieth Vo <tqvo>
#ifndef EDGE_HPP
#define EDGE_HPP
#include <vector>
using namespace std;

class Vertex;

class Edge
{
public:
  // Default constructor
  Edge(): v1(nullptr), v2(nullptr), cost(0), time(0){}
  
  // Overloaded constructor
  Edge(Vertex* first, Vertex* second, int newCost, int newTime) : v1(first),
       v2(second), cost(newCost), time(newTime){}

  // Returns the cost of the edge
  int getCost() const {return cost;}

  // Returns the time cost of the edge
  int getTime() const {return time;}
  
  // Returns either Vertex one or Vertex two as a pointer.
  Vertex* getV1() const {return v1;}
  Vertex* getV2() const {return v2;}
private:
  int cost;   // The connection's cost
  int time;   // The connection's time
  Vertex* v1; // The first vertex the edge connects
  Vertex* v2; // The second vertex the edge connects
};

#endif
