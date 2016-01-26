#include "boggleplayer.h"
#include "boggleutil.h"
#include <iostream>
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
		//lexiconWords.push_back(*it);
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
			boggleBoard[i][j] = diceArray[i][j];
		}
	}
	setBoardCalled = true;

	// Prints out the board to help visualization.	
	/*std::cerr << "\nPrinting Board\n";
	for (unsigned int i =0; i < ROWS; i++)
	{
		for (unsigned int j = 0; j < COLS; j++)
		{
			cout << boggleBoard[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;*/
}



bool BogglePlayer::getAllValidWords(unsigned int minimum_word_length, set<string>* words) {
	if (buildLexiconCalled == false || setBoardCalled == false)
	{
		return false;
	}

	/*
	for (unsigned int i = 0; i < lexiconWords.size(); i++)
	{
		if ((lexiconWords[i].length() > minimum_word_length) && (isOnBoard(lexiconWords[i]).size() != 0))
		{
			words->insert(lexiconWords[i]);
		}
	}
	*/

	// recursively find the words in the lexicon that are on the board
	findWordsInTrie(lexRoot, "", 0, minimum_word_length, words);

	// if words were found, return true
	if (words->size() > 0)
	{
		return true;
	}
	// if no words were found, return false
	else
	{
		return false;
	}
	
	
}


/* Recursive method to check if a word in the lexicon is on the board */
void BogglePlayer::findWordsInTrie(Node* current, string stringpath, int length, int minLength, set<string>* words)
{
	// if current Node does not exist, return
	if (current == nullptr)
	{
		return;
	}
	
	// recursive calls to go left and right	
	findWordsInTrie(current->getLeft(), stringpath, length, minLength, words);
	findWordsInTrie(current->getRight(), stringpath, length, minLength, words);
	
	// if the string including the current node is not on the board, return
	if ((isOnBoard(stringpath + current->getData())).size() == 0)
	{
		return;
	}

	// if length of string is greater than minLength and the current node is the end of a word,
	// then add it to the set of valid words
	if ( (length >= minLength) && ((current->getEnd()) == true) )
	{
		words->insert(stringpath + current->getData());
	}

	// continue down the middle root
	findWordsInTrie(current->getMiddle(), stringpath+current->getData(), length+1, minLength, words);
}




bool BogglePlayer::isInLexicon(const string& word_to_check) {
	if (buildLexiconCalled == true)
	{
		// Use recursive helper function to search
		return findInLexicon(lexRoot, word_to_check);
	}
	else
	{
		return false;
	}
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


	// convert all characters in word to lowercase.
	string lowercaseword = toLowerCase(word);
	
	// Finds possible starting points and calls the recursive isOnBoard
	// function to find the remaining word and modify the result vector.
	for (unsigned int i = 0; i < ROWS; i++)
	{
		for (unsigned int j = 0; j < COLS; j++)
		{
			string diceString = toLowerCase(boggleBoard[i][j]);
			if (word.compare(0, boggleBoard[i][j].length(), diceString) == 0)
			{
				isOnBoard(word.substr(boggleBoard[i][j].length(), word.length()), result, i * COLS + j);
			}
			// If found word, result result vector
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
	string lowerCaseWord = word;
	for (unsigned int i = 0; i < word.length(); i++) 
	{
		// if letter of the word is between capital A and capital Z
		if (lowerCaseWord[i] >= 65 && lowerCaseWord[i] <= 90)
		{
			lowerCaseWord[i] = tolower(lowerCaseWord[i]);
		}
	}
	return lowerCaseWord;
}

// Helper function to check a possible die path.
void BogglePlayer::diceCheck(int rowIndex, int colIndex, string wordRemaining, vector<int>& pathSoFar)
{
	string diceWord = toLowerCase(boggleBoard[rowIndex][colIndex]);
	// Check if the die is a part of the word left.
	if ((diceWord == wordRemaining.substr(0, boggleBoard[rowIndex][colIndex].size())) && !(isOnPath(rowIndex * COLS + colIndex, pathSoFar)))
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
	}
	
	// 8 Checks. One for each possible die path.
	else if (!wordFound)
	{
		if (!(row-1 < 0)) {
			diceCheck(row-1, col, wordRemaining, pathSoFar);   	   // Top
			if ((unsigned)(col+1) != COLS)
				diceCheck(row-1, col+1, wordRemaining, pathSoFar); // Top right
			if (!(col-1 < 0))			
				diceCheck(row-1, col-1, wordRemaining, pathSoFar); // Top left
		}
		if ((unsigned)(col+1) != COLS)
			diceCheck(row, col+1, wordRemaining, pathSoFar);  	   // Right
		if ((unsigned)(row+1) != ROWS) {
			diceCheck(row+1, col, wordRemaining, pathSoFar);   	   // Bottom
			if ((unsigned)(col+1) != COLS) 
				diceCheck(row+1, col+1, wordRemaining, pathSoFar); // Bottom right
			if (!(col-1 < 0))
				diceCheck(row+1, col-1, wordRemaining, pathSoFar); // Bottom left
		}
		if (!(col-1 < 0))
			diceCheck(row, col-1, wordRemaining, pathSoFar);   	   // Left
	}
}


void BogglePlayer::getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols) {
	//new_board = new std::string*[*rows];

	ROWS = *rows;
	COLS = *cols;

	// Builds the boggleBoard object
	boggleBoard = new std::string*[ROWS];
	for (unsigned int r = 0; r < ROWS; r++) 
	{
		boggleBoard[r] = new std::string[COLS];
	}
	
	// Assigns the string values for each die.
	for (unsigned int i =0; i < *rows; i++)
	{
		for (unsigned int j = 0; j < *cols; j++)
		{
			boggleBoard[i][j] = new_board[i][j];
		}
	}
	setBoardCalled = true;

}

