#include "algotrainer.h"
#include "algosolver.h"
#include <QTimer>
#include <QPoint>
#include <cmath>
#include <algorithm>

void AlgoTrainer::startSelectionDemo()
{
    demoMode = true;
    int array[16] = {1, 15, 12, 3, 2, 4, 7, 14, 13, 16, 9, 11, 5, 6, 8, 10};
    currentArraySequence = AlgoSolver::getSelectionSortSequence(array, 16);
    lastArray = currentArraySequence.front();
    currentArraySequence.pop_front();

    emit displayArray(AlgoSolver::vectorToArray(lastArray));
    emit changeBoxColor(4, QColor("#ff1654"));
    emit changeBoxColor(1, QColor("#02c39a"));
    emit updateProgress(0);
    emit displayMessage(200, 400, "Welcome to selection sort! \n\n (click here to continue)");
}

void AlgoTrainer::startInsertionDemo()
{
    demoMode = true;
    int array[16] = {1, 15, 12, 3, 2, 4, 7, 14, 13, 16, 9, 11, 5, 6, 8, 10};
    currentArraySequence = AlgoSolver::getInsertionSortSequence(array, 16);
    lastArray = currentArraySequence.front();
    currentArraySequence.pop_front();
    createInsertSeq();

    emit displayArray(AlgoSolver::vectorToArray(lastArray));
    emit changeBoxColor(2, QColor("#ff1654"));
    emit changeBoxColor(1, QColor("#02c39a"));
    emit updateProgress(0);
    emit displayMessage(200, 400, "Welcome to insertion sort! \n\n (click here to continue)");
}

void AlgoTrainer::startMergeDemo()
{
    demoMode = true;
    int array[16] = {1, 15, 12, 3, 2, 4, 7, 14, 13, 16, 9, 11, 5, 6, 8, 10};
    currentArraySequence = AlgoSolver::getMergeSortSequence(array, 16);
    lastArray = currentArraySequence.front();
    currentArraySequence.pop_front();

    emit displayArray(AlgoSolver::vectorToArray(lastArray));
    emit changeBoxColor(0, QColor("#ff1654"));
    emit changeBoxColor(1, QColor("#02c39a"));
    emit updateProgress(0);
    emit displayMessage(200, 400, "Welcome to merge sort! \n\n (click here to continue)");
}

void AlgoTrainer::setAlgorithm(AlgoTrainer::Algorithm algorithm)
{
    currentAlgorithm = algorithm;
    // Restart colors for blocks.
    for (int i = 0; i < 16; i++)  {
        emit changeBoxColor(i, QColor("#007ea7"));
    }
}
void AlgoTrainer::startAlgorithm()
{
    demoMessageIndex = 0;
    switch(currentAlgorithm)
    {
    case Algorithm::SELECTIONDEMO:
        startSelectionDemo();
       break;
    case Algorithm::INSERTIONDEMO:
        startInsertionDemo();
        break;
    case Algorithm::MERGEDEMO:
        startMergeDemo();
        break;
    }
}
void AlgoTrainer::continueDemo(int* array)
{
    switch(currentAlgorithm)
    {
    case Algorithm::SELECTIONDEMO:
        arrayChangedSelectedSortDemoMode(array);
        break;
    case Algorithm::INSERTIONDEMO:
        arrayChangedInsertionSortDemoMode(array);
        break;
    case Algorithm::MERGEDEMO:
        arrayChangedMergeSortDemoMode(array);
        break;
    }
}
void AlgoTrainer::bypassDemoAlgorithm()
{
    demoMessageIndex = 100;
    demoMode=false;
    switch(currentAlgorithm)
    {
    case Algorithm::SELECTIONDEMO:
    {
        startSelectionSort();
        emit resetBoxColors();
       break;
    }
    case Algorithm::INSERTIONDEMO:
    {
        startInsertionSort();

        emit resetBoxColors();
        break;
    }
    case Algorithm::MERGEDEMO:
    {   demoMode = false;
        emit resetMerge();
        startMergeSort();       //change to demo once method is created
        emit resetBoxColors();
        break;
    }
    }
}

void AlgoTrainer::arrayChangedSelectedSortDemoMode(int* array)
{
    if (currentArraySequence.size() > 0)
    {
        std::vector<int> vec = AlgoSolver::arrayToVector(array, 16);

        // If they didn't move anything, do nothing.
        if(vec == lastArray)
            return;
        // If the changed array corresponds the next array state in the sequence,
        // we increment the array sequence, and update the progress bar.
        if (vec == currentArraySequence.front())
        {
            lastArray = currentArraySequence.front();
            currentArraySequence.pop_front();
            //emit updateProgress(static_cast<int>(floor((100.0*(initialSequenceSize-currentArraySequence.size())/initialSequenceSize))));

            if (currentArraySequence.size() == 11)
            {
                prevBeginIndex=4;
                prevEndIndex=1;
                selectionDemoColorChange(3, 2);
            }
            else if (currentArraySequence.size() == 10)
            {
                selectionDemoColorChange(5,3);
            }
            else if (currentArraySequence.size() == 9)
            {
                selectionDemoColorChange(12,4);
            }
            else if (currentArraySequence.size() == 8)
            {
                selectionDemoColorChange(13,5);
            }
            else if (currentArraySequence.size() == 7)
            {
                selectionDemoColorChange(14,7);
            }
            else if (currentArraySequence.size() == 6)
            {
                selectionDemoColorChange(10, 8);
            }
            else if (currentArraySequence.size() == 5)
            {
                selectionDemoColorChange(15,9);
            }
            else if(currentArraySequence.size() == 4)
            {
                selectionDemoColorChange(11,10);
            }
            else if(currentArraySequence.size() ==3)
            {
                selectionDemoColorChange(13, 11);
            }
            else if(currentArraySequence.size() ==2)
            {
                selectionDemoColorChange(13,12);
            }
            else if(currentArraySequence.size() ==1)
            {
                selectionDemoColorChange(14, 13);
            }
            else if(currentArraySequence.size() ==0)
            {
                emit displayMessage(500, 100, "You did it!");
                emit resetBoxColors();
                emit celebrate();
                demoMode = false;
                startSelectionSort();
            }
        }
        else
        {
            // Otherwise, we revert to the last array, and warn them of the error.
            emit displayArray(AlgoSolver::vectorToArray(lastArray));
        }
    }
    else if (lastArray.size() == 16)
    {
        emit displayArray(AlgoSolver::vectorToArray(lastArray));
    }
}

void AlgoTrainer::arrayChangedInsertionSortDemoMode(int* array)
{
    if (currentArraySequence.size() > 0)
    {
        std::vector<int> vec = AlgoSolver::arrayToVector(array, 16);

        if(vec == lastArray)
        {
            return;
        }

        if (vec == currentArraySequence.front())
        {
            lastArray = currentArraySequence.front();
            currentArraySequence.pop_front();

            if(currentArraySequence.size() == 0)
            {
                emit displayMessage(500, 100, "You did it!");
                emit celebrate();
                emit resetBoxColors();
                demoMode = false;
                startInsertionSort();

            }
            else
            {
                emit resetBoxColors();
                insertDemoSeq.pop_back();

                emit changeBoxColor(insertDemoSeq.back().first, QColor("#ff1654"));
                emit changeBoxColor(insertDemoSeq.back().second, QColor("#02c39a"));
                insertDemoSeq.pop_back();
            }
        }
        else
        {
            // Otherwise, we revert to the last array, and warn them of the error.
            emit displayArray(AlgoSolver::vectorToArray(lastArray));
        }
    }
    else if (lastArray.size() == 16)
    {
        emit displayArray(AlgoSolver::vectorToArray(lastArray));
    }
}

void AlgoTrainer::arrayChangedMergeSortDemoMode(int* array)
{
    if (currentArraySequence.size() > 0)
    {
        std::vector<int> vec = AlgoSolver::arrayToVector(array, 16);

        // If the changed array corresponds the next array state in the sequence,
        // we increment the array sequence, and update the progress bar.
        if (vec == currentArraySequence.front())
        {
            lastArray = currentArraySequence.front();
            currentArraySequence.pop_front();

            if(currentAlgorithm == MERGEDEMO)
            {
                resetBoxColors();
                if (currentArraySequence.size() != 0)
                {
                    emit accept();

                }
                else
                    resetMerge();
            }

            if (currentArraySequence.size() == 0)
            {

                emit displayMessage(500, 500, "You did it!");
                emit celebrate();
                emit resetBoxColors();
                demoMode = false;
                startMergeSort();
            }
        }
        else
        {
            // Otherwise, we revert to the last array, and warn them of the error.
            emit displayArray(AlgoSolver::vectorToArray(lastArray));
            // Display a message with x, y, message.
            if (currentAlgorithm == MERGEDEMO)
                emit displayMessage(430, 400, "That's not right! Try again.");
            else
                emit displayMessage(500, 500, "That's not right! Try again.");
        }
    }
    else if (lastArray.size() == 16)
    {
        emit displayArray(AlgoSolver::vectorToArray(lastArray));
    }
}

void AlgoTrainer::selectionDemoColorChange(int startIndex, int endIndex)
{
    emit changeBoxColor(prevBeginIndex, QColor("#007ea7"));
    emit changeBoxColor(prevEndIndex, QColor("#007ea7"));
    emit changeBoxColor(startIndex, QColor("#ff1654"));
    emit changeBoxColor(endIndex, QColor("#02c39a"));
    prevBeginIndex = startIndex;
    prevEndIndex = endIndex;
}

void AlgoTrainer::startSelectionSort()
{
    demoMode = false;
    int array[16] = {10, 5, 12, 3, 7, 4, 2, 8, 13, 16, 9, 6, 15, 11, 14, 1};
    random_shuffle(begin(array), end(array));
    currentArraySequence = AlgoSolver::getSelectionSortSequence(array, 16);

    // Keep the initialSequenceSize to update the progress bar correctly.
    initialSequenceSize = currentArraySequence.size();

    // We want to keep the array state of that which we just displayed.
    lastArray = currentArraySequence.front();
    currentArraySequence.pop_front();

    // This tells ExerciseView to display the first array state.
    emit displayArray(AlgoSolver::vectorToArray(lastArray));
    emit updateProgress(0);
    emit displayMessage(420, 500, "Try it yourself!");
}

void AlgoTrainer::startInsertionSort()
{
    // Just like above, but we call AlgoSolver for insertion sort.
    int array[16] = {14, 9, 15, 8, 5, 3, 6, 12, 13, 10, 1, 16, 4, 2, 11, 7};
    random_shuffle(begin(array), end(array));

    currentArraySequence = AlgoSolver::getInsertionSortSequence(array, 16);
    initialSequenceSize = currentArraySequence.size();
    lastArray = currentArraySequence.front();
    currentArraySequence.pop_front();

    emit displayArray(AlgoSolver::vectorToArray(lastArray));
    emit displayMessage(420, 500, "Try it yourself!");
    emit updateProgress(0);
}
void AlgoTrainer::startMergeSort()
{
    // Just like above, but we call AlgoSolver for merge sort.
    int array[16] = {15, 10, 14, 7, 5, 11, 16, 6, 3, 1, 4, 13, 8, 2, 9, 12};
    random_shuffle(begin(array), end(array));

    currentArraySequence = AlgoSolver::getMergeSortSequence(array, 16);
    initialSequenceSize = currentArraySequence.size();
    lastArray = currentArraySequence.front();

    emit displayArray(AlgoSolver::vectorToArray(lastArray));
    currentArraySequence.pop_front();

    emit displayMessage(430, 400, "Try it yourself!");
    emit updateProgress(0);
}



void AlgoTrainer::arrayChanged(int* array)
{
    if(demoMode){
        continueDemo(array);
    }
    else
    {
        if (currentArraySequence.size() > 0)
        {
            std::vector<int> vec = AlgoSolver::arrayToVector(array, 16);

            // If the changed array corresponds the next array state in the sequence,
            // we increment the array sequence, and update the progress bar.
            if (vec == currentArraySequence.front())
            {
                hideMessage();
                lastArray = currentArraySequence.front();
                currentArraySequence.pop_front();
                emit updateProgress(static_cast<int>(floor((100.0*(initialSequenceSize-currentArraySequence.size())/initialSequenceSize))));

                if(currentAlgorithm == MERGEDEMO)
                {
                    resetBoxColors();
                    if (currentArraySequence.size() != 0)
                    {
                        emit accept();

                    }
                    else
                        resetMerge();
                }

                if (currentArraySequence.size() == 0)
                {

                    emit displayMessage(500, 500, "You did it!   \n\n\n\n\n\n (click here to dismiss.)");
                    emit celebrate();
                    demoMode =true;
                }
            }
            else
            {
                // Otherwise, we revert to the last array, and warn them of the error.
                emit displayArray(AlgoSolver::vectorToArray(lastArray));
                // Display a message with x, y, message.
                if (currentAlgorithm == MERGEDEMO)
                    emit displayMessage(430, 400, "That's not right! Try again.");
                else
                    emit displayMessage(500, 500, "That's not right! Try again.");
            }
        }
        else if (lastArray.size() == 16)
        {
            emit displayArray(AlgoSolver::vectorToArray(lastArray));
        }
    }

}

void AlgoTrainer::createInsertSeq()
{
    int xBox[24] = {1, 3, 2, 4, 1, 5, 5, 6, 5, 7, 7, 8, 7, 10, 10, 11, 6, 12, 4, 13, 6, 14, 8, 15};
    int yBox[24] = {2, 1, 3, 1, 2, 3, 3, 4, 4, 6, 6, 6, 6, 5, 5, 6, 7, 4, 12, 5, 5, 7, 7, 9};

    for(int index = 23; index >= 0; index--)
    {
        insertDemoSeq.push_back(qMakePair(xBox[index], yBox[index]));
    }
}

std::vector<std::string> selectionIntroduction_text{
   "Selection sort is an in-place comparison-based algorithm in which the list is divided into two parts, the sorted part at the left end and the unsorted part at the right end. \n\n (Click here to continue)",
    "Initially, the sorted part is empty and the unsorted part is the entire list. \n\n (Click here to continue)",
    "The smallest element is selected from the unsorted array and swapped with the leftmost element, and that element becomes a part of the sorted array. \n\n (Click here to continue)",
    "This process continues moving unsorted array boundary by one element to the right. \n\n (Click here to continue)",
    "Selection sort works through a swapping mechanism. Swap the red block with the green block. \n\n (Click here to continue)",
    "Grab the red block and swap it with the green block. \n\n (Click here to continue)",
    "Now finish sorting the algorithm yourself! \n Swap red blocks with green blocks till done!"
};

std::vector<std::string> inserstionIntroduction_text{
    "Insertion sort is an in-place algorithm that builds the final sorted array one element at a time \n\n (Click here to continue)",
    "Insertion sort will iterate through the unsorted array one element at a time and move the element to its corresdonding place in the sorted array \n\n (Click here to continue)",
    "After the algorithm iterate through each element the array will be sorted \n\n (Click here to continue)",
    "Start by selecting the red block and shift it to the green block \n\n (Click here to continue)",
    "The red block will be the next element from the original array and the green block will be the location for the red block in the sorted array \n\n (Click here to continue)",
    "Repeat this process until you have iterated through all of the elements and the final array is sorted \n\n (Click here to continue)"
};

std::vector<std::string> mergesortIntroduction_text
{
    "Merge sort is a general comparison-based sorting algorithm that divides the array into n sub arrays \n\n (Click here to continue)",
    "Initially the array is recursively split into single size sub arrays \n\n (Click here to continue)",
    "The arrays will merge together in order back into the parent array by comparing each element of the arrays \n\n (Click here to continue)",
    "Mergesort works by repeatedly sorting these sub arrays until there is only one array remaining \n\n (Click here to continue)",
    "Start by clicking the red block to sort it back into the correct location in order \n\n (Click here to continue)",
    "Continue selecting the smallest block of each sub array till there are none left! \n\n (Click here to continue)",
};

std::vector<QPoint> selectionIntroduction_text_location{
    QPoint(200, 400),
    QPoint(440, 670),
    QPoint(685, 645),
    QPoint(440, 645),
    QPoint(685, 645),
    QPoint(685, 645),
    QPoint(650, 660),
    QPoint(650, 660),
};

std::vector<QPoint> inserstionIntroduction_text_location
{
    QPoint(200, 550),
    QPoint(500, 650),
    QPoint(560, 550),
    QPoint(560, 550),
    QPoint(620, 640),
    QPoint(200, 550)
};

std::vector<QPoint> mergesortIntroduction_text_location
{
    QPoint(200, 400),
    QPoint(200, 400),
    QPoint(1020, 240),
    QPoint(1020, 240),
    QPoint(1020, 240),
    QPoint(1020, 240),
};

/*
 *  userFinishedPrompt() manages the text that is shown or not shown to the user depending on which mode that they are in.
 */
void AlgoTrainer::userFinishedPrompt() {
    if(demoMode == true && currentAlgorithm == Algorithm::SELECTIONDEMO) {
        if (demoMessageIndex< selectionIntroduction_text.size()) {
            int x = selectionIntroduction_text_location[demoMessageIndex].x();
            int y = selectionIntroduction_text_location[demoMessageIndex].y();
            emit displayMessage(x, y, selectionIntroduction_text[demoMessageIndex]);
            demoMessageIndex++;
        }
    }
    else if(demoMode == true && currentAlgorithm == Algorithm::INSERTIONDEMO)
    {
        if (demoMessageIndex< inserstionIntroduction_text.size()) {
            int x = inserstionIntroduction_text_location[demoMessageIndex].x();
            int y = inserstionIntroduction_text_location[demoMessageIndex].y();
            emit displayMessage(x, y, inserstionIntroduction_text[demoMessageIndex]);
            demoMessageIndex++;
        }
    }
    else if(demoMode == true && currentAlgorithm == Algorithm::MERGEDEMO)
    {
        if (demoMessageIndex< mergesortIntroduction_text.size()) {
            int x = mergesortIntroduction_text_location[demoMessageIndex].x();
            int y = mergesortIntroduction_text_location[demoMessageIndex].y();
            emit displayMessage(x, y, mergesortIntroduction_text[demoMessageIndex]);
            demoMessageIndex++;
        }
    }
}
