// Michael Furuya <mfuruya>, Kieth Vo <tqvo>

#include "BitOutputStream.hpp"

void BitOutputStream::writeBit(int bit)
{
	if (bufi >= 8)
		this->flush();
	char lsb = bit & 1;	// Finds the least significant bit
	buf = (buf << 1) | lsb;	// Shifts left and adds to the buffer
	bufi++;			// Increments the index
}

void BitOutputStream::writeByte(int b)
{
}

void BitOutputStream::writeInt(int i)
{
}

void BitOutputStream::flush()
{
	// if less 8 bits were written to buffer
	if (bufi != 8) {
		// shift bits to the MSBits of buf
		buf = (buf << (8 - bufi));
		out.put(buf);
		out.flush();
	}
	else {
		out.put(buf);
		out.flush();
		buf = 0;
		bufi = 0;
	}
}


