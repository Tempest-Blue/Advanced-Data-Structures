#ifndef BST_HPP
#define BST_HPP
#include "BSTNode.hpp"
#include "BSTIterator.hpp"
#include <utility> // for std::pair

template<typename Data>
class BST {

protected:

  /** Pointer to the root of this BST, or nullptr if the BST is empty */
  BSTNode<Data>* root;

  /** Number of Data items stored in this BST. */
  unsigned int isize;


public:

  /** iterator is an aliased typename for BSTIterator<Data>. */
  typedef BSTIterator<Data> iterator;

  /** Default constructor.
      Initialize an empty BST.
   */
  BST() : root(nullptr), isize(0)  {
  }


  /** Default destructor.
   *  It is virtual, to allow appropriate destruction of subclass objects.
   *  Delete every node in this BST.
   */ // TODO
  virtual ~BST() {
    clear(root);
  }

  /** Insert a Data item in the BST.
   *  Return a pair, with the sspair's first member set to an
   *  iterator pointing to either the newly inserted element
   *  or to the equivalent element already in the set.
   *  The pair's second element is set to true
   *  if a new element was inserted or false if an
   *  equivalent element already existed.
   */ // TODO
  virtual std::pair<iterator,bool> insert(const Data& item) {
    BSTIterator<Data> newIterator = this->find(item);
    if (newIterator != nullptr)
      {
	std::pair<iterator,bool> result = std::make_pair (newIterator,false);
	return result; 
      }
    else
      {
	BSTNode<Data>* current = root;
	if (current == nullptr)
	  {
	    BSTNode<Data>* newNode = new BSTNode<Data>(item);
	    current = newNode;
       	    BSTIterator<Data> newIter = BST<Data>::iterator(current);
       	    std::pair<iterator,bool> result = std::make_pair (newIter, true);
	    ++isize;
       	    return result;
	  }
	else
	  {
	while (current != nullptr)
	  {
	    if (item < current->data)
	      {
		if (current->left == nullptr)
		  {
		    BSTNode<Data>* newNode = new BSTNode<Data>(item);
		    current->left = newNode;
		    BSTIterator<Data> newIter = BST<Data>::iterator(current->left);
		    std::pair<iterator,bool> result = std::make_pair (newIter, true);
		    return result;
		    current = nullptr;
		  }
		else
		  current = current->left;
	      }
	    else
	      {
		if (current->right == nullptr)
		  {
		    BSTNode<Data>* newNode = new BSTNode<Data>(item);
		    current->right = newNode;
		    BSTIterator<Data> newIter = BST<Data>::iterator(current->right);
		    std::pair<iterator,bool> result = std::make_pair (newIter, true);
		    return result;
		    current = nullptr;
		  }
		else
		  current = current->right;
	      }
	  }
	isize += 1;
      }
      }
  }


  /** Find a Data item in the BST.
   *  Return an iterator pointing to the item, or the end
   *  iterator if the item is not in the BST.
   */ // TODO
  iterator find(const Data& item) const {
  BSTNode<Data>* current = root;
     while (current->left != nullptr)
	{
	  current = current->left;
	}
  BSTIterator<Data> newIter = BST<Data>::iterator(current);
     while (newIter != nullptr)
	{
             if (*newIter == item)
	         return newIter;
	     else
 	         ++newIter;
	}
  return newIter;
}
  /** Return the number of items currently in the BST.
   */ // TODO
  unsigned int size() const {
      return isize;
  }

  /** Remove all elements from this BST, and destroy them,
   *  leaving this BST with a size of 0.
   */ // TODO
  void clear(BSTNode<Data>* someNode) {
    if (someNode != nullptr)
    {
        clear(someNode->left);
        clear(someNode->right);
        delete someNode;
        someNode = nullptr;
    }
    root = nullptr;
    isize = 0;
  }

  /** Return true if the BST is empty, else false.
   */ // TODO
  bool empty() const {
    if (isize != 0)
        return false;
    else return true;
  }

  /** Return an iterator pointing to the first item in the BST.
   */ // TODO
  iterator begin() const {
    if (isize == 0)
     return typename BST<Data>::iterator(nullptr);
    if (isize == 1)
     return typename BST<Data>::iterator(root);
   else
    {
      BSTNode<Data>* current = root;
      while (current->left != nullptr)
	{
	  current = current->left;
	}
      return typename BST<Data>::iterator(current);
    }
  }

  

  /** Return an iterator pointing past the last item in the BST.
   */
  iterator end() const {
    return typename BST<Data>::iterator(nullptr);
  }

  

 };


#endif //BST_HPP
