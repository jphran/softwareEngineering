#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Box2D/Box2D.h"
#include "econengine.h"
#include "endgamedialog.h"
#include "lemonade.h"
#include "ui_endgamedialog.h"
#include "ui_endgamedialog.h"
#include "endgamedialog.h"
#include "scrolltext.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QTimer>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * A Class that is the main window and basically the view for the application.
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, EconEngine* model = EconEngine::instance());
    ~MainWindow();


signals:
    void sigNewPos(int);
    void sigStartSimulation(Lemonade lemonade);
    void updateWallet(int upgrade);
    void openEndDialog();
    void showCalendar();
    void sigAnimationEnded();

public slots:
    void updateWorld();
    void onSimulationComplete();

    void redirectKhanAcademy();
    void redirectUofU();
    void redirectYoutube();
    void updateSugarBody();
    void updateLemonBody();
    void updateIceBody();

private slots:
    void on_progress_start();
    void imageScroll();
    void createLemonade();
    void updateData();

    void closeGame();
    void closeDialogClosed(int i);

    void lemonSpinBox_valueChanged(int i);
    void iceSpinBox_valueChanged(int i);
    void sugarSpinBox_valueChanged(int i);
    void pitcherSpinBox_valueChanged(int i);

    void on_startButton_clicked();
    void on_yesterdayButton_clicked();
    void on_welcomeCheck4_clicked(bool checked);
    void on_welcomeCheck3_clicked(bool checked);
    void on_welcomeCheck2_clicked(bool checked);
    void on_MuteMusic_clicked();
    void on_beginButton_clicked();

    void on_BuyUmbrella_clicked();
    void on_BuyPitcher_clicked();
    void on_BuyBoomBox_clicked();
    void on_BuySugar_clicked();
    void on_BuyLemons_clicked();
    void on_BuyNeonSIgn_clicked();
    void on_BuyInsurance_clicked();


private:
    bool jump = false;
    Ui::MainWindow *ui;
    b2Body* iceCubeBody;
    b2Body* sugarCubeBody;
    Ui::endgamedialog egd;
    QDialog egPopup;
    b2Body* groundBody;
    b2Body* lemonBody;
    b2Body* pitcherBody;
    b2World world;
    QHBoxLayout* layout;
    QLabel* lemonImage;
    QLabel* pitcherImage;
    QLabel* sugarImage;
    QLabel* iceImage;
    QWidget *lemWin;
    int displayDate;

    GameState& game = *EconEngine::gameState();
    Lemonade lemonade;
    QTimer crowdTimer;
    QTimer whaleTimer;

    QMediaPlayer *noise = new QMediaPlayer();
    bool isMusicPlaying = true;
    bool hasBoughtUmbrella = false;
    bool hasBoughtGrapes = false;
    bool hasBoughtBoomBox = false;
    bool hasBoughtInsurance = false;
    bool hasBoughtSugar = false;
    bool hasBoughtLemon = false;
    bool hasBoughtPitcher = false;
    bool hasBoughtSign = false;
    QVector<QString>* newsStories;
    QHBoxLayout* newsLayout;
    ScrollText* news;

    void playMusic();

    void loadStartImages();
    void loadUpgradeImages();
    void changeNewsText(QString scrollText);
    void animationForDay();
    void animationForWhale();
    void enablePitcherBody(bool);
    void enableLemonBody(bool);
    void enableIceBody(bool);
    void enableSugarBody(bool);

    void collisionCheck();
    void createGroundBody();
    void createLemonBody();
    void createPitcherBody();
    void createIceCubeBody();
    void createSugarCubeBody();
    void calendarWeather(int currWeek);
    void checkAffordablilityOfUpgrades();
    QVector<QString>* getNewsStories(QString filePath);
    void openEndGameDialog(QString message);
    double uiLemonadeCurrCost();
    void updateIngredientsFrameCost();
    void updateCupsofLemonadeMade();
};
#endif // MAINWINDOW_H
