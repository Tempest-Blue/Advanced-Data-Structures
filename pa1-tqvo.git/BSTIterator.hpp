#ifndef BSTITERATOR_HPP
#define BSTITERATOR_HPP
#include "BSTNode.hpp"
#include <list>
#include <iterator>

// declare BST here, so friend declaration below will work.
template<typename X> class BST;

template<typename Data>
class BSTIterator : public std::iterator<std::input_iterator_tag,Data> {

private:

  BSTNode<Data>* curr; // pointer to this BSTIterator's current BSTNode

  /** Constructor.  Use the argument to initialize a given BSTNode
   *  in this BSTIterator.
   *  Note: this constructor is private; but friend classes can access.
   */ // TODO
  BSTIterator(BSTNode<Data>* curr) {
    this->curr = curr;
  }


public:
  // make BST a friend class, so BST can create a BSTIterator when needed.
  friend class BST<Data>;
  // make BSTNode a friend class, so BSTNode can access curr member variable.
  friend class BSTNode<Data>;


  /** Dereference operator.
   *  Return a copy of the Data item in the current BSTNode.
   */
  Data operator*() const {
    if (curr == nullptr)
      {
	return NULL;
      }
    else
      return curr->data;
  }
  
  /** Pre-increment operator.
   *  Update this BSTIterator to point to the inorder successor of the current
   *  BSTNode, and return a reference to the updated BSTIterator.
   */
  BSTIterator<Data>& operator++() {
    curr = curr->successor();
    return *this;
  }

  /** Post-increment operator. 
   *  Update this BSTIterator to point to the inorder successor of the current
   *  BSTNode, and return a copy of the old, non-updated BSTIterator.
   */
  BSTIterator<Data> operator++(int) {
    BSTIterator before = BSTIterator(curr);
    ++(*this);
    return before;
  }
  

  /** Equality test operator. */ // TODO
  bool operator==(BSTIterator<Data> const & other) const {
    BSTIterator current = BSTIterator(curr);
    if (*current == *other)
      return true;
    else
      return false;
  }

  /** Inequality test operator. */ // TODO
  bool operator!=(BSTIterator<Data> const & other) const {
    BSTIterator current = BSTIterator(curr);
    if (*current != *other)
      return true;
    else
      return false;
  }

};

#endif //BSTITERATOR_HPP
