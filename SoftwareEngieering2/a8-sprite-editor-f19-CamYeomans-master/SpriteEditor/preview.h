#ifndef PREVIEW_H
#define PREVIEW_H

#include <QDialog>
#include "model.h"

namespace Ui {
class Preview;
}

class Preview : public QDialog
{
    Q_OBJECT

public:
    explicit Preview(QWidget *parent = nullptr);
    ~Preview();
    void resizePreviewArea(unsigned x, unsigned y);
    void loopOverFramesForAnimation();
    void setSpriteModel(model*);

private:
    Ui::Preview *ui;
    int animationPlayBackCurrentFrame;
    model* spriteModel;
    QTimer* animationTimer;
};

#endif // PREVIEW_H
