// Kieth Vo <tqvo>, Michael Furuya <mfuruya>

#include "boggleplayer.h"
#include "boggleutil.h"
#include <iostream>
#include <ctime>
#include <boost/timer.hpp>
#include <fstream>
using std::cout;
using std::endl;
using std::string;

// Builds the lexicon using a ternary trie
void BogglePlayer::buildLexicon(const set<string>& word_list){
	// Intializes the root node.
	lexRoot = new Node;
	// Reads the set of strings and inserts the strings into the trie.
	for (std::set<string>::iterator it = word_list.begin(); it != word_list.end(); ++it)
	{
		insert(lexRoot, *it);
		lexiconWords.push_back(*it);
	}
	buildLexiconCalled = true;
}

void BogglePlayer::insert(Node* current, string word)
{
	string temp;
	if (word.size() != 0)
	{
		// Get the first character
		temp = word.substr(0,1);
		// Set the first character if the node is empty.
		if (current->getData() == "")
		{
			current->setData(temp);
		}
		// Checks if the character is smaller
		if (temp.compare(current->getData()) < 0)
		{
			if (current->getLeft() == nullptr)
			{
				Node* nextLeft = new Node;
				current->setLeft(nextLeft);
			}
			insert(current->getLeft(), word);
		}
		// Checks if the character is bigger
		else if (temp.compare(current->getData()) > 0)
		{
			if (current->getRight() == nullptr)
			{
				Node* nextRight = new Node;
				current->setRight(nextRight);
			}
			insert(current->getRight(), word);
		}
		// The character is equal to current node's character
		else
		{
			// If word length is > 1 then continue down middle child
			if (word.length() > 1)
			{
				if (current->getMiddle() == nullptr)
				{
					Node* nextMiddle = new Node;
					current->setMiddle(nextMiddle);
				}
				insert(current->getMiddle(), word.substr(1, word.length()));
			}
			// If word length is 1 then it's the last character of the word
			if (word.length() == 1)
				current->setEnd(true);
		}
	}
}

void BogglePlayer::setBoard(unsigned int rows, unsigned int cols, string** diceArray) {
	ROWS = rows;
	COLS = cols;

	// Builds the boggleBoard object
	boggleBoard = new std::string*[ROWS];
	for (unsigned int r = 0; r < ROWS; r++) 
	{
		boggleBoard[r] = new std::string[COLS];
	}
	
	// Assigns the string values for each die.
	for (unsigned int i =0; i < rows; i++)
	{
		for (unsigned int j = 0; j < cols; j++)
		{
			boggleBoard[i][j] = toLowerCase(diceArray[i][j]);
		}
	}
	setBoardCalled = true;
}



bool BogglePlayer::getAllValidWords(unsigned int minimum_word_length, set<string>* words) {
	boost::timer t;
	// If board or lexicon not built. Return false.
	if (buildLexiconCalled == false || setBoardCalled == false)
	{
		return false;
	}
	
	// "THE BAD METHOD" X_X
	for (unsigned int i = 0; i < lexiconWords.size(); i++)
	{
		if ((lexiconWords[i].length() >= minimum_word_length) && !(isOnBoard(lexiconWords[i]).empty()))
		{
			words->insert(lexiconWords[i]);
		}
	}

	// Recursively find the words in the lexicon that are on the board
	// "THE GOOD METHOD" but it's slower than the bad method for us. T_T
	//findWordsInTrie(lexRoot, "", 0, minimum_word_length, words);

	// Print the elapsed time
	std::cout << "Time elapsed: " << t.elapsed()*1000 << " ms" << std::endl;

	// If words were found, return true
	if (!(words->empty()))
		return true;
	// If no words were found, return false
	else
		return false;
}


// Recursive method to check if a word in the lexicon is on the board
void BogglePlayer::findWordsInTrie(Node* current, string stringpath, int length, int minLength, set<string>* words)
{
	// If current Node does not exist, return
	if (current == nullptr)
		return;
	
	// Recursive calls to go left and right
	findWordsInTrie(current->getLeft(), stringpath, length, minLength, words);
	findWordsInTrie(current->getRight(), stringpath, length, minLength, words);
	
	// If the string including the current node is not on the board, return
	string temp = current->getData();
	if (isOnBoard(stringpath + temp).empty())
		return;

	// If length of string is greater than minLength and the current node is the end of a word,
	// then add it to the set of valid words
	if ( ((int)(length+temp.length()) >= minLength) && ((current->getEnd()) == true))
		words->insert(stringpath + temp);

	// Continue down the middle root
	findWordsInTrie(current->getMiddle(), stringpath+temp, length+temp.length(), minLength, words);
}




bool BogglePlayer::isInLexicon(const string& word_to_check) {
	// If lexicon is built. Use recursive function to search
	if (buildLexiconCalled == true)
		return findInLexicon(lexRoot, word_to_check);
	else
		return false;
}

bool BogglePlayer::findInLexicon(Node* current, string word)
{
	string temp;
	// If you reach the end of the trie then the word doesn't exist
	if (current == nullptr)
		return 0;
	if (word.length() != 0)
	{
		temp = word.substr(0,1);
		// If the character is smaller
		if (temp.compare(current->getData()) < 0)
			return findInLexicon(current->getLeft(), word);
		// If the character is bigger
		else if (temp.compare(current->getData()) > 0)
			return findInLexicon(current->getRight(), word);
		// The character is equal to the Node's character
		else
		{
			// If it's the last character and the character is the end of the
			// word in the trie that means it exists
			if (word.length() == 1)
				return current->getEnd();
			// Else keep going down the middle child of the node
			return findInLexicon(current->getMiddle(), word.substr(1,word.length()));
		}
	}
	return 0;
}

vector<int> BogglePlayer::isOnBoard(const string& word) {
	result.clear();
	wordFound = false;

	// If there is no board then return an empty string
	if (ROWS == 0 && COLS == 0)
	{
		std::cerr << "There is no board made yet.\n";
		return result;
	}

	// Finds possible starting points and calls the recursive isOnBoard
	// function to find the remaining word and modify the result vector.
	for (unsigned int i = 0; i < ROWS; i++)
	{
		for (unsigned int j = 0; j < COLS; j++) 
		{
			if (word.compare(0, boggleBoard[i][j].length(), boggleBoard[i][j]) == 0)
				isOnBoard(word.substr(boggleBoard[i][j].length(), word.length()), result, i * COLS + j);
			// If found word, return result vector
			if (wordFound) 
				return result;
		}
	}

	// After searching, if no result vector returned yet, then return empty vector.
	return result;
}

// Returns if the die is already added on the index path.
bool BogglePlayer::isOnPath(int index, vector<int>& pathSoFar) {
	for (unsigned int i = 0; i < pathSoFar.size(); i++)
	{
		if (index == pathSoFar[i])
			return true;
	}
	return false;
}

// Returns a lower-case version of a string.
string BogglePlayer::toLowerCase(string word)
{
	for (unsigned int i = 0; i < word.length(); i++) 
	{
		word[i] = tolower(word[i]);
	}
	return word;
}

// Helper function to check a possible die path.
void BogglePlayer::diceCheck(int rowIndex, int colIndex, string wordRemaining, vector<int>& pathSoFar)
{
	// Check if the die is a part of the word left.
	if ((boggleBoard[rowIndex][colIndex] == wordRemaining.substr(0, 
	     boggleBoard[rowIndex][colIndex].length())) && !(isOnPath(rowIndex * COLS + colIndex, pathSoFar)))
	{
		isOnBoard(wordRemaining.substr(boggleBoard[rowIndex][colIndex].length(),
		wordRemaining.length()), pathSoFar, rowIndex * COLS + colIndex);
	}
}

void BogglePlayer::isOnBoard(string wordRemaining, vector<int> pathSoFar, int startIndex) {
	int col = startIndex%COLS;
	int row = (startIndex-col)/COLS;
	
	pathSoFar.push_back(startIndex);

	// Base case. Nothing left to find.
	if (wordRemaining == "") {
		result = pathSoFar;
		wordFound = true;
		return;
	}
	
	// 8 Checks. One for each possible die path.
	else if (!wordFound)
	{
		// Extra return statements to try to optimize the search times!!!
		if (!(row-1 < 0)) {
			diceCheck(row-1, col, wordRemaining, pathSoFar);   	   // Top
			if (wordFound)
				return;
			if ((unsigned)(col+1) != COLS)
				diceCheck(row-1, col+1, wordRemaining, pathSoFar); // Top right
			if (wordFound)
				return;
			if (!(col-1 < 0))			
				diceCheck(row-1, col-1, wordRemaining, pathSoFar); // Top left
		}
		if (wordFound)
			return;
		if ((unsigned)(col+1) != COLS)
			diceCheck(row, col+1, wordRemaining, pathSoFar);  	   // Right
		if (wordFound)
			return;
		if ((unsigned)(row+1) != ROWS) {
			diceCheck(row+1, col, wordRemaining, pathSoFar);   	   // Bottom
			if (wordFound)
				return;
			if ((unsigned)(col+1) != COLS) 
				diceCheck(row+1, col+1, wordRemaining, pathSoFar); // Bottom right
			if (wordFound)
				return;
			if (!(col-1 < 0))
				diceCheck(row+1, col-1, wordRemaining, pathSoFar); // Bottom left
		}
		if (wordFound)
			return;
		if (!(col-1 < 0))
			diceCheck(row, col-1, wordRemaining, pathSoFar);   	   // Left
	}
}


void BogglePlayer::getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols) {

	ifstream infile;
	string line;
	vector<string> strings;

	infile.open("brd.txt");

	infile >> line;
	*rows = atoi(line.c_str());
	
	infile >> line;
	*cols = atoi(line.c_str());

	while (!infile.eof())
	{
		infile >> line;
		strings.push_back(line);
	}

	infile.close();

	ROWS = *rows;
	COLS = *cols;

	// Create an infile
	// Set rows and cols using the first 2 lines
	// Set new_board[i][j]

	// Builds the boggleBoard object
	new_board = new std::string*[ROWS];
	for (unsigned int r = 0; r < ROWS; r++) 
	{
		new_board[r] = new std::string[COLS];
	}
	
	// Assigns the string values for each die.
	int stringsindex = 0;
	for (unsigned int i =0; i < *rows; i++)
	{
		for (unsigned int j = 0; j < *cols; j++)
		{
			new_board[i][j] = strings[stringsindex];
			stringsindex++;
		}
	}
}

