#include "algotrainer.h"
#include "algosolver.h"
#include <QObject>
#include <QColor>
#include <QTimer>
#include <cmath>
#include <QEventLoop>
#include <QPoint>


//class AlgoTrainer::ExerciseStep
//{
//public:
//    ExerciseStep(int startIndex, int endIndex)
//    {
//        startBoxIndex = startIndex;
//        endBoxIndex = endIndex;
//        message = "Grab the green rectangle and swap it with the orange rectangle.";
//    }

//    QPoint *msgBoxPos;
//    QString message;
    // The index in which the highlight method will traverse to
 //   int startBoxIndex;
 //   int endBoxIndex;
//};

AlgoTrainer::AlgoTrainer()
{

}

void AlgoTrainer::startSelectionDemo()
{
    int array[16] = {1, 15, 12, 3, 2, 4, 7, 14, 13, 16, 9, 11, 5, 6, 8, 10};
    currentArraySequence = AlgoSolver::getSelectionSortSequence(array, 16);
   // while (currentArraySequence.size()>7)
    {
     //   currentArraySequence.pop_front();
    }
    lastArray = currentArraySequence.front();
    currentArraySequence.pop_front();

    emit displayArray(AlgoSolver::vectorToArray(lastArray));
    emit changeBoxColor(4, Qt::magenta);
    emit changeBoxColor(1, Qt::cyan);
    emit updateProgress(0);

    emit displayMessage(200, 400, "Welcome to selection sort! \n\n (click here to continue)");
}

void AlgoTrainer::startInsertionDemo()
{
    int array[16] = {1, 15, 12, 3, 2, 4, 7, 14, 13, 16, 9, 11, 5, 6, 8, 10};
    currentArraySequence = AlgoSolver::getInsertionSortSequence(array, 16);
    lastArray = currentArraySequence.front();
    currentArraySequence.pop_front();
    createInsertSeq();

    emit displayArray(AlgoSolver::vectorToArray(lastArray));
    emit changeBoxColor(1, Qt::magenta);
    emit changeBoxColor(2, Qt::cyan);
    emit updateProgress(0);
    emit displayMessage(200, 400, "Welcome to selection sort! \n\n (click here to continue)");
}

void AlgoTrainer::setAlgorithm(AlgoTrainer::Algorithm algorithm)
{
    currentAlgorithm = algorithm;
    // Restart colors for blocks.
    for (int i = 0; i < 16; i++)  {
        emit changeBoxColor(i, Qt::yellow);
    }
}

void AlgoTrainer::startAlgorithm()
{
    switch(currentAlgorithm)
    {
    case Algorithm::SELECTIONDEMO:
        if(demoMode)
            startSelectionDemo();
        else
            startSelectionSort();
        break;
    case Algorithm::INSERTIONDEMO:
        if(demoMode)
            startInsertionDemo();
        else
            startInsertionSort();
        break;
    case Algorithm::MERGE:
        startMergeSort();
        break;
    }
}

void AlgoTrainer::continueDemo(int* array)
{
    switch(currentAlgorithm)
    {
    case Algorithm::SELECTIONDEMO:
        arrayChangedDemoMode(array);
        break;
    case Algorithm::INSERTIONDEMO:
        insertionSortDemoMode(array);
        break;
    case Algorithm::MERGE:
        startMergeSort();
        break;
    }
}

void AlgoTrainer::startSelectionSort()
{
    // Here we initalize the array that we will always use for selection sort.
    // And we get the sorting sequence from AlgoSolver.
    int array[16] = {10, 5, 12, 3, 7, 4, 2, 8, 13, 16, 9, 6, 15, 11, 14, 1};
    currentArraySequence = AlgoSolver::getSelectionSortSequence(array, 16);

    // Keep the initialSequenceSize to update the progress bar correctly.
    initialSequenceSize = currentArraySequence.size();

    // We want to keep the array state of that which we just displayed.
    lastArray = currentArraySequence.front();
    currentArraySequence.pop_front();

    // This tells ExerciseView to display the first array state.
    emit displayArray(AlgoSolver::vectorToArray(lastArray));

    emit updateProgress(0);
    emit displayMessage(500, 100, "Try it yourself!");
}

void AlgoTrainer::startInsertionSort()
{
    // Just like above, but we call AlgoSolver for insertion sort.
    int array[16] = {14, 9, 15, 8, 5, 3, 6, 12, 13, 10, 1, 16, 4, 2, 11, 7};

    currentArraySequence = AlgoSolver::getInsertionSortSequence(array, 16);
    initialSequenceSize = currentArraySequence.size();
    lastArray = currentArraySequence.front();
    currentArraySequence.pop_front();

    emit displayArray(AlgoSolver::vectorToArray(lastArray));
    emit displayMessage(500, 100, "Try it yourself!");
    emit updateProgress(0);
}


void AlgoTrainer::arrayChangedDemoMode(int* array)
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
                emit changeBoxColor(14, Qt::yellow);
                emit changeBoxColor(13, Qt::yellow);
                emit displayMessage(500, 100, "You did it!");
                emit celebrate();
                demoMode = false;
                startSelectionSort();
            }
        }
        else
        {
            // Otherwise, we revert to the last array, and warn them of the error.
            emit displayArray(AlgoSolver::vectorToArray(lastArray));
            // Display a message with x, y, message.
          //  emit displayMessage(500, 100, "That's not right! Try again.");
        }
    }
    else if (lastArray.size() == 16)
    {
        emit displayArray(AlgoSolver::vectorToArray(lastArray));
    }
}

void AlgoTrainer::insertionSortDemoMode(int* array)
{
    if (currentArraySequence.size() > 0)
    {
        std::vector<int> vec = AlgoSolver::arrayToVector(array, 16);

        if(vec == lastArray)
            return;

        if (vec == currentArraySequence.front())
        {
            lastArray = currentArraySequence.front();
            currentArraySequence.pop_front();

            if(currentArraySequence.size() == 0)
            {
                emit displayMessage(500, 100, "You did it!");
                emit celebrate();
                demoMode = false;
                startInsertionSort();

            } else {

                emit changeBoxColor(insertDemoSeq.back().first, Qt::yellow);
                emit changeBoxColor(insertDemoSeq.back().second, Qt::yellow);
                insertDemoSeq.pop_back();

                emit changeBoxColor(insertDemoSeq.back().first, Qt::magenta);
                emit changeBoxColor(insertDemoSeq.back().second, Qt::cyan);
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

void AlgoTrainer::selectionDemoColorChange(int startIndex, int endIndex)
{
    emit changeBoxColor(prevBeginIndex, Qt::yellow);
    emit changeBoxColor(prevEndIndex, Qt::yellow);
    emit changeBoxColor(startIndex, Qt::magenta);
    emit changeBoxColor(endIndex, Qt::cyan);
    prevBeginIndex = startIndex;
    prevEndIndex = endIndex;
}

void AlgoTrainer::startMergeSort()
{
    // Just like above, but we call AlgoSolver for merge sort.
    int array[16] = {15, 10, 14, 7, 5, 11, 16, 6, 3, 1, 4, 13, 8, 2, 9, 12};
    currentArraySequence = AlgoSolver::getMergeSortSequence(array, 16);
    AlgoSolver::printSequence(currentArraySequence);
    initialSequenceSize = currentArraySequence.size();
    lastArray = currentArraySequence.front();
    emit displayArray(AlgoSolver::vectorToArray(lastArray));
    currentArraySequence.pop_front();
    emit displayMessage(500, 500, "Try it yourself! \n\n\n\n\n\n (click here to continue)");
    emit updateProgress(0);
}

void AlgoTrainer::arrayChanged(int* array)
{
    if(demoMode)
    {
        //arrayChangedDemoMode(array);
        // insertionSortDemoMode(array);
        continueDemo(array);
    }
    else
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
                emit updateProgress(static_cast<int>(floor((100.0*(initialSequenceSize-currentArraySequence.size())/initialSequenceSize))));
                if (currentArraySequence.size() == 0)
                {
                    emit displayMessage(500, 100, "You did it!");
                    emit celebrate();
                }
                else
                    emit displayMessage(500, 100, "So far so good...");
            }
            else
            {
                // Otherwise, we revert to the last array, and warn them of the error.
                emit displayArray(AlgoSolver::vectorToArray(lastArray));
                // Display a message with x, y, message.
                emit displayMessage(500, 100, "That's not right! Try again.");
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
    "Selection sort works through a swapping mechanism. Swap the magenta block with the cyan block. \n\n (Click here to continue)",
    "Grab the magenta block and swap it with the cyan block."
};

std::vector<QPoint> selectionIntroduction_text_location{
    QPoint(200, 400),
    QPoint(500, 680),
    QPoint(720, 660),
    QPoint(500, 680),
    QPoint(720, 660),
};

/*
 *  userFinishedPrompt() manages the text that is shown or not shown to the user depending on which mode that they are in.
 */
void AlgoTrainer::userFinishedPrompt() {
    static unsigned messageIndex;
    if(demoMode == true && currentAlgorithm == Algorithm::SELECTIONDEMO) {
        if (messageIndex < selectionIntroduction_text.size()) {
            int x = selectionIntroduction_text_location[messageIndex].x();
            int y = selectionIntroduction_text_location[messageIndex].y();
            emit displayMessage(x, y, selectionIntroduction_text[messageIndex]);
            messageIndex++;
        }
    }
}
