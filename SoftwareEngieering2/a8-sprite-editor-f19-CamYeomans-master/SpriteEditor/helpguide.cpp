#include "helpguide.h"
#include "ui_helpguide.h"

HelpGuide::HelpGuide(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpGuide)
{
    ui->setupUi(this);
}

HelpGuide::~HelpGuide()
{
    delete ui;
}
