// Michael Furuya <mfuruya>, Kieth Vo <tqvo>

#include "HCNode.hpp"

bool HCNode::operator<(const HCNode& other)
{
	if (this->count == other.count)
	{
		return other.symbol < this->symbol;
	}
	else
		return other.count < this->count;
}
