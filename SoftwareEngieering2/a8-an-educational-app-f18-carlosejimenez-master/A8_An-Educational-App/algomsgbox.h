#ifndef OVERLAY_H
#include <QPlainTextEdit>
#define OVERLAY_H

namespace Ui {
class AlgoMsgBox;
}

/*
 * AlgoMsgBox is a method of communcation with the user. It has the a dialog box that is displayed to the user.
 * AlgoMsgBox is a singleton at any point you can get and modify the AlgoMsgBox.
 * The basic functionality is to first call getMessageBox() then setMessage and setPosition is called.
 * Must display the AlgoMsgBox by calling setVisibleMsgBox(), this same method can be used to hide the box from the users view.
 *
 * I used the following as reference https://stackoverflow.com/questions/1008019/c-singleton-design-pattern - Austin
*/
class AlgoMsgBox : public QWidget
{
    Q_OBJECT

public:
    ~AlgoMsgBox();
    static AlgoMsgBox& getMessageBox() {
        static AlgoMsgBox instance;
        return instance;
    }

    // Given a QString changes the text of the AlgoMsgBox
    void setMessage(QString message);

    /*
     * Given a QPoint change orientation of the AlgoMsgBox.
     * position is where the AlgoMsgBox points to.
    */
    void setPosition(QPoint position);

    // if isVisble then display the AlgoMsgBox, otherwise hide.
    void setVisibleMsgBox(bool isVisible);

signals:
    void messageBoxClosed();

private:
    AlgoMsgBox();
    AlgoMsgBox(AlgoMsgBox const&) = delete;
    void operator=(AlgoMsgBox const&) = delete;

    Ui::AlgoMsgBox *ui;
    QWidget* arrow;
    QPlainTextEdit* note;
    void mousePressEvent(QMouseEvent* mouseEvent);
};

#endif // OVERLAY_H
