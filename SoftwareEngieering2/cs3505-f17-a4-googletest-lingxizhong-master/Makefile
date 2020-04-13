# Osama Kergaye and Ling
# 3505
# u0767339
# Testing and Travis
GOOGLETEST ?= ../googletest/googletest
CC = g++
CFLAGS = -Wall -std=c++11
StudentsTests: StudentsTests.cpp
	$(CC) $(CFLAGS) -o StudentsTests StudentsTests.cpp Students.o -I$(GOOGLETEST)/include  -L$(GOOGLETEST) -lgtest -lpthread

clean:
	rm StudentsTests
#
test: StudentsTests
	./StudentsTests
