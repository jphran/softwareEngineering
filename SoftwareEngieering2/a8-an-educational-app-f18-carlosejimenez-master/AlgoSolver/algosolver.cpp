#include "algosolver.h"
#include <cmath>
#include <vector>
#include <cstdint>
#include <limits>
#include <iostream>

using namespace std;

vector<int> AlgoSolver::arrayToVector(int array[], int arraySize)
{
    vector<int> vec(arraySize);
    for (int i = 0; i < arraySize; i++)
        vec[i] = array[i];
    return vec;
}

int* AlgoSolver::vectorToArray(vector<int> vec) {
    int* array = new int[vec.size()];

    for (int i = 0; i < vec.size(); i++)
        array[i] = vec[i];

    return array;
}

void AlgoSolver::printSequence(deque<vector<int>> sequence)
{
    for(int h = 0; h < sequence.size(); h++)
    {
       for(int i = 0; i < sequence[h].size(); i++)
            cout << sequence[h][i] << ' ';
            cout << endl;
    }
}

deque<vector<int>> AlgoSolver::getSelectionSortSequence(int inputArray[], int arraySize)
{
    deque<vector<int>> sequenceVector;
    vector<int> first = arrayToVector(inputArray, arraySize);
    sequenceVector.push_back(first);

    for (int j = 0; j < arraySize-1; j++)
    {
        int iMin = j;
        for (int i = j+1; i < arraySize; i++)
            if (inputArray[i] < inputArray[iMin])
                iMin = i;

        if (iMin != j)
        {
            swap(inputArray[j], inputArray[iMin]);
            vector<int> vec = arrayToVector(inputArray, arraySize);
            sequenceVector.push_back(vec);
        }
    }
    return sequenceVector;
}

deque<vector<int>> AlgoSolver::getInsertionSortSequence(int inputArray[], int arraySize)
{
    deque<vector<int>> sequenceVector;
    vector<int> first = arrayToVector(inputArray, arraySize);
    sequenceVector.push_back(first);

    for (int i = 1; i < arraySize; i++)
    {
        int key_index = i;
        int key = inputArray[key_index];
        for(int j = i-1; j >= 0; j--)
        {
            if (inputArray[j] > inputArray[key_index])
            {
                swap(inputArray[j], inputArray[key_index]);
                key_index = j;
            }
        }
        vector<int> vec = arrayToVector(inputArray, arraySize);
        if (vec != sequenceVector.back())
            sequenceVector.push_back(vec);
    }
    return sequenceVector;
}

int AlgoSolver::min(int x, int y)
{
    return (x < y) ? x : y;
}

vector<int> AlgoSolver::merge(vector<int> vec1, vector<int> vec2)
{
    if (vec1.size() == 0)
        return vec2;
    if (vec2.size() == 0)
        return vec1;

    vector<int> mVec;
    int finalSize = vec1.size() + vec2.size();

    vec1.push_back(INT32_MAX);
    vec2.push_back(INT32_MAX);

    int i1 = 0;
    int i2 = 0;
    for (int i = 0; i < finalSize; i++)
    {
        if (vec1[i1] < vec2[i2])
            mVec.push_back(vec1[i1++]);
        else
            mVec.push_back(vec2[i2++]);
    }
    return mVec;
}

deque<vector<int>> AlgoSolver::getMergeSortSequence(int inputArray[], int arraySize)
{
    deque<bool> stages;
    for (int i = arraySize; i > 0; i=i/2)
        stages.push_front(false);

    deque<vector<int>> sequenceVector;
    vector<int> first = arrayToVector(inputArray, arraySize);
    sequenceVector.push_back(first);

    vector<int> vec1;
    vector<int> vec2;
    vector<int> mergedVec;

    for (int step = 1; step < arraySize; step*=2)
    {
        for (int i = 0; i < arraySize; i += 2*step)
        {
            for (int j = i; j < min(arraySize, i + step); j++)
                vec1.push_back(first[j]);
            for (int j = i + step; j < min(arraySize, i + 2*step); j++)
                vec2.push_back(first[j]);
            mergedVec = merge(vec1, vec2);
            for (int j = 0; j < mergedVec.size(); j++)
                first[i + j] = mergedVec[j];
            sequenceVector.push_back(first);
            vec1.clear();
            vec2.clear();
            mergedVec.clear();
        }
    }
    return sequenceVector;
}
