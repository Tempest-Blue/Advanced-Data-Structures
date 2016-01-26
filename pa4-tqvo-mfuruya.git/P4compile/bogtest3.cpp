#include "baseboggleplayer.h"
#include "boggleplayer.h"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cstdlib>
#include <thread>

int main (int argc, char* argv[]) {

  BaseBogglePlayer * p = new BogglePlayer();
  set<string> lex;
  string wordA("a");
  string wordX("x");
  lex.insert(wordA);
  lex.insert("z");
  lex.insert("apple");
  string row0[] = {"D","K","E","D","D","D"};
  string row1[] = {"D","E","S","D","S","D"};
  string row2[] = {"E","L","D","D","A","E"};
  string row3[] = {"B","D","A","A","S","A"};
  string* board[] = {row0,row1,row2,row3};
  set<string> words;
  vector<int> locations;
 
  p->setBoard(4,6,board);
  vector<int> temp = p->isOnBoard("SADDLE");
  
  cout << "Indices" << endl;
  for (unsigned int i = 0; i < temp.size(); i++)
  {
	cout << temp[i] << " ";
  }
  cout << endl;

  p->buildLexicon(lex);

  if (p->isInLexicon(wordA) == true)
	cout << wordA << " is in lexicon" << endl;
  if (p->isInLexicon("z") == true)
	cout << "z is in lexicon" << endl;
  if (p->isInLexicon("BBB") == false)
	cout << "BBB is not in the lexicon" << endl;
  if (p->isInLexicon("apple") == true)
	cout << "apple is in the lexicon\n";
  if (p->isInLexicon("app") == false)
	cout << "app is not in the lexicon" << endl;
  /*
  p->setBoard(2,2,board);

  if(p->isInLexicon(wordX)) {
    std::cerr << "Apparent problem 	with isInLexicon #1." << std::endl;
    return -1;
  }
  if(!p->isInLexicon(wordA)) {
    std::cerr << "Apparent problem with isInLexicon #2." << std::endl;
    return -1;
  }

  if(p->isOnBoard(wordX).size() > 0) {
    std::cerr << "Apparent problem with isOnBoard #1." << std::endl;
    return -1;
  }

  locations.clear();
  locations = p->isOnBoard(wordA);
  if(locations.size() != 1 || locations[0] != 3) {
    std::cerr << "Apparent problem with isOnBoard #2." << std::endl;
    return -1;
  }
  
  
  if(!p->getAllValidWords(0,&words)) {
    std::cerr << "Apparent problem with getAllValidWords #1." << std::endl;
    return -1;
  };
  if(words.size() != 1 || words.count(wordA) != 1) {
    std::cerr << "Apparent problem with getAllValidWords #2." << std::endl;
    return -1;
  }
*/
  delete p;
  return 0;
}
