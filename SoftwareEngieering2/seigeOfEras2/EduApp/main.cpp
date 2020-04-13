#include "view.h"
#include "startpage.h"
#include "model.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QObject>
#include <QPushButton>
#include <QPixmap>
#include <QFont>
#include <string>
#include <SFML/Audio.hpp>
#include <QIcon>
#include "batteringramrender.h"
#include "seigetowerrender.h"
#include "catapultrender.h"
#include "trojanhorserender.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View view;
    startPage start;
    Model* model;
    model = new Model();
    view.model = model;

    BatteringRamRender* franceRender;
    franceRender = new BatteringRamRender();
    SeigeTowerRender* romeRender;
    romeRender = new SeigeTowerRender();
    CatapultRender* englandRender;
    englandRender = new CatapultRender();
    TrojanHorseRender* greeceRender;
    greeceRender = new TrojanHorseRender();

    a.setWindowIcon(QIcon("../EduApp/trebuchetThumbnail.png"));

    // Splash Screen Setup
    QSplashScreen *splash = new QSplashScreen;
    QFont splashFont;
    splashFont.setItalic(true);
    splash->setFont(splashFont);
    splash->setPixmap(QPixmap("../EduApp/splash_screen_image.jpg"));
    splash->showMessage("Loading Game...", Qt::AlignBottom, Qt::white);
    splash->show();

    //play bacground music
    sf::Music music;
    if (!music.openFromFile("../EduApp/gauntlet_legends_music.wav")) {
        return EXIT_FAILURE;
    }
    music.setVolume(40.f);
    music.play();

    //timer for the splash screen
    QTimer::singleShot(2500, splash, SLOT(close()));
    QTimer::singleShot(2500, &start, SLOT(show()));

    // Connections
    QObject::connect(&start, SIGNAL(openView()), &view, SLOT(openView()));
    QObject::connect(model, SIGNAL(updateInfoText(std::string)),&view, SLOT(updateInfoTextSlot(std::string)));
    QObject::connect(&view, SIGNAL(updateInfoTextSignal(std::string)),model, SLOT(updateInfoTextSlot(std::string)));
    QObject::connect(&view, SIGNAL(updateTriviaQuestionSignal(std::string)),model, SLOT(getTriviaQuestionSlot(std::string)));
    QObject::connect(model, SIGNAL(updateTriviaText(TriviaQuestion)),&view, SLOT(updateTriviaTextSlot(TriviaQuestion)));
    QObject::connect(&view, SIGNAL(checkGuessSignal(std::string)),model, SLOT(checkGuessSlot(std::string)));
    QObject::connect(model, SIGNAL(checkAnswerSignal(bool)),&view, SLOT(getResultSlot(bool)));
    QObject::connect(&view, SIGNAL(nextInfoSignal()),model, SLOT(nextInfoSlot()));
    QObject::connect(&view, SIGNAL(previousInfoSignal()),model, SLOT(previousInfoSlot()));
    QObject::connect(&view, SIGNAL(removeQuestion()),model, SLOT(removeQuestionSlot()));
    QObject::connect(model, SIGNAL(doneWithLocation(std::string)),&view, SLOT(doneWithLocationSlot(std::string)));
    QObject::connect(model, SIGNAL(endOfInfoText()), &view, SLOT(endOfInfoText()));
    QObject::connect(model, SIGNAL(startOfInfoText()), &view, SLOT(startOfInfoText()));
    QObject::connect(&view, SIGNAL(resetNextPrevBtnSignal()),model, SLOT(resetNextPrevBtnSlot()));
    QObject::connect(&view, SIGNAL(getImageFromLocation(std::string,int)),model, SLOT(getImageSlot(std::string,int)));
    QObject::connect(model, SIGNAL(updateImage(imageInfo,int)), &view, SLOT(updateImageSlot(imageInfo,int)));
    QObject::connect(model, SIGNAL(updateViewScore(int)), &view, SLOT(updateScoreSlot(int)));
    QObject::connect(&view, SIGNAL(startAudio(std::string)),model, SLOT(startAudioSlot(std::string)));
    QObject::connect(&view, SIGNAL(stopAudio()),model, SLOT(stopAudioSlot()));
    QObject::connect(&view, SIGNAL(pauseAudio()),model, SLOT(pauseAudioSlot()));
    QObject::connect(&view, SIGNAL(resumeAudio()),model, SLOT(resumeAudioSlot()));
    QObject::connect(&view, SIGNAL(playAnimation(std::string)),model, SLOT(playAnimationSlot(std::string)));
    QObject::connect(model, SIGNAL(playFranceAnim()), franceRender, SLOT(init()));
    QObject::connect(model, SIGNAL(playRomeAnim()), romeRender, SLOT(init()));
    QObject::connect(model, SIGNAL(playEnglandAnim()), englandRender, SLOT(init()));
    QObject::connect(model, SIGNAL(playGreeceAnim()), greeceRender, SLOT(init()));
    QObject::connect(&view, SIGNAL(finishedLocation(std::string)),model, SLOT(finishedLocationSlot(std::string)));
    QObject::connect(model, SIGNAL(endGame()), &view, SLOT(endGameSlot()));
    QObject::connect(franceRender, SIGNAL(goBackSignal()), &view, SLOT(goBackSlot()));
    QObject::connect(romeRender, SIGNAL(goBackSignal()), &view, SLOT(goBackSlot()));
    QObject::connect(englandRender, SIGNAL(goBackSignal()), &view, SLOT(goBackSlot()));
    QObject::connect(greeceRender, SIGNAL(goBackSignal()), &view, SLOT(goBackSlot()));

    return a.exec();
}



