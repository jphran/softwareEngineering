#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Box2D/Box2D.h"
#include "algomsgbox.h"
#include "algotrainer.h"
#include <QWindow>
#include <QPlainTextEdit>
#include <QTimer>
#include <QGraphicsRectItem>
#include <string>

void MainWindow::moveEvent(QMoveEvent* event)
{
    if (((MainWindow*)this)->msgToUser!= nullptr)
    {
        int deltaX = event->pos().x() - event->oldPos().x();
        int deltaY = event->pos().y() - event->oldPos().y();
        ((MainWindow*)this)->msgToUser->move(msgToUser->pos().x() + deltaX, msgToUser->pos().y() + deltaY);
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (((MainWindow*)this)->msgToUser != nullptr)
    {
        msgToUser->close();
    }

    this->close();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    msgToUser = nullptr;
    ui->studentProgress->setRange(0, 100);
    ui->studentProgress->setValue(0);
    this->setFocusPolicy(Qt::ClickFocus); // not that this can be removed if not using focus of windows for popup control

    // startAlgorithm is called whenever we click a button. It either initializes the scene
    // implementing the bars dropping at the beginning, or it simply gets an array from AlgoTrainer.
    connect(this, &MainWindow::startAlgorithm, ui->exerciseContainer, &ExerciseView::startAlgorithm);

    // MainWindow tells AlgoTrainer which algorithm was selected, so it can initailize its variables.
    connect(this, &MainWindow::setTrainer, &trainer, &AlgoTrainer::setAlgorithm);
    connect(this, &MainWindow::skipButtonClick, &trainer, &AlgoTrainer::bypassDemoAlgorithm);

    // These should be self-explanatory.
    connect(&trainer, &AlgoTrainer::updateProgress, this, &MainWindow::exerciseProgressChange);
    connect(&trainer, &AlgoTrainer::displayMessage, this, &MainWindow::postMessage);
    connect(&trainer, &AlgoTrainer::celebrate, ui->exerciseContainer, &ExerciseView::celebrate);
    connect(&trainer, &AlgoTrainer::resetMerge, ui->exerciseContainer, &ExerciseView::resetMergeVars);
    connect(&trainer, &AlgoTrainer::hideMessage, this, &MainWindow::hideMessage);
    connect(&trainer, &AlgoTrainer::changeBoxColor, ui->exerciseContainer, &ExerciseView::setBoxColor);
    connect(&trainer, &AlgoTrainer::resetBoxColors, ui->exerciseContainer, &ExerciseView::resetBoxColors);

    // displayArray called from AlgoTrainer, tells exerciseContainer to display a new array on the screen.
    connect(&trainer, &AlgoTrainer::displayArray, ui->exerciseContainer, &ExerciseView::setupBoxesPosition);
    connect(&trainer, &AlgoTrainer::accept, ui->exerciseContainer, &ExerciseView::incrementMergeLocation);

    // This is called after the dropping boxes at the beginning completes, so we may get the first array
    // from AlgoTrainer.
    connect(ui->exerciseContainer, &ExerciseView::motionComplete, &trainer, &AlgoTrainer::startAlgorithm);
    connect(ui->exerciseContainer, &ExerciseView::motionComplete, this, &MainWindow::enableButtons);
    connect(ui->exerciseContainer, &ExerciseView::disableButtons, this, &MainWindow::disableButtons);

    // arrayUpdated is called from exerciseContainer to notify AlgoTrainer of the changed array.
    connect(ui->exerciseContainer, &ExerciseView::arrayUpdated, &trainer, &AlgoTrainer::arrayChanged);
    connect(ui->exerciseContainer, &ExerciseView::disableButtons, this, &MainWindow::disableButtons);
    //connect(ui->exerciseContainer, &ExerciseView::arrayUpdated, &trainer, &AlgoTrainer::arrayChangedDemoMode);

    msgToUser = &msgToUser->getMessageBox();

    // These connections are used for informating AlgoTrainer about when a AlgoMsgBox is closed.
    connect(msgToUser, &AlgoMsgBox::messageBoxClosed, this, [&]() {emit this->UserProcessedMessage();});
    connect(this, &MainWindow::UserProcessedMessage, &trainer, &AlgoTrainer::userFinishedPrompt);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_skipButton_clicked()
{
    emit skipButtonClick();
}

void MainWindow::on_restartButton_clicked()
{
    switch (currentAlgorithm)
    {
    case AlgoTrainer::SELECTION:
        on_selectionSortButton_clicked();
        break;
    case AlgoTrainer::INSERTION:
        on_insertionSortButton_clicked();
        break;
    case AlgoTrainer::MERGE:
        on_mergeSortButton_clicked();
        break;
    }
}

void MainWindow::on_selectionSortButton_clicked()
{
    currentAlgorithm = AlgoTrainer::SELECTION;
    // We first tell AlgoTrainer which algorithm to initalize for.
    emit setTrainer(AlgoTrainer::SELECTIONDEMO);
    // We then tell exerciseView to start an algorithm and set its swap policy.
    emit startAlgorithm(ExerciseView::SWAP);
}

void MainWindow::on_insertionSortButton_clicked()
{
    currentAlgorithm = AlgoTrainer::INSERTION;
    // We first tell AlgoTrainer which algorithm to initialize for.
    emit setTrainer(AlgoTrainer::INSERTIONDEMO);

    // We then tell exerciseView to start set its shift policy.
    emit startAlgorithm(ExerciseView::SHIFT);
}

void MainWindow::on_mergeSortButton_clicked()
{
    currentAlgorithm = AlgoTrainer::MERGE;
    ui->exerciseContainer->resetMergeVars();
    // We first tell AlgoTrainer which algorithm to initialize for.
    emit setTrainer(AlgoTrainer::MERGEDEMO);

    // We then tell exerciseView to start set its shift policy.
    emit startAlgorithm(ExerciseView::MERGE);
}

void MainWindow::exerciseProgressChange(int progress)
{
    ui->studentProgress->setValue(progress);
}

void MainWindow::hideMessage()
{
    if (msgToUser != nullptr)
    {
        msgToUser->setVisibleMsgBox(false);
    }
}

void MainWindow::postMessage(int x, int y, std::string message)
{
    if (msgToUser != nullptr)
    {
        msgToUser->setMessage(QString::fromStdString(message));
        msgToUser->setPosition(QPoint(this->x() + x, this->y() + y));
        msgToUser->setVisibleMsgBox(true);
    }
}

void MainWindow::disableButtons()
{
    ui->skipButton->setEnabled(false);
    ui->restartButton->setEnabled(false);
    ui->insertionSortButton->setEnabled(false);
    ui->selectionSortButton->setEnabled(false);
    ui->mergeSortButton->setEnabled(false);
}

void MainWindow::enableButtons()
{
    ui->skipButton->setEnabled(true);
    ui->restartButton->setEnabled(true);
    ui->insertionSortButton->setEnabled(true);
    ui->selectionSortButton->setEnabled(true);
    ui->mergeSortButton->setEnabled(true);
}
