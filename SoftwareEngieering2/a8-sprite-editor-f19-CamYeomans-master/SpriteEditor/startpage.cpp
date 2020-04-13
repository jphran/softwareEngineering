#include "startpage.h"
#include "ui_startpage.h"
#include <QFileDialog>

StartPage::StartPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartPage)
{
    ui->setupUi(this);
}

StartPage::~StartPage()
{
    delete ui;
}

void StartPage::on_LearnButton_released()
{
    emit learnClicked();
}

void StartPage::on_CreateNewButton_released()
{
    emit createNewClicked();
    hide();
}

void StartPage::on_OpenFileButton_released()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Sprite"), "/home", tr("Sprite Files (*.ssp)"));

    if(!fileName.isNull())
    {
        this->hide();
        emit loadDudeSignal(fileName.toStdString());
        //TODO: emit signal to draw selected file
    }

}

void StartPage::on_closeButton_released()
{
    emit cancelClicked();
    exit(0);
}
