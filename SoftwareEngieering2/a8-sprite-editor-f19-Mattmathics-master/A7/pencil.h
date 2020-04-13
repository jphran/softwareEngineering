#ifndef PENCIL_H
#define PENCIL_H

#include "tool.h"

/**
 * Pencil Tool
 *
 * Set color of a one pixer.
 */
class Pencil : public Tool
{
public:
    Pencil(QColor color, Canvas* canvas);
    ~Pencil() override;

    void perform(int x, int y) override;
    void resetStrokes();

private:
    bool **brushStrokes;
};

#endif // PENCIL_H
