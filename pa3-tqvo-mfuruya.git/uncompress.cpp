// Michael Furuya <mfuruya>, Kieth Vo <tqvo>

#include "HCTree.hpp"
#include "HCNode.hpp"
#include "BitInputStream.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

int main (int argc, char ** argv) {
	
	double outfileSize = 0;	
	int infileSize = 0;
	
	if (argc != 3) {
		std::cerr << "./uncompress called with incorrect arguments.\n";
		std::cerr << "Usage: ./uncompress infile outfile\n";
		return -1;
	}
	
	// Opens the input file for reading
	std::ifstream infile(argv[1], std::ios_base::binary);
	
	// check if the input file exists
	if (!infile) {
		std::cerr << "Error opening \"0\".\n";
		std::cerr << "File does not exist.\n";
		return -1;
	}


	std::vector<int> frequency(256, 0);
	int a = 0;
	int b = 0;
	int h = 0;

	// Read the file header
	// get the number of characters in header
	int headercounter = 0;
	int frequencyCount = 0; 
	std::cout << "Reading header from file \"" << argv[1] << "\"... ";
	if (infile.read(reinterpret_cast<char*>(&h), 4).good()) {
		headercounter = h;
	}

	for (int i = 0; i < headercounter; i++) {
		if (infile.read(reinterpret_cast<char*>(&a), 1).good()) {
			if (infile.read(reinterpret_cast<char*>(&b), 4).good())
			{
				frequency[a] = b;
				frequencyCount += b;
			}
		}
	}
	std::cout << "done\n";
	std::cout << "Uncompressed file will have " << headercounter << " unique symbols and size " << frequencyCount << " bytes.\n";
	
	// Builds the Huffman code tree using the frequency vector
	std::cout << "Building Huffman code tree... ";	
	HCTree tree;
	tree.build(frequency);
	std::cout << "done.\n";
	
	// Opens the output file
	std::cout << "Writing to file \"" << argv[2] << "\"... ";
	std::ofstream outfile(argv[2], std::ios_base::binary);
	BitInputStream bis(infile);	
	bis.readByte2();
	
	// Decode bits from the input file
	int decodedValue;
	int count = 0; // # of decoded values written to outfile
	int decodeCalls = 0;
	while (infile.good()) {	// while there is still stuff to read
		decodedValue = tree.decode(bis);
		decodeCalls++;
		outfile.write(reinterpret_cast<char*>(&decodedValue), 1);
		count++;
		if (count >= frequencyCount) {
			break;
		}
	}

	std::cout << "done.\n";
	infile.close();
	outfile.close();

	// get the size of the output file
	std::ifstream infile3(argv[2], std::ios_base::binary);
	infile3.seekg(0, infile3.end);
	outfileSize = infile3.tellg();
	infile3.seekg(0, infile3.beg);
	infile3.close();
	std::cout << "Output file has size " << outfileSize << " bytes.\n";

	// get the size of the input file
	std::ifstream infile_original(argv[1], std::ios_base::binary);
	infile_original.seekg(0, infile_original.end);
	infileSize = infile_original.tellg();
	infile_original.seekg(0, infile_original.beg);
	infile_original.close();
	std::cout << "Uncompression ratio: " << (double) outfileSize/infileSize
		  << std::endl;

	return 0;
}
