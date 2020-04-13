#include "model.h"
#include <vector>
#include <iostream>
#include <viewcontroller.h>

#define STARTING_SEQUENCE_SIZE 4


model::model()
{
}


bool model::checkUserGameMoves(int buttonPressed, unsigned int currentMoveNumber)
{
    return buttonPressed == this->sequence[currentMoveNumber];
}


//randomized sequence starting at a set size, increasing as game rpogresses
//0 is red, 1 is blue
void model::createSequence()
{
    for(unsigned int i = 0; i < STARTING_SEQUENCE_SIZE; i++)
    {
        this->sequence.push_back(rand()%2); //gives us a range of [0,1]
    }
    this->lastFlashedIndex = 0;
}


void model::generateNextForSequence()
{
    this->sequence.push_back(rand()%2); //gives range of [0,1]
}


// c# style return true/false and pass value via out param
bool model::getNextToFlash(int& outParamButtonColor)
{
    if(this->lastFlashedIndex < sequence.size())
    {
        outParamButtonColor = this->sequence[this->lastFlashedIndex++];
        return true;
    }
    else
    {
        // Reset index back to start
        this->lastFlashedIndex = 0;
        return false;
    }
}

void model::resetGame()
{
    this->sequence.clear();
}


