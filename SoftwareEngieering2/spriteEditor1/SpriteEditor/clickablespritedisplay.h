/**
 * Code created with help from QT wiki
 * https://wiki.qt.io/Clickable_QLabel
 */
#ifndef CLICKABLESPRITEDISPLAY_H
#define CLICKABLESPRITEDISPLAY_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableSpriteDisplay : public QLabel {
    Q_OBJECT

public:
    explicit ClickableSpriteDisplay(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickableSpriteDisplay();

signals:
    void clicked(QPointF);
    void mouseMoved(QPointF);
    void unclicked(QPointF);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

};


#endif // CLICKABLESPRITEDISPLAY_H




