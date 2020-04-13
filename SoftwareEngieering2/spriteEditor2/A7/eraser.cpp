/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#include "eraser.h"

Eraser::Eraser(Canvas* canvas): Tool(QColor(0, 0, 0, 0), canvas)
{

}

Eraser::~Eraser()
{

}

void Eraser::perform(int x, int y)
{
    getCurrentFrame()->setPixel(x, y, brushColor);
}
