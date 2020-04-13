#ifndef MODEL_H
#define MODEL_H


#include <QObject>
#include "triviaBank.h"
#include "imageinfo.h"
#include <SFML/Audio.hpp>
#include <QMessageBox>

class Model: public QObject
{
    Q_OBJECT
    std::vector<std::string> textBank;
    TriviaBank allBank = TriviaBank("all");
    TriviaBank englandBank = TriviaBank("England");
    TriviaBank franceBank = TriviaBank("France");
    TriviaBank greeceBank = TriviaBank("Greece");
    TriviaBank romeBank = TriviaBank("Rome");
    std::vector<imageInfo> englandImages;
    std::vector<imageInfo> franceImages;
    std::vector<imageInfo> romeImages;
    std::vector<imageInfo> greeceImages;
    bool freemode;
    //used to keep track of correct answer for trivia question
    TriviaQuestion currentQuestion;
    std::vector<std::string> infoText;
    unsigned int infoIndex;
    std::string currentLoctation;
    unsigned int questionIndex;
    std::string userName;
    unsigned int userScore;
    sf::Music narrative;
    bool englandDone;
    bool franceDone;
    bool greeceDone;
    bool romeDone;
    bool freeDone;

public:
    Model(QObject *parent = nullptr);

signals:
    void updateInfoText(std::string location);
    void updateTriviaText(TriviaQuestion currentQuestion);
    void checkAnswerSignal(bool correct);
    void doneWithLocation(std::string location);
    void endOfInfoText();
    void startOfInfoText();
    void updateViewScore(int);
    void updateImage(imageInfo newImage,int index);
    void playEnglandAnim();
    void playFranceAnim();
    void playRomeAnim();
    void playGreeceAnim();
    void endGame();

private slots:
    void updateInfoTextSlot(std::string location);
    void getTriviaQuestionSlot(std::string location);
    void checkGuessSlot(std::string guess);
    void nextInfoSlot();
    void previousInfoSlot();
    void removeQuestionSlot();
    void resetNextPrevBtnSlot();
    void getImageSlot(std::string location,int index);
    void addUserName(std::string);
    void startAudioSlot(std::string location);
    void stopAudioSlot();
    void pauseAudioSlot();
    void resumeAudioSlot();
    void playAnimationSlot(std::string location);
    void finishedLocationSlot(std::string location);

};

#endif // MODEL_H
