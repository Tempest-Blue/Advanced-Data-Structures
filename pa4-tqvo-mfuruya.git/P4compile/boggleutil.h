#ifndef BOGGLEUTIL_H
#define BOGGLEUTIL_H
#include <string>

using std::string;

class Node
{
public:
  Node()
	{
		data = "";
		endOfWord = false;
		left = middle = right = nullptr;
	}
  Node(string newData) : data(newData)
	{
		endOfWord = false;
		left = middle = right = nullptr;
	}
  string getData() const {return data;}
  Node* getLeft() {return left;}
  Node* getMiddle() {return middle;}
  Node* getRight() {return right;}
  bool getEnd() {return endOfWord;}
  void setData(const string newData) {data = newData;}
  void setEnd(bool newEnd) {endOfWord = newEnd;}
  void setLeft(Node* newLeft) {left = newLeft;}
  void setMiddle(Node* newMiddle) {middle = newMiddle;}
  void setRight(Node* newRight) {right = newRight;}
protected:
  string data;
  bool endOfWord;
  Node* root;
  Node* left;
  Node* middle;
  Node* right;
};

#endif
