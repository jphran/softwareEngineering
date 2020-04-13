#ifndef HELPGUIDE_H
#define HELPGUIDE_H

#include <QDialog>

namespace Ui {
class HelpGuide;
}

class HelpGuide : public QDialog
{
    Q_OBJECT

public:
    explicit HelpGuide(QWidget *parent = nullptr);
    ~HelpGuide();

private:
    Ui::HelpGuide *ui;
};

#endif // HELPGUIDE_H
