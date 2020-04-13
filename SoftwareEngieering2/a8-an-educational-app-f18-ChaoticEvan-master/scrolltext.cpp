/**
  * This class is mimicing a class to scroll the text at the top of the screen.
  * Created by Serena Aeschilman, Spencer Elkington, Andrew Stender, Evan Voordeckers, Ryan Williamson, Keegan Spencer, and Theaux Masqulier.
  */

#include "scrolltext.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QWidget>

/// This class implements the scrolling of text for news feed at top of MainWindow UI.
/// \brief ScrollText::ScrollText
/// \param parent
///
ScrollText::ScrollText(QWidget *parent) : QWidget(parent), scrollPos(0)
{
    // Sets variables for input text.
    staticText.setTextFormat(Qt::PlainText);
    setFixedHeight(fontMetrics().height() + 10);
    // Width of news frame
    leftMargin = 1061;
    seperator = "   ";
    timer.setInterval(30);

    QObject::connect(&timer, &QTimer::timeout, this, &ScrollText::timer_timeout);
}

/// Deconstructor.
/// \brief ScrollText::~ScrollText
///
ScrollText::~ScrollText()
{

}

/// Getter for text param.
/// \brief ScrollText::getText
/// \return
///
QString ScrollText::getText()
{
    return text;
}

/// Setter for text param.
/// \brief ScrollText::setText
/// \param _text
///
void ScrollText::setText(QString _text)
{
    // Sets new text and tells PaintEvent to also update.
    text = _text;
    updateText();
    update();
}

/// Getter for seperator param.
/// \brief ScrollText::getSeperator
/// \return
///
QString ScrollText::getSeperator()
{
    return seperator;
}

/// Setter for seperator param.
/// \brief ScrollText::setSeperator
/// \param _seperator
///
void ScrollText::setSeperator(QString _seperator)
{
    // Sets new amount of seperation between text and tells PaintEvent to also update.
    seperator = _seperator;
    updateText();
    update();
}

/// Updates the scroll area with the new text.
/// \brief ScrollText::updateText
///
void ScrollText::updateText()
{
    // Updates variables for a new QString text so paint can update correctly.
    singleTextWidth = fontMetrics().width(text);
    timer.start();
    scrollPos = 0;
    staticText.setText(text);
    staticText.prepare(QTransform(), font());
    wholeTextSize = QSize(fontMetrics().width(staticText.text()), fontMetrics().height());
}

/// Paint event that handles the scrolling adjustments as it moves across page.
/// \brief ScrollText::paintEvent
///
void ScrollText::paintEvent(QPaintEvent *)
{
    // Sets up paint device.
    QPainter painter(this);
    buffer = QImage(size(), QImage::Format_ARGB32_Premultiplied);

    buffer.fill(qRgba(0, 0 ,0, 0));
    QPainter pb(&buffer);
    pb.setPen(painter.pen());
    pb.setFont(painter.font());

    // x value of beginning of text.
    int x = qMin(-scrollPos, 0)+ leftMargin + (leftMargin/2);

    // Text is still on screen.
    if(x < width())
    {
        pb.drawStaticText(QPointF(x, (height() - wholeTextSize.height()) / 2) + QPoint(0,0), staticText);
        x +=  wholeTextSize.width();

        if(x < 0)
        {
            scrollPos = 0;
        }

        painter.drawImage(0, 0, buffer);
    }
}

/// Called each time the text needs to be repainted.
/// \brief ScrollText::timer_timeout
///
void ScrollText::timer_timeout()
{
    scrollPos = (scrollPos + 5);
    update();
}
