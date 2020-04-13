#ifndef CREATENEWPAGE_H
#define CREATENEWPAGE_H

#include <QDialog>

namespace Ui {
class CreateNewPage;
}

class CreateNewPage : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNewPage(QWidget *parent = nullptr);
    ~CreateNewPage();

public slots:
    void openCreateNewPage();

signals:
    void createNewAccepted(unsigned x, unsigned y);
    void openStartPage();


private slots:
    void on_buttonBox_accepted();

    void on_xSpinBox_valueChanged(int arg1);

    void on_ySpinBox_valueChanged(int arg1);

    void on_buttonBox_rejected();

private:
    Ui::CreateNewPage *ui;
};

#endif // CREATENEWPAGE_H
