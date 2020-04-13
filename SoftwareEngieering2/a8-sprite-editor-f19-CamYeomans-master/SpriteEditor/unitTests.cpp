#include "model.h"
#include <iostream>

bool testAddFrame()
{
    model* spriteModel;
    spriteModel = new model();

    if(spriteModel->frames.size() != 0)
    {
        return false;
    }

    spriteModel->addFrame();
    if(spriteModel->frames.size() != 1)
    {
        return false;
    }

    spriteModel->addFrame();
    if(spriteModel->frames.size() != 2)
    {
        return false;
    }

    spriteModel->addFrame();
    if(spriteModel->frames.size() != 3)
    {
        return false;
    }

    return true;
}

bool testRemoveFrame()
{
    model* spriteModel;
    spriteModel = new model();

    if(spriteModel->frames.size() != 0)
    {
        return false;
    }
    spriteModel->addFrame();
    spriteModel->addFrame();
    spriteModel->addFrame();
    if(spriteModel->frames.size() != 3)
    {
        return false;
    }

    spriteModel->removeFrame(0);
    if(spriteModel->frames.size() != 2)
    {
        return false;
    }

    spriteModel->removeFrame(0);
    if(spriteModel->frames.size() != 1)
    {
        return false;
    }

    spriteModel->removeFrame(0);
    if(spriteModel->frames.size() != 0)
    {
        return false;
    }

    return true;
}




/**
 * "main" method to run tests
 */
void runTests()
{
    std::cout << "testAddFrame: ";
    testAddFrame() ? std::cout << "pass" : std::cout << "fail";

    std::cout << "testRemoveFrame: ";
    testRemoveFrame() ? std::cout << "pass" : std::cout << "fail";

}





