/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#ifndef ERASER_H
#define ERASER_H

#include "tool.h"

/*
 * A tool to set the selected pixel's rbga value to zero.
 */
class Eraser : public Tool
{
public:
    Eraser(Canvas* canvas);
    ~Eraser() override;

    void perform(int x, int y) override;
};

#endif // ERASER_H
