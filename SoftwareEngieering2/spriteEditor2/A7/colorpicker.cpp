/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#include "colorpicker.h"

ColorPicker::ColorPicker(QColor color, Canvas* canvas) : Tool(color, canvas)
{

}

ColorPicker::~ColorPicker()
{

}

void ColorPicker::perform(int x, int y)
{
    //Prevent pick color which alpha is 0.
    if(getCurrentFrame()->getPixel(x,y).alpha() != 0)
        brushColor = getCurrentFrame()->getPixel(x,y);
}
