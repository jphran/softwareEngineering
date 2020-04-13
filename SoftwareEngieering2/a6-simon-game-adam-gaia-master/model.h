#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <QObject>

class model : QObject
{
public:
    model();
    bool checkUserGameMoves(int buttonPressed, unsigned int currentMoveNumber);
    void createSequence();
    void generateNextForSequence();
    std::vector<int> sequence;
    bool getNextToFlash(int& nextButtonToFlash);
    void resetGame();

private:
    unsigned int lastFlashedIndex;

};

#endif // MODEL_H
