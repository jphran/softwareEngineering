#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include "model.h"
#include <QColor>
#include <QStackedWidget>
#include <QComboBox>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT
private:
    void createImageBox(imageInfo newImage,int index);

public:
    View(QWidget *parent = nullptr);
    ~View();
    Model* model;


signals:
    void getImageFromLocation(std::string location,int index);
    void updateInfoTextSignal(std::string location);
    void updateTriviaQuestionSignal(std::string location);
    void checkGuessSignal(std::string guess);
    void nextInfoSignal();
    void previousInfoSignal();
    void removeQuestion();
    void resetNextPrevBtnSignal();
    void startAudio(std::string location);
    void stopAudio();
    void pauseAudio();
    void resumeAudio();
    void playAnimation(std::string location);
    void finishedLocation(std::string location);

public slots:
    void openView();
    void updateInfoTextSlot(std::string newText);
    void getResultSlot(bool correct);
    void updateImageSlot(imageInfo newImage,int index);
    void updateScoreSlot(int newScore);
    void endGameSlot();

private slots:
    void updateTriviaTextSlot(TriviaQuestion newQuestion);
    void on_actionExit_triggered();
    void page1();
    void page2();
    void page3();
    void page4();
    void startOfInfoText();
    void endOfInfoText();

    void on_franceButton_clicked();

    void on_englandButton_clicked();

    void on_romeButton_clicked();

    void on_greeceButton_clicked();

    void on_backButton_clicked();

    void on_nextButton_2_clicked();

    void on_nextBtn_clicked();

    void on_previousBtn_clicked();

    void on_goBackBtn_clicked();

    void on_goBackBtn_2_clicked();

    void on_triviaBtn_clicked();

    void doneWithLocationSlot(std::string location);

    void on_actionHelp_triggered();

    void on_actionSources_triggered();

    void on_imageBtn_clicked();

    void on_answerThree_clicked();

    void on_freePlayBtn_clicked();

    void on_PlayNarBtn_clicked();
    void goBackSlot();

private:
    Ui::View *ui;
};
#endif // VIEW_H
