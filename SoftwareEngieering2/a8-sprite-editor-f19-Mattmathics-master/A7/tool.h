#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QColor>

#include "canvas.h"
#include "frame.h"

/**
 * Tool abstract class
 *
 * @author Kyungyoon Kim
 */
class Tool
{
public:
    virtual ~Tool();

    /* METHOD */
    /**
     * Perfrom on (x, y).
     */
    virtual void perform(int x, int y) = 0;

    /**
     * Perform as line(Linear Equation) from (x1, y1) to (x2, y2).
     */
    void preformLine(int x1, int y1, int x2, int y2);

    /* GETTER, SETTER */
    void setCurrentCanvas(Canvas* canvas);
    void setBrushColor(QColor color);
    QColor getBrushColor();

protected:
    Tool(QColor color, Canvas* canvas);

    /* GETTER, SETTER */
    Frame* getCurrentFrame();

    /* VARS */
    Canvas* currentCanvas;
    QColor brushColor;
};

#endif // TOOL_H
