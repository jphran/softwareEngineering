#ifndef QIMAGEBUTTON_H
#define QIMAGEBUTTON_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPainter>

/**
 * QImagaeButton Widget
 *
 * QImageButton supports image background and it's clickable.
 *
 * @author Kyungyoon Kim
 */
class QImageButton : public QLabel
{
Q_OBJECT
public:
        void mousePressEvent(QMouseEvent* event);
        QImageButton(QWidget *widget);

signals :
        void clicked();
};

#endif // QIMAGEBUTTON_H
