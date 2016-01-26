// Michael Furuya <mfuruya>, Kieth vo <tqvo>

#include "BitInputStream.hpp"

int BitInputStream::readBit() {
	// if the buffer is empty
	if (bufi == 0) {
		// fill the bit buffer
		// check for EOF
		if (in.eof()) {
			std::cerr << "\n\n EOF!!!\n\n";
			return -1;
		}
		buf = 0;
		in.read(&buf, 1);
		bufi = 8;
	}
	// find the LSBit
	int lsb = ((buf >> (bufi-1)) & 1);
	bufi--;
	return lsb;
}


int BitInputStream::readByte() {
	return 0;
}

int BitInputStream::readInt() {
	return 0;
}

void BitInputStream::readByte2() {
	in.read(&buf, 1);
}
