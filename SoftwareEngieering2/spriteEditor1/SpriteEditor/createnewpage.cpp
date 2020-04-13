#include "createnewpage.h"
#include "ui_createnewpage.h"
#include <QDebug>

CreateNewPage::CreateNewPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNewPage)
{
    ui->setupUi(this);
}

CreateNewPage::~CreateNewPage()
{
    delete ui;
}

void CreateNewPage::openCreateNewPage()
{
    show();
}

void CreateNewPage::on_buttonBox_accepted()
{
    emit createNewAccepted(ui->xSpinBox->value(), ui->ySpinBox->value());
    hide();
}

void CreateNewPage::on_xSpinBox_valueChanged(int arg1)
{
    ui->ySpinBox->setValue(ui->xSpinBox->value());
}

void CreateNewPage::on_ySpinBox_valueChanged(int arg1)
{
    ui->xSpinBox->setValue(ui->ySpinBox->value());
}

void CreateNewPage::on_buttonBox_rejected()
{
    emit openStartPage();
}
