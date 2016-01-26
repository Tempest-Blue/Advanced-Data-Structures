#ifndef BOGGLEPLAYER_H
#define BOGGLEPLAYER_H
#include "baseboggleplayer.h"
#include "boggleutil.h"

using namespace std;

class BogglePlayer : public BaseBogglePlayer {
  public:
  void buildLexicon(const set<string>& word_list);
  
  void insert(Node* current, string word);

  bool findInLexicon(Node* current, string word);
  
  void setBoard(unsigned int rows, unsigned int cols, string** diceArray); 

  bool getAllValidWords(unsigned int minimum_word_length, set<string>* words);
 
  void findWordsInTrie(Node* current, string stringpath, int length, int minLength, set<string>* words);
 
  bool isInLexicon(const string& word_to_check); 

  vector<int> isOnBoard(const string& word);

  void isOnBoard(string wordRemaining, vector<int> pathSoFar, int startIndex);

  string toLowerCase(string word);

  bool isOnPath(int Index, vector<int>& pathSoFar);

  void diceCheck(int rowInde, int colIndex, string wordRemaining, vector<int>& pathSoFar);

  void getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols);

  BogglePlayer() {
  }
  
  ~BogglePlayer() {
  }

private:
  unsigned int ROWS; // The number of rows.
  unsigned int COLS; // The number of columns.
  string** boggleBoard; // Boggleboard object.
  vector<int> result; // Vector of indicies for a string.
  bool wordFound; // Is the word found yet?
  Node* lexRoot; // Where the lexicon trie is.
  bool setBoardCalled;
  bool buildLexiconCalled;
  vector<string> lexiconWords;
};

#endif
