/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#include "nonantialias.h"

    const QPixmap& nonantialias::pixmap() const
    {
        return m_pixmap;
    }
    void nonantialias::setPixmap(const QPixmap& px)
    {
        m_pixmap = px;
        update();
    }

    void nonantialias::paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, false);
        style()->drawItemPixmap(&painter, rect(), Qt::AlignCenter, m_pixmap.scaled(rect().size()));
    }

