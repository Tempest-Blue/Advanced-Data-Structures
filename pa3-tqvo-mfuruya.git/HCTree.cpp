// Michael Furuya <mfuruya>, Kieth Vo <tqvo>

#include "HCTree.hpp"
#include "BitOutputStream.hpp"
#include "BitInputStream.hpp"
#include <queue>

void HCTree::build(const vector<int>& freqs)
{

	// create a priority queue of HCNodes
	std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp > priorityQ;
	
	for(unsigned int i = 0; i < freqs.size(); ++i)
	{
		if (freqs[i] != 0)
		{
			// create each node with a count and byte, but no children or parent
			leaves[i] = new HCNode (freqs[i], i, 0, 0, 0);
			priorityQ.push(leaves[i]);
		}
	}

	while (priorityQ.size() > 1) {
		// find the 2 nodes with the lowest count
		HCNode* leftChild = priorityQ.top();
		priorityQ.pop();
		HCNode* rightChild = priorityQ.top();
		priorityQ.pop();
		// create a parent node
		HCNode* parent = new HCNode((leftChild->count + rightChild->count),
					    0, leftChild, rightChild, 0);
		leftChild->p = parent;
		rightChild->p = parent;
		priorityQ.push(parent);
	}

	// set the root of the tree
	if (priorityQ.size() > 0) {
		root = priorityQ.top();
	}
	else {
		root = (HCNode*) 0;
	}


}

void HCTree::encode (byte symbol, ofstream& out) const {

	std::vector<int> code;
	// find the path to the leaf to make the code
	for (int i = 0; i < leaves.size(); i++) {
		if (leaves[i] != 0 && leaves[i]->symbol == symbol) {
			HCNode * node = leaves[i];
			while (node->p != 0) {
				if (node == node->p->c0) {
					code.push_back(0);
					node = node->p;
				}
				else if (node == node->p->c1) {
					code.push_back(1);
					node = node->p;
				}
			}
		}

		
	}	

	// write the code to the file
	for (int v = code.size() - 1; v >= 0 ; v--) {
		out.write(reinterpret_cast<char*> (&code[v]), 1);	
	}
}

void HCTree::encode (byte symbol, BitOutputStream& out) const {

	std::vector<int> code;
	// find the path to the leaf to make the code
	for (int i = 0; i < leaves.size(); i++) {
		if (leaves[i] != 0 && leaves[i]->symbol == symbol) {
			HCNode * node = leaves[i];
			while (node->p != 0) {
				if (node == node->p->c0) {
					code.push_back(0);
					node = node->p;
				}
				else if (node == node->p->c1) {
					code.push_back(1);
					node = node->p;
				}
			}
		}
	}	

	// write the code to the file
	for (int v = code.size() - 1; v >= 0 ; v--) {
		out.writeBit(code[v]);
	}
}

int HCTree::decode (ifstream& in) const {
	int a = 0;
	HCNode * current = root;
	
	// traverse the tree until a leaf is found
	while (in.read(reinterpret_cast<char*>(&a), 1).good()) {
		if (a == 0) {
			current = current->c0;
		}
		else if (a == 1) {
			current = current->c1;
		}
		// check if a leaf
		if (current->symbol != 0 && (current->c0 == 0 && current->c1 == 0)) {
			return current->symbol;
		}

	}
}

int HCTree::decode (BitInputStream& in) const {
	int b = 0;

	HCNode * current = root;
	
	// check if root is the only node in the tree	
	if (current->c0 == 0 && current->c1 == 0) {
		return current->symbol;
	}

	// traverse the tree until a leaf is found
	while ((b = in.readBit()) != -1) {
		if (b == 0) {
			current = current->c0;
		}
		else if (b == 1) {
			current = current->c1;
		}
		// check if a leaf
		if (current->c0 == 0 && current->c1 == 0) {
			return current->symbol;
		}

	}

}

