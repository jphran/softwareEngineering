#include "view.h"
#include "ui_view.h"
#include <QPixmap>
#include <SFML/Audio.hpp>

View::View(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);

    //Enabling all of the map buttons
    ui->englandButton->setEnabled(true);
    ui->romeButton->setEnabled(true);
    ui->greeceButton->setEnabled(true);
    ui->franceButton->setEnabled(true);

    //Setting the background
    QPixmap bkgnd("../EduApp/velvet.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    //when the opening screen is displayed, all of the buttons will not be visible
    ui->backButton->setVisible(false);
    ui->regionLabel->setText("Siege of Eras");

    //again, you wont be able to access the previous button if tehre is nothing to go back to, so it is not enabled
    ui->previousBtn->setEnabled(false);
    ui->previousBtn->setVisible(false);
    ui->nextBtn->setEnabled(true);

    //All of the buttons for the locations made with maps as icons
    QPixmap englandPixmap("../EduApp/england.jpg");
    ui->englandButton->setIcon(QIcon(englandPixmap));
    ui->englandButton->setIconSize(QSize(300, 300));

    QPixmap francePixmap("../EduApp/france.jpg");
    ui->franceButton->setIcon(QIcon(francePixmap));
    ui->franceButton->setIconSize(QSize(300, 300));

    QPixmap greecePixmap("../EduApp/greece.jpg");
    ui->greeceButton->setIcon(QIcon(greecePixmap));
    ui->greeceButton->setIconSize(QSize(300, 300));

    QPixmap romePixmap("../EduApp/italy.png");
    ui->romeButton->setIcon(QIcon(romePixmap));
    ui->romeButton->setIconSize(QSize(300, 300));
}


View::~View()
{
    delete ui;
}

void View::page1()
{
    //allow narration button to be accessed
    ui->PlayNarBtn->setEnabled(true);
    int currentIndex = ui->stackedWidget->currentIndex();

    if( currentIndex != ui->stackedWidget->count())
    {
        ui->stackedWidget->setCurrentIndex(0);
    }

    //change background image
    QPixmap bkgnd("../EduApp/velvet.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    //give the button text
    ui->PlayNarBtn->setText("Play Narration");
}

void View::page2()
{
    ui->PlayNarBtn->setEnabled(false);
    int currentIndex = ui->stackedWidget->currentIndex();
    if( currentIndex != ui->stackedWidget->count())
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    ui->stackedWidget->setStyleSheet("");
    ui->PlayNarBtn->setText("Play Narration");
}

void View::page3()
{

    ui->PlayNarBtn->setEnabled(true);
    int currentIndex = ui->stackedWidget->currentIndex();
    if( currentIndex < ui->stackedWidget->count())
    {
        ui->stackedWidget->setCurrentIndex(2);
        ui->stackedWidget->setStyleSheet("");
    }
    ui->PlayNarBtn->setText("Play Narration");
}

void View::page4()
{

    ui->PlayNarBtn->setEnabled(true);
    int currentIndex = ui->stackedWidget->currentIndex();
    if( currentIndex < ui->stackedWidget->count())
    {
        ui->stackedWidget->setCurrentIndex(3);
    }
    ui->PlayNarBtn->setText("Play Narration");
}

/**
 * slot to open this window
 */
void View::openView()
{
    this->showFullScreen();
    emit updateInfoTextSignal("All");
}

void View::updateInfoTextSlot(std::string newText)
{
    ui->infoLabel->setText(QString::fromStdString(newText));
}

/**
 * trigger for selecting new frame in tools menu
 */
void View::on_actionExit_triggered()
{
    this->close();
    ui->backButton->setVisible(true);
}

/**
 * @brief View::on_franceButton_clicked
 * all of these things trigger when the france location is selected
 */
void View::on_franceButton_clicked()
{
    emit stopAudio();
    ui->freePlayBtn->setVisible(false);
    this->page4();
    ui->backButton->setVisible(true);
    startOfInfoText();
    emit updateInfoTextSignal("France");
    emit updateTriviaQuestionSignal("France");
    emit resetNextPrevBtnSignal();
    ui->regionLabel->setText("France");
    //change background image
    QPixmap bkgnd("../EduApp/frenchBackground.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

/**
 * @brief View::on_englandButton_clicked
 * things that are triggered and updated when england is selected
 */
void View::on_englandButton_clicked()
{
    emit stopAudio();
    ui->freePlayBtn->setVisible(false);
    this->page4();
    ui->backButton->setVisible(true);
    startOfInfoText();
    emit updateInfoTextSignal("England");
    emit updateTriviaQuestionSignal("England");
    emit resetNextPrevBtnSignal();
    ui->regionLabel->setText("England");

    //change background image
    QPixmap bkgnd("../EduApp/englishBackground.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

/**
 * @brief View::on_romeButton_clicked
 * things that are triggered and updated when Rome is selected
 */
void View::on_romeButton_clicked()
{
    emit stopAudio();
    ui->freePlayBtn->setVisible(false);
    this->page4();
    ui->backButton->setVisible(true);
    startOfInfoText();
    emit updateInfoTextSignal("Rome");
    emit updateTriviaQuestionSignal("Rome");
    emit resetNextPrevBtnSignal();
    ui->regionLabel->setText("Rome");
    //change background image
    QPixmap bkgnd("../EduApp/romanSiege.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

/**
 * @brief View::on_greeceButton_clicked
 * things that are triggered and updated when Greece is selected
 */
void View::on_greeceButton_clicked()
{
    emit stopAudio();
    ui->freePlayBtn->setVisible(false);
    this->page4();
    ui->backButton->setVisible(true);
    startOfInfoText();
    emit updateInfoTextSignal("Greece");
    emit resetNextPrevBtnSignal();
    emit updateTriviaQuestionSignal("Greece");
    ui->regionLabel->setText("Greece");
    //change background image
    QPixmap bkgnd("../EduApp/trojanHorse.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

void View::on_backButton_clicked()
{
    emit stopAudio();
    if(ui->regionLabel->text() == "Free Play")
    {
        this->page1();
        ui->regionLabel->setText("Siege of Eras");
        if(ui->freePlayBtn->isEnabled() == true){
            ui->freePlayBtn->setVisible(true);
        }
        emit resetNextPrevBtnSignal();
        emit updateInfoTextSignal("All");
        startOfInfoText();
    }
    else{
        this->page4();
        std::string tempString = ui->regionLabel->text().toStdString();
        emit resetNextPrevBtnSignal();
        emit updateInfoTextSignal(tempString);
        startOfInfoText();
    }

}

void View::on_nextButton_2_clicked()
{
    std::string guess = "";
    if (ui->answerOne->isChecked())
    {
        guess = "A";
    }
    else if(ui->answerTwo->isChecked())
    {
        guess = "B";
    }
    else if(ui->answerThree->isChecked())
    {
        guess = "C";
    }
    else
    {
        guess = "D";
    }

    emit checkGuessSignal(guess);
    ui->backButton->setVisible(true);
}

void View::updateTriviaTextSlot(TriviaQuestion newQuestion)
{
    ui->questionLabel->setText(QString::fromStdString(newQuestion.getQuestion()));
    ui->answerOne->setText(QString::fromStdString(newQuestion.getAnswers()[0]));
    ui->answerTwo->setText(QString::fromStdString(newQuestion.getAnswers()[1]));
    ui->answerThree->setText(QString::fromStdString(newQuestion.getAnswers()[2]));
    ui->answerFour->setText(QString::fromStdString(newQuestion.getAnswers()[3]));
}

void View::getResultSlot(bool correct)
{
    if(correct)
    {
        if(ui->regionLabel->text() != "Free Play"){
            this->page3();
            emit playAnimation(ui->regionLabel->text().toStdString());
        }
        emit removeQuestion();
        emit resetNextPrevBtnSignal();
        startOfInfoText();
    }
    else
    {
        sf::Music music;
        music.openFromFile("../EduApp/boo.wav");
        music.play();
        sf::Music music2;
        music2.openFromFile("../EduApp/boo2.wav");
        music2.play();

        QMessageBox msg;
        msg.setText("Wrong!");
        QFont newFont("URW Chancery L", 13, QFont::Bold, true);
        msg.setFont(newFont);
        msg.exec();
    }
}

void View::on_nextBtn_clicked()
{
    ui->previousBtn->setEnabled(true);
    ui->previousBtn->setVisible(true);
    ui->nextBtn->setEnabled(true);
    ui->nextBtn->setVisible(true);
    emit nextInfoSignal();
}

void View::on_previousBtn_clicked()
{
    ui->previousBtn->setEnabled(true);
    ui->previousBtn->setVisible(true);
    ui->nextBtn->setEnabled(true);
    ui->nextBtn->setVisible(true);
    emit previousInfoSignal();
}

void View::on_goBackBtn_clicked()
{
    emit stopAudio();
    this->page2();
}

void View::on_goBackBtn_2_clicked()
{
    emit stopAudio();
    if(ui->freePlayBtn->isEnabled() == true){
        ui->freePlayBtn->setVisible(true);
    }
    this->page1();
    emit updateInfoTextSignal("All");
    emit resetNextPrevBtnSignal();
    ui->regionLabel->setText("Siege of Eras");
    startOfInfoText();
}

void View::on_triviaBtn_clicked()
{
    emit stopAudio();
    this->page2();
    emit resetNextPrevBtnSignal();
    startOfInfoText();
    emit updateInfoTextSignal("All");
}

void View::doneWithLocationSlot(std::string location)
{
    emit stopAudio();
    if(location == "All")
    {
        this->page1();
        ui->freePlayBtn->setEnabled(false);
        ui->freePlayBtn->setVisible(false);
        emit finishedLocation("Free");

    }
    else if(location == "England")
    {
        ui->englandButton->setEnabled(false);
        QPixmap englandPixmap("../EduApp/check.jpg");
        ui->englandButton->setIcon(QIcon(englandPixmap));
        ui->englandButton->setIconSize(QSize(300, 300));
        this->page1();
        emit finishedLocation("England");
    }
    else if(location == "France")
    {
        ui->franceButton->setEnabled(false);
        QPixmap francePixmap("../EduApp/check.jpg");
        ui->franceButton->setIcon(QIcon(francePixmap));
        ui->franceButton->setIconSize(QSize(300, 300));
        emit finishedLocation("France");
        this->page1();
    }
    else if(location == "Greece")
    {
         ui->greeceButton->setEnabled(false);
         QPixmap greecePixmap("../EduApp/check.jpg");
         ui->greeceButton->setIcon(QIcon(greecePixmap));
         ui->greeceButton->setIconSize(QSize(300, 300));
         emit finishedLocation("Greece");
         this->page1();
    }
    else if(location == "Rome")
    {
        ui->romeButton->setEnabled(false);
        QPixmap romePixmap("../EduApp/check.jpg");
        ui->romeButton->setIcon(QIcon(romePixmap));
        ui->romeButton->setIconSize(QSize(300, 300));
        emit finishedLocation("Rome");
        this->page1();
    }
    if(ui->freePlayBtn->isEnabled() == true){
         ui->freePlayBtn->setVisible(true);
    }
    ui->regionLabel->setText("Siege of Eras");
    emit resetNextPrevBtnSignal();
    startOfInfoText();
}

void View::endOfInfoText()
{
    ui->nextBtn->setEnabled(false);
    ui->nextBtn->setVisible(false);
    ui->previousBtn->setEnabled(true);
    ui->previousBtn->setVisible(true);
}

void View::startOfInfoText()
{
    ui->nextBtn->setEnabled(true);
    ui->nextBtn->setVisible(true);
    ui->previousBtn->setEnabled(false);
    ui->previousBtn->setVisible(false);
}

void View::on_actionHelp_triggered()
{   
    QMessageBox msg;
    msg.setText("Welcome to Siege of Eras! In this mini-game, you will be learning about the siege tactics of different major kingdoms throughout history! You will first learn about a location after clicking it, and after reading (or listening) to the facts, you can select the trivia option to enter the game (or select images to looks at some cool pictures relating to the location). The more questions you get right, the higher your score, and the more likely you'll succeed when you siege an enemy castle! Good luck and have fun :)");
    QFont newFont("URW Chancery L", 13, QFont::Bold, true);
    msg.setFont(newFont);
    msg.exec();
}

void View::on_actionSources_triggered()
{
    QMessageBox msg;
    msg.setText("Sources:\n1: http://blog.english-heritage.org.uk/longest-siege-english-history/\n2: https://en.wikipedia.org/wiki/Siege_of_Kenilworth\n3: http://www.medieval-life-and-times.info/medieval-weapons/trebuchet.htm\n4:http://www.medievalchronicles.com/medieval-weapons/castle-siege-weapons/warwolf/\n5:https://www.zainoo.com/en/italy/sicily/syracuse/history\n6:https://en.wikipedia.org/wiki/Siege_of_Syracuse_(213%E2%80%93212_BC)\n7: https://www.ancient.eu/Roman_Siege_Warfare/\n8: https://www.ancient.eu/article/1230/siege-warfare-in-medieval-europe/\n9: http://www.lordsandladies.org/siege-warfare.htm\n10: https://en.wikipedia.org/wiki/Roman_siege_engines");
    QFont newFont("URW Chancery L", 13, QFont::Bold, true);
    msg.setFont(newFont);
    msg.exec();
}

void View::on_imageBtn_clicked()
{
    emit getImageFromLocation(ui->regionLabel->text().toStdString(),0);
}


void View::createImageBox(imageInfo newImage,int index){
    QMessageBox about;
    about.setWindowTitle(QString::fromStdString(ui->regionLabel->text().toStdString()));
    about.setText(QString::fromStdString(newImage.getName()));
    about.setInformativeText(QString::fromStdString(newImage.getDesc()));
    about.setIconPixmap(QPixmap(QString::fromStdString(newImage.getPath())).scaledToHeight(about.height()));
    QAbstractButton *previousBtn = about.addButton(tr("Previous"), QMessageBox::ActionRole);
    QAbstractButton *nextBtn = about.addButton(tr("Next"), QMessageBox::ActionRole);
    about.addButton(QMessageBox::Close);
    about.show();
    about.exec();

    if (about.clickedButton() == previousBtn) {
        emit getImageFromLocation(ui->regionLabel->text().toStdString(),index -1);
    }
    if (about.clickedButton() == nextBtn) {
        emit getImageFromLocation(ui->regionLabel->text().toStdString(),index + 1);
    }
}

void View::updateImageSlot(imageInfo newImage,int index){
    createImageBox(newImage,index);
}

void View::on_answerThree_clicked()
{
    if(ui->answerThree->text().toStdString() == "Grond"){
        QMessageBox about;
        about.setIconPixmap(QPixmap(QString::fromStdString("../EduApp/grond.png")).scaledToHeight(about.height()));
        about.show();
        sf::Music grond;
        grond.openFromFile("../EduApp/GROND.wav");
        grond.play();
        about.exec();

    }
}

void View::on_freePlayBtn_clicked()
{
    emit stopAudio();
    ui->regionLabel->setText("Free Play");
    ui->freePlayBtn->setVisible(false);
    ui->backButton->setVisible(true);
    emit updateTriviaQuestionSignal("All");
    this->page2();
    ui->PlayNarBtn->setEnabled(true);
    emit updateInfoTextSignal("Free");
    emit resetNextPrevBtnSignal();
    startOfInfoText();
}

void View::updateScoreSlot(int newScore){
    ui->scoreLabel->setText("Score: " +  QString::fromStdString(std::to_string(newScore)));
}

void View::on_PlayNarBtn_clicked()
{
    if(ui->PlayNarBtn->text() == "Play Narration"){

        if(ui->regionLabel->text() == ("Siege of Eras"))
        {
            emit startAudio("Home");
        }
        else if(ui->regionLabel->text() == ("Free Play"))
        {
            emit startAudio("Free");
        }
        else if(ui->regionLabel->text() == ("England"))
        {
            emit startAudio("England");
        }
        else if(ui->regionLabel->text() == ("France"))
        {
            emit startAudio("France");
        }
        else if(ui->regionLabel->text() == ("Rome"))
        {
            emit startAudio("Rome");
        }
        else if(ui->regionLabel->text() == ("Greece"))
        {
            emit startAudio("Greece");
        }
        ui->PlayNarBtn->setText("Pause Narration");
    }
    else if(ui->PlayNarBtn->text() == "Pause Narration")
    {
        emit pauseAudio();
        ui->PlayNarBtn->setText("Resume Narration");
    }
    else
    {
        emit resumeAudio();
        ui->PlayNarBtn->setText("Pause Narration");
    }
}


void View::endGameSlot(){
    QMessageBox endGame;
    QAbstractButton *quitBtn = endGame.addButton(tr("Quit"), QMessageBox::ActionRole);
    endGame.setText(QString::fromStdString("You Win!"));
    endGame.show();
    endGame.exec();

    if (endGame.clickedButton() == quitBtn) {
        this->close();
    }
}


void View::goBackSlot(){
    emit stopAudio();
    this->page2();
}
