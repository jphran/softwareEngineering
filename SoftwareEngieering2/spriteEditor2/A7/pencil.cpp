/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#include "pencil.h"

Pencil::Pencil(QColor color, Canvas* canvas) : Tool(color, canvas)
{
    resetStrokes();
}

Pencil::~Pencil()
{
    delete [] brushStrokes;
}

void Pencil::perform(int x, int y)
{
    if(brushStrokes[x][y])
        return;

    brushStrokes[x][y] = true;
    QColor backColor = getCurrentFrame()->getPixel(x,y);
    if(backColor == QColor(0,0,0,0)) {
        getCurrentFrame()->setPixel(x, y, brushColor);
    }
    else {
        int brushAlpha = brushColor.alpha();
        double oneMinus = 1.0-((double)brushAlpha/255.0);
        int outA = (double)brushColor.alpha() + ((double)backColor.alpha()*oneMinus);
        if(outA>255) {
            outA = 255;
        }
        int outR = (brushColor.red()*brushColor.alpha()+backColor.red()*backColor.alpha()*oneMinus)/outA;
        if(outR>255) {
            outR = 255;
        }
        int outG = (brushColor.green()*brushColor.alpha()+backColor.green()*backColor.alpha()*oneMinus)/outA;
        if(outG>255) {
            outG = 255;
        }
        int outB = (brushColor.blue()*brushColor.alpha()+backColor.blue()*backColor.alpha()*oneMinus)/outA;
        if(outB>255) {
            outB = 255;
        }
        getCurrentFrame()->setPixel(x,y, QColor(outR, outG, outB, outA));
    }
}

void Pencil::resetStrokes(){

    brushStrokes = new bool*[getCurrentFrame()->getSizeX()];
    for (int x = 0; x < getCurrentFrame()->getSizeX(); x++) {
        brushStrokes[x] = new bool[getCurrentFrame()->getSizeY()];

        for (int y = 0; y < getCurrentFrame()->getSizeY(); y++) {
            brushStrokes[x][y] = false;
        }
    }
}
