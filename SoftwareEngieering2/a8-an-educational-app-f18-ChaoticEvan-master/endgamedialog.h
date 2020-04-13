#ifndef ENDGAMEDIALOG_H
#define ENDGAMEDIALOG_H

#include <QDialog>

namespace Ui {
class endgamedialog;
}

class endgamedialog : public QDialog
{
    Q_OBJECT

public:
    explicit endgamedialog(QWidget *parent = nullptr);
    ~endgamedialog();

private:
    Ui::endgamedialog *ui;
};

#endif // ENDGAMEDIALOG_H
