#include "dragdroptest.h"
#include <QtWidgets>

dragdroptest::dragdroptest(QWidget *parent) : QFrame(parent)
{
    setMinimumSize(200, 200);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);

    QLabel *kitten = new QLabel(this);
    QRect rect = new QRect(100, 100, 100, 100);
    // kitten->setPixmap(QPixmap(":/downloads/happyCat.jpg"));
    kitten->setPixmap(QPixmap(rect));
    kitten->move(10, 10);
    kitten->show();
    kitten->setAttribute(Qt::WA_DeleteOnClose);
}

void dragdroptest::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-dnditemdata")){
        if(event->source() == this){
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void dragdroptest::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-dnditemdata")){
        if(event->source() == this){
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void dragdroptest::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-dnditemdata")){
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPixmap pix;
        QPoint offset;
        dataStream >> pix >> offset;

        QLabel *newKitten = new QLabel(this);
        newKitten->setPixmap(pix);
        newKitten->move(event->pos() - offset);
        newKitten->show();
        newKitten->setAttribute(Qt::WA_DeleteOnClose);

        if(event->source() == this){
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }

    } else {
        event->ignore();
    }
}

void dragdroptest::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if(!child){
        return;
    }

    QPixmap pix = *child->pixmap();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pix << QPoint(event->pos() - child->pos());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pix);
    drag->setHotSpot(event->pos() - child->pos());

    QPixmap tempPix = pix;
    QPainter painter;
    painter.begin(&tempPix);
    painter.fillRect(pix.rect(), QColor(127, 127, 127, 127));
    painter.end();

    child->setPixmap(tempPix);

    if(drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction){
        child->close();
    } else {
        child->show();
        child->setPixmap(pix);
    }
}
