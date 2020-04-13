#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <algomsgbox.h>
#include "exerciseview.h"
#include "algotrainer.h"
#include <QMoveEvent>
#include <QCloseEvent>
#include <QFocusEvent>

namespace Ui {
class MainWindow;
}

class BarBox
{
    BarBox(int x, int y, int val) : posX(x), posY(y), numVal(val) {}
public:
    int posX;
    int posY;
    int numVal;
    QColor color;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_skipButton_clicked();
    void on_restartButton_clicked();
    void on_selectionSortButton_clicked();
    void on_insertionSortButton_clicked();
    void on_mergeSortButton_clicked();
    // for when model wants to change progress bar
    void exerciseProgressChange(int progress);
    void hideMessage();
    // Post message to algomsgbox, from AlgoTrainer.
    void postMessage(int x, int y, std::string message);
    void disableButtons();
    void enableButtons();

signals:
    void setTrainer(AlgoTrainer::Algorithm);
    void skipButtonClick();
    void restartButtonClick();
    void startAlgorithm(ExerciseView::Mode mode);
    void moveOverlay(int x, int y);
    void UserProcessedMessage();

private:
    AlgoMsgBox* msgToUser;
    Ui::MainWindow *ui;
    AlgoTrainer trainer;
    AlgoTrainer::Algorithm currentAlgorithm = AlgoTrainer::SELECTION;

    void moveEvent(QMoveEvent* moveEvent);
    void closeEvent(QCloseEvent* moveEvent);
};

#endif // MAINWINDOW_H
