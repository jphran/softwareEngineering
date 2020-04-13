#ifndef ALGOSOLVER_H
#define ALGOSOLVER_H

#include <deque>
#include <vector>

using namespace std;

class AlgoSolver
{
public:
    AlgoSolver() {}

    static deque<vector<int>> getSelectionSortSequence(int inputArray[], int arraySize);
    static deque<vector<int>> getInsertionSortSequence(int inputArray[], int arraySize);
    static deque<vector<int>> getMergeSortSequence(int inputArray[], int arraySize);
    static void printSequence(deque<vector<int>> sequence);
    static vector<int> arrayToVector(int array[], int arraySize);
    static int* vectorToArray(vector<int> vec);
    static int* v2a(vector<int> v);

private:
    static int min(int x, int y);
    static vector<int> merge(vector<int> vec1, vector<int> vec2);
};

#endif // ALGOSOLVER_H
