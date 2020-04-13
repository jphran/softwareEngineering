/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#ifndef PAINTALLSAMECOLOR_H
#define PAINTALLSAMECOLOR_H

#include "tool.h"

/*
 * A tool that iterates through all of the colors in the canvas matching the selected one, and replaces them with the current
 * brush color.
 */
class PaintAllSameColor : public Tool
{
public:
    PaintAllSameColor(QColor color, Canvas* Canvas);
    ~PaintAllSameColor() override;

    void perform(int x, int y) override;
};

#endif // PAINTALLSAMECOLOR_H
