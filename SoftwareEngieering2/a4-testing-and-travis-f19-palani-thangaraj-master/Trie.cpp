/*
Auth: Justin Francis & Palani Thangaraj
Class: CS3505
Assign: A4: Refactoring and Testing
Created: 9/26/19
Version: 1.0
Modified:

Refactoring of assingment A3's Trie by implementing a map

*/

#include "Trie.h"
#include <vector>
#include <string>
#include <map>

using namespace std;

/*
Constructor
*/
Trie::Trie()
{
  isEndOfWord = false;
}


/*
A method addAWord that accepts a std::string and returns void. The word
passed into the method should be added to the Trie. Duplicate adds should
not change the Trie. You may assume that the word is only made up of
lower-case characters from a-z.
*/
void Trie::addAWord(string word)
{
  if(!word.empty())
    addAWordRecursive(word);
}


/*
a method addAWordRecursive is a elper method to add a word recursively to the
Trie
*/
void Trie::addAWordRecursive(string word)
{
  if(!word.empty())
  {
    char firstLetterOfWord = word[0];
    //if branch doesnt exist, create a new node
    if(alphabetMap.find(firstLetterOfWord) == alphabetMap.end())
      alphabetMap.insert(pair<char, Trie>(firstLetterOfWord, Trie()));
    //recurse with shortened word
    alphabetMap[firstLetterOfWord].addAWordRecursive(word.substr(1, word.length()));
  }
  // base case
  else
    isEndOfWord = true;
}


/*
A method isAWord that accepts a std::string and returns bool. The argument
is a string assumed to be made up of characters 'a'-'z'. If the word is
found in the Trie, the method should return true, otherwise return
false.  A Trie should report that an empty string is not in the Trie.
*/
bool Trie::isAWord(string word)
{
  bool isWord;
  //base case for recusersion
  if(word.empty())
    isWord = isEndOfWord;
  else
  {
    char firstLetterOfWord = word[0];
    // if letter is in map, recurse with shortened word
    if(alphabetMap.find(firstLetterOfWord) != alphabetMap.end())
      isWord = alphabetMap[firstLetterOfWord].isAWord(word.substr(1, word.length()));
    else
      isWord = false;
  }
  return isWord;
}

/*
A method allWordsStartingWithPrefix that accepts a std::string and returns
a vector<std::string> that contains all the words in the Trie that start
with the passed in argument prefix string. A word that is just the prefix
is acceptable. An empty prefix should return all words in the Trie.
*/
vector<string> Trie::allWordsStartingWithPrefix(string prefix)
{
  vector<string> vectorOfWords;
  //use a pointer to traverse trie to reduce time complexity
  Trie* currentNode = this;

  if(!prefix.empty())
  {
    //traverse prefix
    for(auto const& letterInWord : prefix)
    {
      //if branch exists set a new ptr
      if(currentNode->alphabetMap.find(letterInWord) != currentNode->alphabetMap.end())
        currentNode = &(currentNode->alphabetMap[letterInWord]);
      else
        return vectorOfWords; //cut if branch doesn't exist
    }

    //if prefix is a word, add it
    if(currentNode->isEndOfWord)
      vectorOfWords.push_back(prefix);
  }
  //recursively gather all remaining words
  currentNode->recursiveAllWordsStartingWithPrefix(prefix, vectorOfWords);

  return vectorOfWords;
}


//helper to recursively grab words from trie
//takes in the prefix and the vector of strings to return
void Trie::recursiveAllWordsStartingWithPrefix(string word, vector<string>& vectorOfWords)
{
  //go through every possible branch w/out the const& type def bc we pass it recurseivley
  for(auto currNode : alphabetMap)
  {
    //create postfix for each root in new trie
    string postfix = "";
    //add branch name to word
    postfix.push_back(currNode.first);
    //if at end of branch, add word to vector
    if(currNode.second.isEndOfWord)
      vectorOfWords.push_back(word + postfix);

    //recuse to pull all words from Trie
    currNode.second.recursiveAllWordsStartingWithPrefix((word + postfix), vectorOfWords);
  }
}
