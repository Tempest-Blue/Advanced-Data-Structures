#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>
#include <bitset>

using namespace std;

/** A class for writing bits (and chars and ints) to an ostream
 */
class BitOutputStream {

private:
  ostream& out;  // the ostream to delegate to
  char buf;     // the buffer of bits
  //  bitset<8> buf;     // the buffer of bits
  int bufi;     // the bit buffer index

public:
  BitOutputStream(ostream& s) : out(s), buf(0), bufi(0) { }
  //  BitOutputStream(ostream& s) : out(s), bufi(0) { }

  /** Write the least significant bit of the argument into
   *  the bit buffer, and increment the bit buffer index.
   *  Flush to the ostream first if the bit buffer is full.
   *  This must be consistent with BitInputStream::readBit().
   */
  void writeBit(int bit);

  /** Write the least significant byte of the argument to the ostream.
   *  This function doesn't touch the bit buffer.
   *  The client has to manage interaction between writing bits
   *  and writing bytes.
   */
  void writeByte(int b);

  /** Write the argument to the ostream.
   *  This function doesn't touch the bit buffer.
   *  The client has to manage interaction between writing bits
   *  and writing ints.
   */
  void writeInt(int i); 

  /** If the bit buffer contains any bits, flush the bit buffer to the ostream,
   *  clear the bit buffer, and set the bit buffer index to 0.
   *  Also flush the ostream itself.
   */
  void flush();
};

#endif // BITOUTPUTSTREAM_HPP
