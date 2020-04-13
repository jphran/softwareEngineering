#ifndef DRAGDROPTEST_H
#define DRAGDROPTEST_H

#include <QFrame>

class DragEnterEvent;
class QDropEvent;

class dragdroptest: public QFrame
{

public:
    explicit dragdroptest(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

};

#endif // DRAGDROPTEST_H
