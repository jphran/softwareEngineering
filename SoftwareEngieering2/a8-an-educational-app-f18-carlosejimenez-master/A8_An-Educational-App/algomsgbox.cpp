#include "algomsgbox.h"
#include "ui_algomsgbox.h"

AlgoMsgBox::AlgoMsgBox() : QWidget(), ui(new Ui::AlgoMsgBox)
{
    // set up window
    setWindowFlags(Qt::Widget | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_TransparentForMouseEvents, true); // this may need to be removed for "click here to continue"
    setAttribute(Qt::WA_OpaquePaintEvent, true);

    // set up arrow
    arrow = new QWidget(this);
    arrow->setStyleSheet("width: 0; height: 0; border-top: 100px solid #a0dfff; border-right: 100px solid rgba(1,1,1, 0)");
    arrow->setMinimumSize(100,100);

    // set up box
    note = new QPlainTextEdit(this);
    note->setReadOnly(true);
    note->setMinimumSize(450, 200);
    note->showMinimized();
    note->setStyleSheet("background-color: #a0dfff; border-radius: 0px; font: 18px Open Sans Extrabold");
    note->setBackgroundVisible(false);
    note->setAttribute(Qt::WA_TransparentForMouseEvents);
}

AlgoMsgBox::~AlgoMsgBox()
{
    delete arrow;
    delete note;
    delete ui;
}

void AlgoMsgBox::setMessage(QString message)
{
    note->clear();
    note->appendPlainText(message);
    QPoint position(this->x(), this->y());
    setPosition(position); // to account for any resizing due to large amount of text.
}

void AlgoMsgBox::setPosition(QPoint position)
{
    this->move(position.x(), position.y() - note->height());
    arrow->move(0, 200);
}

void AlgoMsgBox::setVisibleMsgBox(bool wantVisible) {
    this->setVisible(wantVisible);
}

void AlgoMsgBox::mousePressEvent(QMouseEvent* mouseEvent)
{
    this->setVisibleMsgBox(false);
    emit messageBoxClosed();
}
