/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#ifndef BUCKET_H
#define BUCKET_H

#include <QDebug>

#include "tool.h"

/*
 * Tool for replacing all of the same color that are adjacent to each other and bounded by a different color with
 * the currently selected brush color.
 */
class Bucket : public Tool
{
public:
    Bucket(QColor color, Canvas* canvas);
    ~Bucket() override;
    
    /* Uses a helper recursive method to iterate through the adjacent pixels. */
    void perform(int x, int y) override;
    
    bool compareColor(QColor *left, QColor *right);

private:
    void recursivePerform(int x, int y);
    QColor changingColor;
};

#endif // BUCKET_H

