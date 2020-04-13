/*
Auth: Justin Francis & Palani Thangaraj
Class: CS3505
Assign: A4: Refactoring and Testing
Created: 9/26/19
Version: 1.0
Modified:

Refactoring of assingment A3's Trie by implementing a map

*/

#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <string>
#include <map>

using namespace std;

class Trie
{
private:
  map<char, Trie> alphabetMap;
  bool isEndOfWord;

  // Private Member Functions
  void recursiveAllWordsStartingWithPrefix(std::string word, std::vector<std::string>& vectorOfWords);

public:
  Trie();

  // Public Member Functions
  void addAWord(std::string word);
  void addAWordRecursive(std::string word);
  bool isAWord(std::string word);
  vector<string> allWordsStartingWithPrefix(string prefix);
};
#endif
