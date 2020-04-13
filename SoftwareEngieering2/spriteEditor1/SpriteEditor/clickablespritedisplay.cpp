/**
 * Code created with help from QT wiki
 * https://wiki.qt.io/Clickable_QLabel
 */
#include "clickablespritedisplay.h"
#include <QMouseEvent>

ClickableSpriteDisplay::ClickableSpriteDisplay(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

ClickableSpriteDisplay::~ClickableSpriteDisplay() {}

void ClickableSpriteDisplay::mousePressEvent(QMouseEvent* event) {
    emit clicked(event->localPos());
}

void ClickableSpriteDisplay::mouseReleaseEvent(QMouseEvent *event)
{
    emit unclicked(event->localPos());
}

void ClickableSpriteDisplay::mouseMoveEvent(QMouseEvent *event)
{
    emit mouseMoved(event->localPos());
}
