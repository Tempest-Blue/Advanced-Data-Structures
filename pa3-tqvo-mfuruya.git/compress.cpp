// Michael Furuya <mfuruya>, Kieth Vo <tqvo>

#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include "HCTree.hpp"
#include "HCNode.hpp"
#include "BitOutputStream.hpp"

int main (int argc, char ** argv) {

	// check the number of arguments
	if (argc != 3) {
		std::cerr << "./compress called with incorrect arguments.\n";
		std::cerr << "Usage: ./compress infile outfile\n";
		return -1;
	}

	
	// get the name of the input file
	std::ifstream infile(argv[1], std::ios_base::binary);
	
	// check if the input file exists
	if (!infile) {
		std::cerr << "Error opening \"0\".\n";
		std::cerr << "File does not exist.\n";
		return -1;
	}
	std::vector<int> frequency(256, 0); // counts of each byte
	int a = 0;
	int fileSizeBytes = 0;
	int numUniqueSymbols = 0;
	//int outfileSize = 0;
	double outfileSize = 0;	
	int infileSize = 0;
	

	// reads the file and counts occurrence of each byte
	std::cout << "Reading from file \"" << argv[1] << "\" ... ";
	while ((infile.read(reinterpret_cast<char*>(&a), 1)).good()) {
		++frequency[a];
		fileSizeBytes++;
	}
	std::cout << "done\n";
	infile.close();

	for (int n = 0; n < frequency.size(); n++) {
		if (frequency[n] != 0) {
			numUniqueSymbols++;
		}
	}
	std::cout << "Found " << numUniqueSymbols << " unique symbols " <<
		"in input file of size " << fileSizeBytes << " bytes\n";


	std::cout << "Building Huffman code tree... ";
	HCTree tree;// = new HCTree();
	tree.build(frequency);
	std::cout << "done\n";

	// open the output file
	std::ofstream outfile (argv[2], std::ios_base::binary);
	std::cout << "Writing to file \"" << argv[2] << "\"... ";


	// write the file header of the output file (all frequencies)
	// write out how many different bytes are in the file
	outfile.write(reinterpret_cast<char*>(&numUniqueSymbols), 4);

	// write each character that appears and its frequency next to it
	for (int f = 0; f < frequency.size(); f++) {
		if (frequency[f] != 0) {
			outfile.write(reinterpret_cast<char*>(&f), 1);
			outfile.write(reinterpret_cast<char*>(&frequency[f]), 4);
		}
	}


	BitOutputStream bos(outfile);
	// re-open the input file
	std::ifstream infile2(argv[1], std::ios_base::binary);
	while (infile2.read(reinterpret_cast<char*>(&a), 1).good()) {
		//tree.encode((byte)a, outfile);
		tree.encode((byte)a, bos);
	}
	bos.flush();	

	infile2.close(); // close the input file
	outfile.close(); // close the file
	
	std::cout << "done\n";  // finished writing to the file


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
	std::cout << "Compression ratio: " << (double) outfileSize/infileSize
		  << std::endl;

}
