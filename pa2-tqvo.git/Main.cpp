// Kieth Vo <tqvo>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <istream>
#include <vector>
#include "Edge.hpp"
#include "Vertex.hpp"
#include "Graph.hpp"

using namespace std;

// Takes the second input from the terminal
// and that input is the name of the file to
// read from. Makes the graph. Sorts the edge
// vector and uses Kruskal's algorithm.
int main(int argc, char** argv)
{
  if (!(argc == 2))
	cerr << "Error";
  string fileName = argv[1];
  Graph g1;
  g1.createGraph(fileName);
  g1.sortEdges();
  g1.Kruskal();
  int part1 = g1.totalCost();
  int part2 = g1.totalCheapCost();
  int part3 = part1 - part2;
  int part4 = g1.Dijsktra();
  int part5 = g1.cheapDijsktra();
  int part6 = part5 - part4;
  cout << part1 << endl;
  cout << part2 << endl;
  cout << part3 << endl;
  cout << part4 << endl;
  cout << part5 << endl;
  cout << part6 << endl;
  return 0;
}
