/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#ifndef COLORPICKER_H
#define COLORPICKER_H

#include "tool.h"

/*
 * A tool to replace the primary brush color with that of the selected pixel's color.
 */
class ColorPicker : public Tool
{
public:
    ColorPicker(QColor color, Canvas* canvas);
    ~ColorPicker() override;

    void perform(int x, int y) override;
};

#endif // COLORPICKER_H
