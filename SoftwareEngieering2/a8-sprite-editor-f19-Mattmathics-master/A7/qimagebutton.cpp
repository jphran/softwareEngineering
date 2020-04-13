#include "qimagebutton.h"

QImageButton::QImageButton(QWidget *widget) : QLabel(widget)
{

}

void QImageButton::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}
