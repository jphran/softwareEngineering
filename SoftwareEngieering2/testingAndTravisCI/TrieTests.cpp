/*
Auth: Justin Francis & Palani Thangaraj
Class: CS3505
Assign: A4: Refactoring and Testing
Created: 9/26/19
Version: 0.1
Modified:

This class implements gtest to thoroughly test our Trie Class

*/

#include "Trie.h"
#include "gtest/gtest.h"
#include <vector>
#include <string>
using namespace std;

//********************************IS WORD TESTS***************************

//determine if an empty Trie contains anything or empty string
TEST (isAWordTest, emptyTrie)
{
  Trie t = Trie();

  ASSERT_FALSE (t.isAWord("test"));
  ASSERT_FALSE(t.isAWord(""));
}

//detemine if affirmative base case works
TEST(isAWordTest, existingWord)
{
  Trie t = Trie();
  t.addAWord("exist");

  ASSERT_TRUE(t.isAWord("exist"));
}

//determine if negative base case works
TEST(isAWordTest, notAWord)
{
  Trie t = Trie();
  t.addAWord("word");

  ASSERT_TRUE(t.isAWord("word"));
  ASSERT_FALSE(t.isAWord("notword"));
  ASSERT_FALSE(t.isAWord("w"));
}

//determine if edge cases pass, testing empty string
TEST(isAWordTest, edgeCase)
{
  Trie t = Trie();
  t.addAWord("");

  ASSERT_FALSE(t.isAWord(""));
}


//********************************ADD WORD TESTS*************************
//affirmative base case
TEST (addAWordTest, addSingleWord)
{
  Trie t = Trie();
  string word = "test";
  t.addAWord(word);

  ASSERT_TRUE (t.isAWord(word));
}

//robust affirmative base case
TEST (addAWordTest, addMultipleWords)
{
  Trie t = Trie();
  t.addAWord("testa");
  t.addAWord("testb");
  t.addAWord("testc");

  ASSERT_TRUE (t.isAWord("testa"));
  ASSERT_TRUE (t.isAWord("testb"));
  ASSERT_TRUE (t.isAWord("testc"));
}

//negative base case
TEST (addAWordTest, checkWrongWord)
{
  Trie t = Trie();
  t.addAWord("tes");
  t.addAWord("testa");
  t.addAWord("testb");
  t.addAWord("testc");

  ASSERT_FALSE (t.isAWord("test"));
}

//testing the prefix fn with multiple words to ensure roustness
TEST (addAWordTest, checkPrefixMultipleWords3)
{
  Trie t = Trie();
  t.addAWord("cat");
  t.addAWord("car");
  t.addAWord("robust");
  t.addAWord("shouldnotbeadded");

  vector<string> input;

  input.push_back("car");
  input.push_back("cat");


  vector<string> output = t.allWordsStartingWithPrefix("ca");

  ASSERT_TRUE (input == output);
}

//edge cases testing duplicates and empty strings
TEST(addAWordTest, addEmptyString)
{
  Trie t =  Trie();
  t.addAWord("");

  vector<string> output = t.allWordsStartingWithPrefix("");

  EXPECT_EQ(0, output.size());
}

//edge cases
TEST(addAWordTest, addSameWords)
{
  Trie t =  Trie();
  t.addAWord("test");
  t.addAWord("test");

  vector<string> output = t.allWordsStartingWithPrefix("");

  EXPECT_EQ(1, output.size());
}

//*****************************RULE OF THREE TESTS ****************************

//test constructor fn on stack
TEST(constructorTest, stackTest)
{
  Trie t;

  ASSERT_FALSE(t.isAWord(""));

  t.addAWord("stack");

  ASSERT_TRUE(t.isAWord("stack"));
}

//tests cstr fn on heap (this create memory leaks bc we have no destructor)
TEST(constructorTest, heapTest)
{
  Trie* t = new Trie();

  ASSERT_FALSE(t->isAWord(""));

  t->addAWord("heap");

  ASSERT_TRUE(t->isAWord("heap"));

  delete t;
}

//we do not have an implemented copy construct nor destructor nor assignment op
//they are are not necessary bc nothing is placed on the heap and we only need
//a shallow copy of the map
