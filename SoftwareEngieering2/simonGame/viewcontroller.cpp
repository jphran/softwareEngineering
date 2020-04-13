#include "viewcontroller.h"
#include "ui_viewcontroller.h"
#include <QSpinBox>
#include <vector>
#include <iostream>
#include <QTimer>
#include <QObject>
#include <QThread>
#include <string>
#include <QMessageBox>
#include <QDebug>
#include <QDir>

#define RED_PRESS 0
#define BLUE_PRESS 1
#define MIN_TIME_BETWEEN_OUTPUT 250 //ms

//Global vars
int titleColor = 0;
unsigned int currentMoveNumber = 0;
unsigned int timeBetweenOutput = 500; //ms

int redFlashColors[3] = {255, 0, 0};
int redNormalColors[3] = {100, 50, 50};
int blueFlashColors[3] = {0, 0, 255};
int blueNormalColors[3] = {50, 50, 100};

bool keepFlashingTitle = true;

viewController::viewController(QWidget *parent): QMainWindow(parent), ui(new Ui::viewController)
{
    ui->setupUi(this);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(4);
    ui->progressBar->setValue(0);

    QObject::connect(this, SIGNAL(userInputSignal()), this, SLOT(updateProgressBar()));
    QObject::connect(this, SIGNAL(userMessedUp()), this, SLOT(endGameSlot()));

    ui->redButton->setEnabled(false);
    ui->blueButton->setEnabled(false);

    this->turnBlueOff();
    this->turnRedOff();


    titleFlashTimer = new QTimer(this);
    connect(titleFlashTimer, &QTimer::timeout, this, QOverload<>::of(&viewController::flashTitle));
    titleFlashTimer->start(timeBetweenOutput);


    QImage image(":/new/image/popupImage.jpeg");
    ui->popupImage->setPixmap(QPixmap::fromImage(image));
    ui->popupImage->setScaledContents(true);
    ui->popupImage->hide();


}

void viewController::flashTitle()
{
    if(keepFlashingTitle)
    {
        if (titleColor == 0)
        {
            ui->title->setStyleSheet("QLabel {color: #FF0000}");
            ui->roundLabel->setStyleSheet("QLabel {color: #FF0000}");
            ui->RoundNumber->setStyleSheet("QLabel {color: #FF0000}");
            titleColor = 1;
        }
        else
        {
            ui->title->setStyleSheet("QLabel {color: #0000FF}");
            ui->roundLabel->setStyleSheet("QLabel {color: #0000FF}");
            ui->RoundNumber->setStyleSheet("QLabel {color: #0000FF}");
            titleColor = 0;
        }
    }
    else
    {
        keepFlashingTitle = false;
    }
}

viewController::~viewController()
{
    delete ui;
}


// ---------- Slots ----------

void viewController::updateProgressBar()
{
    ui->progressBar->setValue(ui->progressBar->value() + 1);

    if(ui->progressBar->value() == this->theModel.sequence.size())
    {
        this->theModel.generateNextForSequence();
        this->displaySequence();

        QTimer::singleShot(200, this, &viewController::waitBeforeRest);

        currentMoveNumber = 0;

    }
}

void viewController::waitBeforeRest()
{
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(this->theModel.sequence.size());
    ui->progressBar->setValue(0);

    QString newRound = QString::fromStdString(std::to_string(this->theModel.sequence.size() - 4));
    ui->RoundNumber->setText(newRound);
}

void viewController::endGameSlot()
{
    ui->popupImage->show();
    QTimer::singleShot(300, ui->popupImage, &QLabel::hide);

    // display messge
    QMessageBox::critical(this, "Game Over", "You messed up", QMessageBox::Ok);

    // reset game
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(4);
    ui->progressBar->setValue(0);
    this->theModel.resetGame();

    ui->startButton->setEnabled(true);

    currentMoveNumber = 0;
    timeBetweenOutput = 500;

    keepFlashingTitle = true;

}

void viewController::on_redButton_released()
{
    this->turnRedOff();

    if(this->theModel.checkUserGameMoves(RED_PRESS, currentMoveNumber++))
    {
        emit updateProgressBar();
    }
    else{emit userMessedUp();}
}


void viewController::on_blueButton_released()
{
    this->turnBlueOff();

    if(this->theModel.checkUserGameMoves(BLUE_PRESS, currentMoveNumber++))
    {
        emit updateProgressBar();
    }
    else{emit userMessedUp();}
}


void viewController::on_startButton_released()
{
    ui->RoundNumber->setText("0");

    // Reset title
    keepFlashingTitle = false;
    ui->title->setStyleSheet("QLabel {color: #FFFFFF}");
    ui->roundLabel->setStyleSheet("QLabel {color: #FFFFFF}");
    ui->RoundNumber->setStyleSheet("QLabel {color: #FFFFFF}");

    this->playGame();
}


void viewController::turnBlueOn()
{
    QString inputString = "QPushButton {background-color: rgb(";
    inputString += QString::fromStdString(std::to_string(blueFlashColors[0]));
    inputString += ", ";
    inputString += QString::fromStdString(std::to_string(blueFlashColors[1]));
    inputString += ", ";
    inputString += QString::fromStdString(std::to_string(blueFlashColors[2]));
    inputString += ");}";

    this->ui->blueButton->setStyleSheet(inputString);
}

void viewController::turnBlueOff()
{
    QString inputString = "QPushButton {background-color: rgb(";
    inputString += QString::fromStdString(std::to_string(blueNormalColors[0]));
    inputString += ", ";
    inputString += QString::fromStdString(std::to_string(blueNormalColors[1]));
    inputString += ", ";
    inputString += QString::fromStdString(std::to_string(blueNormalColors[2]));
    inputString += ");}";

    this->ui->blueButton->setStyleSheet(inputString);
}

void viewController::turnRedOn()
{
    QString inputString = "QPushButton {background-color: rgb(";
    inputString += QString::fromStdString(std::to_string(redFlashColors[0]));
    inputString += ", ";
    inputString += QString::fromStdString(std::to_string(redFlashColors[1]));
    inputString += ", ";
    inputString += QString::fromStdString(std::to_string(redFlashColors[2]));
    inputString += ");}";

    this->ui->redButton->setStyleSheet(inputString);
}

void viewController::turnRedOff()
{
    QString inputString = "QPushButton {background-color: rgb(";
    inputString += QString::fromStdString(std::to_string(redNormalColors[0]));
    inputString += ", ";
    inputString += QString::fromStdString(std::to_string(redNormalColors[1]));
    inputString += ", ";
    inputString += QString::fromStdString(std::to_string(redNormalColors[2]));
    inputString += ");}";

    this->ui->redButton->setStyleSheet(inputString);
}


void viewController::flashButtons()
{
    int nextButtonToFlash; // declare out parameter for theModel.getNextToFlash()
    if(this->theModel.getNextToFlash(nextButtonToFlash))
    {
        if (nextButtonToFlash == RED_PRESS)
        {
            // Turn button on immediately
            QTimer::singleShot(0, this, &viewController::turnRedOn);

            // call a single shot to turn button off in x seconds
            QTimer::singleShot(timeBetweenOutput, this, &viewController::turnRedOff); // TODO: oh hey we could be clever and set a variable to store the function pointer, set it int this if statement, then call the function with the variable pointer since we are learning about function pointers
        }
        else // BLUE_PRESS
        {
            // Turn button on immediately
            QTimer::singleShot(0, this, &viewController::turnBlueOn);

            // call a single shot to turn button off in x seconds
            QTimer::singleShot(timeBetweenOutput, this, &viewController::turnBlueOff);
        }

    }
    else
    {
        this->flashButtomSchedulingTimer->stop();
        ui->redButton->setEnabled(true);
        ui->blueButton->setEnabled(true);
    }
}


void viewController::displaySequence()
{
    ui->redButton->setEnabled(false);
    ui->blueButton->setEnabled(false);
    //ui->redButton->

    // TODO: create one timer and keep using it instead of creating a timer every time?
    // Qtimers do the same thing every x seconds
    this->flashButtomSchedulingTimer = new QTimer(this);
    connect(this->flashButtomSchedulingTimer, &QTimer::timeout, this, QOverload<>::of(&viewController::flashButtons));
    this->flashButtomSchedulingTimer->start(timeBetweenOutput * 2);

    if(timeBetweenOutput > 250)
    {
            timeBetweenOutput = timeBetweenOutput - 50;
    }
}

void viewController::playGame()
{

    ui->redButton->setEnabled(false);
    ui->blueButton->setEnabled(false);

    ui->startButton->setEnabled(false);
    this->theModel.createSequence();


    this->displaySequence();
}


void viewController::on_redButton_pressed()
{
    this->turnRedOn();
}

void viewController::on_blueButton_pressed()
{
    this->turnBlueOn();
}
