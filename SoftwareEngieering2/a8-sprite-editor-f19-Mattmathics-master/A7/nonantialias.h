/*
* S.I.M.P
* Team 2
* https://github.com/University-of-Utah-CS3505/a8-sprite-editor-f19-Mattmathics
*/
#ifndef NONANTIALIAS_H
#define NONANTIALIAS_H
#include <QWidget>
#include <QPainter>
#include <QStyle>

/*
 * Class for disabling anti-aliasing.
 */
class nonantialias : public QWidget{
    Q_OBJECT
    Q_DISABLE_COPY(nonantialias)
public:
    explicit nonantialias(QWidget* parent = Q_NULLPTR)
        : QWidget(parent)
    {}
    const QPixmap& pixmap() const;
    void setPixmap(const QPixmap& px);
protected:
    void paintEvent(QPaintEvent*);
private:
    QPixmap m_pixmap;
};

#endif // NONANTIALIAS_H
