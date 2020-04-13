#ifndef SCROLLTEXT_H
#define SCROLLTEXT_H

#include <QLabel>
#include <QStaticText>
#include <QTimer>
#include <QWidget>

class ScrollText : public QWidget
{
    Q_OBJECT

public:
    explicit ScrollText(QWidget *parent = nullptr);
    virtual ~ScrollText();

public slots:
    QString getText();
    void setText(QString _text);
    QString getSeperator();
    void setSeperator(QString _seperator);


protected:
    void paintEvent(QPaintEvent *);

private:
    void updateText();
    QString text;
    QString seperator;
    QStaticText staticText;
    int singleTextWidth;
    QSize wholeTextSize;
    int leftMargin;
    int scrollPos;
    QImage buffer;
    QTimer timer;
    QLabel* label;

private slots:
    virtual void timer_timeout();
};

#endif // SCROLLTEXT_H
