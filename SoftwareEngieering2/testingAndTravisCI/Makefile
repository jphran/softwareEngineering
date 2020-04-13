CC = g++
CFLAGS = -std=c++11 -g #-fsanitize=address -fsanitize=undefined
GOOGLETEST ?= ../googletest/googletest

# Create the executable file
TrieTests: Trie.o TrieTests.cpp
	$(CC) $(CFLAGS) -o TrieTests TrieTests.cpp Trie.o -I$(GOOGLETEST)/include -L$(GOOGLETEST)/lib -lgtest -lgtest_main -lpthread

# Generate the Trie.o file
Trie.o: Trie.h Trie.cpp
	$(CC) $(CFLAGS) -I$(GOOGLETEST)/include -L$(GOOGLETEST)/lib -c Trie.cpp

clean:
	rm -f TrieTests *.o

test:
	./TrieTests
