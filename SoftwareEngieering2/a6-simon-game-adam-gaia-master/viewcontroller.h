#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QMainWindow>
#include "model.h"
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class viewController; }
QT_END_NAMESPACE

class viewController : public QMainWindow
{
    Q_OBJECT

public:
    viewController(QWidget *parent = nullptr);
    ~viewController();

signals:
    void userMessedUp();
    void userInputSignal();

public slots:
    void turnBlueOn();
    void turnBlueOff();
    void turnRedOn();
    void turnRedOff();
    void updateProgressBar();
    void endGameSlot();
    void waitBeforeRest();

private slots:
    void on_redButton_released();

    void on_blueButton_released();

    void on_startButton_released();

    void displaySequence();

    void playGame();

    void flashButtons();

    void on_redButton_pressed();

    void on_blueButton_pressed();

    void flashTitle();

private:
    Ui::viewController *ui;
    model theModel;
    QTimer* flashButtomSchedulingTimer;
    QTimer* titleFlashTimer;

};
#endif // VIEWCONTROLLER_H
