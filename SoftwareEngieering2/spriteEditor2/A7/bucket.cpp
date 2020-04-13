/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#include "bucket.h"
#include <QColor>

Bucket::Bucket(QColor color, Canvas* canvas) : Tool(color, canvas)
{

}

Bucket::~Bucket()
{

}

void Bucket::perform(int x, int y)
{
    changingColor = getCurrentFrame()->getPixel(x,y);

    if(compareColor(&changingColor, &brushColor)) {
        return;
    }

   recursivePerform(x,y);
}

void Bucket::recursivePerform(int x, int y){

    QColor checking;
    getCurrentFrame()->setPixel(x, y, brushColor);

    //recursively color adjacent pixels of the same color to brushColor
    if(x<getCurrentFrame()->getSizeX()-1 && x>-1){
        //check pixel to the right
        checking = getCurrentFrame()->getPixel(x+1, y);
        if(compareColor(&checking, &changingColor)) {
            recursivePerform(x+1, y);
        }
    }

    if(x>0){
        //left
        checking = getCurrentFrame()->getPixel(x-1, y);
        if(compareColor(&checking, &changingColor)) {
            recursivePerform(x-1, y);
        }
    }

    if(y<getCurrentFrame()->getSizeY()-1) {
        //up
        checking = getCurrentFrame()->getPixel(x, y+1);
        if(compareColor(&checking, &changingColor)){
            recursivePerform(x, y+1);
        }
    }

    if(y>0){
        //down
        checking = getCurrentFrame()->getPixel(x, y-1);
        if(compareColor(&checking, &changingColor)) {
            recursivePerform(x, y-1);
        }
    }
}

bool Bucket::compareColor(QColor *left, QColor *right)
{
    return (left->red() == right->red() &&
            left->green() == right->green() &&
            left->blue() == right->blue() &&
            left->alpha() == right->alpha());
}
