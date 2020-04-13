#ifndef ALGOTRAINER_H
#define ALGOTRAINER_H

#include <QObject>
#include <QColor>
#include <string>
#include <vector>
#include <deque>

class AlgoTrainer : public QObject
{
    Q_OBJECT

public:
    enum Algorithm { SELECTION, SELECTIONDEMO, INSERTION, INSERTIONDEMO, MERGE, MERGEDEMO };
    std::deque<std::vector<int>> currentArraySequence;
    std::vector<int> lastArray;
    Algorithm currentAlgorithm;

    AlgoTrainer() {}
    void setAlgorithm(Algorithm algorithm);
    void startAlgorithm();
    void bypassDemoAlgorithm();
    void startSelectionSort();
    void startInsertionSort();
    void startMergeSort();
    void arrayChanged(int array[]);
    void arrayChangedSelectedSortDemoMode(int array[]);
    void arrayChangedInsertionSortDemoMode(int array[]);
    void arrayChangedMergeSortDemoMode(int array[]);
    void selectionDemoColorChange(int startIndex, int endIndex);

public slots:
    void userFinishedPrompt();

signals:
    void accept();
    void celebrate();
    void displayArray(int array[]);
    void displayMessage(int x, int y, std::string message);
    void hideMessage();
    void highlightIndex(int elementNumber);
    void updateProgress(int value);
    void resetMerge();
    void resetBoxColors();
    void changeBoxColor(int index, QColor color);

private:
    int initialSequenceSize;
    unsigned demoMessageIndex = 0;
    void startSelectionDemo();
    void startInsertionDemo();
    void startMergeDemo();
    class ExerciseStep;
    bool demoMode = true;
    int prevBeginIndex;
    int prevEndIndex;
    int stepCount = 0;
    std::vector<QPair<int, int>> insertDemoSeq;
    void createInsertSeq();
    void continueDemo(int *array);
};

#endif // ALGOTRAINER_H
