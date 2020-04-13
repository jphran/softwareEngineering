/**
  * This class is mimicing a "view".
  * Created by Serena Aeschilman, Spencer Elkington, Andrew Stender, Evan Voordeckers, Ryan Williamson, Keegan Spencer, and Theaux Masqulier.
  */

#include "educationalprompter.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scrolltext.h"
#include "ui_endgamedialog.h"
#include "educationalprompter.h"
#include <iostream>
#include <QString>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QMediaPlaylist>
#include <QMessageBox>
#include <QSpinBox>
#include <QTimer>
#include <QFile>
#include <QTime>
#include <vector>

#define DEGTORAD 0.0174532925199432957f
#define WIDTH 25
#define HEIGHT 40

/// MainWindow constructor makes connections for slots & signals and does initial setup of UI.
/// \brief MainWindow::MainWindow
/// \param parent
/// \param model
///
MainWindow::MainWindow(QWidget *parent, EconEngine* model)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(b2Vec2 (0.0f, 9.81f))

{
    ui->setupUi(this);
    egd.setupUi(&egPopup);

    // These are UI connections.
    // Connections to outside redirect from menu bar.
    QObject::connect(ui->actionMicroeconomics_Rule, &QAction::triggered, this, &MainWindow::redirectKhanAcademy);
    QObject::connect(ui->actionUofU_Business_Program, &QAction::triggered, this, &MainWindow::redirectUofU);
    QObject::connect(ui->actionVideo_Tutorial, &QAction::triggered, this, &MainWindow::redirectYoutube);

    QObject::connect(ui->welcomeCheck4, &QPushButton::clicked, this, &MainWindow::on_welcomeCheck4_clicked);
    QTimer::singleShot(30,this,&MainWindow::updateWorld);
    QObject::connect(this, &MainWindow::sigStartSimulation, model, &EconEngine::onNewDayLemonade);
    QObject::connect(model, &EconEngine::sigSimulationComplete, this, &MainWindow::onSimulationComplete);
    QObject::connect(this, &MainWindow::showCalendar, this, &MainWindow::updateData);

    // Update the wallet.
    QObject::connect(this, &MainWindow::updateWallet, model, &EconEngine::onUpgradePurchased);
    QObject::connect(this, &MainWindow::showCalendar, this, &MainWindow::on_progress_start);

    // Image connections.
    QObject::connect(&crowdTimer, &QTimer::timeout, this, &MainWindow::imageScroll);
    QObject::connect(&whaleTimer, &QTimer::timeout, this, &MainWindow::animationForWhale);

    // Connects the Create Lemonade button to the main window.
    // Allows us to build a lemonade object from the values within the UI.
    QObject::connect(ui->CreateLemonadeButton,&QPushButton::pressed,this,&MainWindow::createLemonade);

    // declare QLabel's for bodies in world.
    lemonImage = new QLabel();
    sugarImage = new QLabel();
    iceImage = new QLabel();
    // create each body and QLabel, then deactivate bodies and hide QLabels until start button is pushed.
    createGroundBody();
    createPitcherBody();
    enablePitcherBody(false);
    createLemonBody();
    enableLemonBody(false);
    createSugarCubeBody();
    enableSugarBody(false);
    createIceCubeBody();
    enableIceBody(false);

    // Preloads all .png files
    loadStartImages();

    // Time between crowd and whale image being updated
    crowdTimer.setInterval(50);
    whaleTimer.setInterval(50);

    // Set beginning text for the game.
    newsLayout = new QHBoxLayout(ui->newsWidget);
    news = new ScrollText(ui->newsWidget);
    QFont font("manjari", 20);
    news->setFont(font);
    newsLayout->addWidget(news);
    changeNewsText("Welcome to Lemonomics! Beware of whales!");
    // QVector of all news stories
    newsStories = MainWindow::getNewsStories(":/txt/textResources/newsStories.txt");


    // End screen pop up.
    QObject::connect(egd.endGameButton, &QPushButton::pressed, this, &MainWindow::closeGame);
    QObject::connect(&egPopup, &QDialog::finished, this, &MainWindow::closeDialogClosed);

    // Connects the ingredients panel spinboxes to update the cost in the ingredients panel.
    QObject::connect(ui->sugarSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::sugarSpinBox_valueChanged);
    QObject::connect(ui->LemonSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::lemonSpinBox_valueChanged);
    QObject::connect(ui->iceSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::iceSpinBox_valueChanged);
    QObject::connect(ui->pitchersSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::pitcherSpinBox_valueChanged);

    ui->yesterdayButton->setEnabled(false);
    ui->startButton->setEnabled(false);

    updateData();

    // Play begin playing the duck song as soon as the game loads.
    playMusic();

    // Upgrade buttons should be false for the objects the player cannot win.
    ui ->BuySugar->setEnabled(false);
    ui ->BuyLemons-> setEnabled(false);
    ui ->BuyUmbrella ->setEnabled(false);
    ui ->BuyPitcher ->setEnabled(false);
    ui->BuyInsurance ->setEnabled(false);
}

    /// Deconstructor.
    MainWindow::~MainWindow()
    {
        delete ui;
    }


    /// Applies linear impulse to move ice body in a arc from lemonade stand into pitcher.
    /// \brief MainWindow::updateIceBody
    ///
    void MainWindow::updateIceBody()
    {
        iceCubeBody->SetActive(true);
        float impulseIce = iceCubeBody->GetMass() * 10;
        iceCubeBody->ApplyLinearImpulse( b2Vec2(impulseIce,-impulseIce*2), iceCubeBody->GetWorldCenter(),true );
    }

    /// Applies linear impulse to move sugar body in a arc from lemonade stand into pitcher.
    /// \brief MainWindow::updateSugarBody
    ///
    void MainWindow::updateSugarBody()
    {
        // Activate body which enables body to have forces acted upon it
        sugarCubeBody->SetActive(true);
        float impulseSugar = sugarCubeBody->GetMass() * 10;
        sugarCubeBody->ApplyLinearImpulse( b2Vec2(impulseSugar,-impulseSugar*2), sugarCubeBody->GetWorldCenter(),true);

    }
    /// Applies linear impulse to move lemon body in a arc from lemonade stand into pitcher.
    /// \brief MainWindow::updateLemonBody
    ///
    void MainWindow::updateLemonBody()
    {
        // Disable jump so all bodies in the world will only move one time per day.
        jump = false;
        // Activate body which enables body to have forces acted upon it.

        float impulseLemon = lemonBody->GetMass() * 10;
        lemonBody->ApplyLinearImpulse( b2Vec2(impulseLemon,-impulseLemon*2), lemonBody->GetWorldCenter(),true );

    }

    /// Enables or Disables iceBody and the iceImage QLabel attached to the iceCubeBody.
    /// \brief MainWindow::enableIceBody
    /// \param enable
    ///
    void MainWindow::enableIceBody(bool enable)
    {
        if(enable)
        {
            iceCubeBody->SetActive(true);
            iceImage->setHidden(false);
        }
        else
        {
            iceCubeBody->SetActive(false);
            iceImage->setHidden(true);
        }
    }

    /// Enables or Disables lemonBody and the lemonImage QLabel attached to the lemonCubeBody.
    /// \brief MainWindow::enableLemonBody
    /// \param enable
    ///
    void MainWindow::enableLemonBody(bool enable)
    {
        if(enable)
        {
            lemonBody->SetActive(true);
            lemonImage->setHidden(false);
        }
        else
        {
            lemonBody->SetActive(false);
            lemonImage->setHidden(true);
        }
    }

    /// Enables or Disables sugarBody and the sugarImage QLabel attached to the sugarCubeBody.
    /// \brief MainWindow::enableSugarBody
    /// \param enable
    ///
    void MainWindow::enableSugarBody(bool enable)
    {
        if(enable)
        {
            sugarCubeBody->SetActive(true);
            sugarImage->setHidden(false);
        }
        else
        {
            sugarCubeBody->SetActive(false);
            sugarImage->setHidden(true);
        }
    }

    /// Enables or Disables pitcherBody and the pitcherImage QLabel attached to the pitherBody.
    /// \brief MainWindow::enablePitcherBody
    /// \param enable
    ///
    void MainWindow::enablePitcherBody(bool enable)
    {
        if(enable)
        {
            pitcherBody->SetActive(true);
            pitcherImage->setHidden(false);

        }
        else
        {
            pitcherBody->SetActive(false);
            pitcherImage->setHidden(true);
        }
    }


    ///Performs a simulation step for box2d world.
    /// Updating the position & velocity of all bodies in the world.
    /// \brief MainWindow::updateWorld
    ///
    void MainWindow::updateWorld()
    {
        world.Step(1.0f/40.f, 4, 4);

        // Jump is only set to true upon start button being pressed. This way it only moves each body once per day.
        if(jump)
        {
            // Activates lemonBody to allow forces to act upon it.
            lemonBody->SetActive(true);
            // Updates velocity of the lemonBody.
            updateLemonBody();
        }

        // Gets position of each body and updates the position of the bodies QLabel image in order to follow the body as it moves.
        b2Vec2 position = lemonBody->GetPosition();
        lemonImage->setGeometry(position.x, position.y, 0, 0);
        b2Vec2 sugarPos = sugarCubeBody->GetPosition();
        sugarImage->setGeometry(sugarPos.x,sugarPos.y,0,0);
        b2Vec2 icePos = iceCubeBody->GetPosition();
        iceImage->setGeometry(icePos.x,icePos.y,0,0);
        b2Vec2 pitchPos = pitcherBody->GetPosition();
        pitcherImage->setGeometry(pitchPos.x,pitchPos.y,0,0);

        // Check for collision of bodies, only if the body is active and moving.
        if(iceCubeBody->IsActive() || lemonBody->IsActive() || sugarCubeBody->IsActive())
        {
            collisionCheck();
        }

        // Continousally calls updateWorld
        QTimer::singleShot(5,this,&MainWindow::updateWorld);
    }
    /// Creates sugarCubeBody and initializes sugarImage QLabel with a sugar cube png.
    /// \brief MainWindow::createSugarCubeBody
    ///
    void MainWindow::createSugarCubeBody()
    {
        // Defines body type.
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(535.0f, 380.0f);
        sugarCubeBody = world.CreateBody(&bodyDef);

        // Creates QLabel image for this body.
        sugarImage->setFixedSize(55, 55);
        sugarImage->setParent(ui->simulationFrame);
        QPixmap sugarPix(":/img/Images/sugarCube.png");
        int w = sugarImage->width();
        int h = sugarImage->height();
        sugarImage->setPixmap(sugarPix.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        sugarImage->raise();
        sugarImage->setHidden(true);

        // Define box shape/size for dynamic body contact region.
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(20, 20);

        // Define the shape properties.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.1f;
        fixtureDef.restitution = 0.1f;

        // Add the shape to the body.
        sugarCubeBody->CreateFixture(&fixtureDef);
        sugarCubeBody->SetActive(false);
    }

    /// Creates iceCubeBody and initializes iceImage QLabel with a ice cube png.
    /// \brief MainWindow::createIceCubeBody
    ///
    void MainWindow::createIceCubeBody()
    {
        // Defines body type.
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(545.0f, 380.0f);
        iceCubeBody = world.CreateBody(&bodyDef);

        // Creates QLabel image for this body.
        iceImage->setFixedSize(25, 25);
        iceImage->setParent(ui->simulationFrame);
        QPixmap icePix(":/img/Images/iceCube.png");
        int w = iceImage->width();
        int h = iceImage->height();
        iceImage->setPixmap(icePix.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        iceImage->raise();
        iceImage->setHidden(true);

        // Define box shape for dynamic body.
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(10, 10);

        // Define the shape properties.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.1f;
        fixtureDef.restitution = 0.1f;

        // Add the shape to the body.
        iceCubeBody->CreateFixture(&fixtureDef);
        iceCubeBody->SetActive(false);
    }

    /// Creates lemonBody and initializes lemonImage QLabel with a lemon png.
    /// \brief MainWindow::createLemonBody
    ///
    void MainWindow::createLemonBody()
    {
        // Defines body type.
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(545.0f, 380.0f);
        lemonBody = world.CreateBody(&bodyDef);

        // Creates QLabel image for this body.
        lemonImage->setFixedSize(35, 35);
        lemonImage->setParent(ui->simulationFrame);
        QPixmap lemonPix(":/img/Images/lemon.png");
        int w = lemonImage->width();
        int h = lemonImage->height();
        lemonImage->setPixmap(lemonPix.scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        lemonImage->raise();

        // Define box shape for dynamic body.
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(10, 10);

        // Define the shape properties.
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.1f;
        fixtureDef.restitution = 0.1f;

        // Add the shape to the body.
        lemonBody->CreateFixture(&fixtureDef);
        lemonBody->SetActive(false);
    }

    /// Creates groundBody which defines the ground level of the world for bodies.
    /// \brief MainWindow::createGroundBody
    ///
    void MainWindow::createGroundBody()
    {
        // Define the ground body.
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(40.0f, 475.0f);
        groundBodyDef.type = b2_staticBody;
        groundBody = world.CreateBody(&groundBodyDef);

        // Define the ground box shape.
        b2PolygonShape groundBox;
        groundBox.SetAsBox(500.0f, 2.0f);

        // Add the ground fixture to the ground body.
        groundBody->CreateFixture(&groundBox, 1.0f);
    }

    /// Creates pitcherBody and initializes pitcherImage QLabel with a pitcher png.
    /// \brief MainWindow::createPitcherBody
    ///
    void MainWindow::createPitcherBody()
    {
        // Defines body type.
        b2BodyDef statTestBodyDef;
        statTestBodyDef.type = b2_staticBody;
        statTestBodyDef.position.Set(575.0f, 415.0f);
        pitcherBody = world.CreateBody(&statTestBodyDef);

        // Creates QLabel image for this body.
        pitcherImage = new QLabel();
        pitcherImage->setFixedSize(125, 105);
        pitcherImage->setParent(ui->simulationFrame);
        QPixmap groundPix(":/img/Images/ClipArtPitcher.png");
        int gw = pitcherImage->width();
        int gh = pitcherImage->height();
        pitcherImage->setPixmap(groundPix.scaled(gw+20, gh, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        pitcherImage->raise();

        // Define the pitcher box shape.
        b2PolygonShape testBox;
        testBox.SetAsBox(WIDTH/2, HEIGHT/10);

        pitcherBody->CreateFixture(&testBox,1.0f);
        pitcherBody->SetActive(false);
    }

    /// Checks for the collision of a body with another body, using the edges of bodies to detect collision.
    /// This method checks all 3 bodies lemonBody,sugarCubeBody and iceCubeBody to collide with pitcherBody.
    /// Upon collision with another body, reset each body back to initial position and updates linear velocity back to 0.
    /// Updates each body one after another starting with lemonBody->iceCubeBody->sugarCubeBody.
    /// \brief MainWindow::collisionCheck
    ///
    void MainWindow::collisionCheck()
    {
        // Gets edge for body and iterates through each edge.
        for (b2ContactEdge* edge = lemonBody->GetContactList(); edge; edge = edge->next)
        {
            // Check if body is in contact with another body.
            if(edge->contact->IsTouching())
            {
                lemonImage->setHidden(true);
                lemonBody->SetTransform(b2Vec2(535.0f, 380.0f),0);
                lemonBody->SetLinearVelocity(b2Vec2(0,0));
                lemonBody->SetActive(false);
                iceImage->setHidden(false);
                updateIceBody();
            }
        }

        for (b2ContactEdge* edge2 = iceCubeBody->GetContactList(); edge2; edge2 = edge2->next)
        {
            //check if body is in contact with another body
            if(edge2->contact->IsTouching())
            {
                iceImage->setHidden(true);
                iceCubeBody->SetTransform(b2Vec2(535.0f, 380.0f),0);
                iceCubeBody->SetLinearVelocity(b2Vec2(0,0));
                iceCubeBody->SetActive(false);
                sugarImage->setHidden(false);
                updateSugarBody();
            }
        }

        for (b2ContactEdge* edge3 = sugarCubeBody->GetContactList(); edge3; edge3 = edge3->next)
        {
            //check if body is in contact with another body
            if(edge3->contact->IsTouching())
            {
                sugarImage->setHidden(true);
                sugarCubeBody->SetTransform(b2Vec2(535.0f, 380.0f),0);
                sugarCubeBody->SetLinearVelocity(b2Vec2(0,0));
                sugarCubeBody->SetActive(false);
            }
        }
    }

/// Slot used to build a lemonade object based on the values within the UI,
/// and then pass by reference to the data member lemonade.
/// \brief MainWindow::createLemonade
///
void MainWindow::createLemonade()
{
    if(game.stand.wallet - uiLemonadeCurrCost() < 0)
    {
        QMessageBox poorIngMsg;
        poorIngMsg.setText("You can't afford to make this recipe!");
        poorIngMsg.exec();
        return;
    }

    if(ui->pitchersSpinBox->value() == 0)
    {
        QMessageBox addIngMsg;
        addIngMsg.setText("You will probably need at least 1 pitcher of lemonade!");
        addIngMsg.exec();
        return;
    }

    if((ui->LemonSpinBox->value() == 0) && (ui->sugarSpinBox->value() == 0) && (ui->iceSpinBox->value() == 0))
    {
        QMessageBox addIngMsg;
        addIngMsg.setText("This isn't a water stand!\nMake some lemonade!");
        addIngMsg.exec();
        return;
    }

    lemonade.setRecipe(ui->LemonSpinBox->value(),
                       ui->sugarSpinBox->value(),
                       ui->iceSpinBox->value(),
                       ui->priceSpinBox->value(),
                       ui->pitchersSpinBox->value());

    ui->startButton->setEnabled(true);
}

/// Uses the lemonade data from yesterday if the user wishes not to change their recipe or price.
/// Sets the values of the spinboxes on the UI to the lemonade data.
/// \brief MainWindow::on_yesterdayButton_clicked
///
void MainWindow::on_yesterdayButton_clicked()
{
    if(ui->pitchersSpinBox->value() == 0)
    {
        QMessageBox addIngMsg;
        addIngMsg.setText("You will probably need at least 1 pitcher of lemonade!");
        addIngMsg.exec();
        return;
    }

    if((ui->LemonSpinBox->value() == 0) && (ui->sugarSpinBox->value() == 0) && (ui->iceSpinBox->value() == 0))
    {
        QMessageBox addIngMsg;
        addIngMsg.setText("This isn't a water stand!\nMake some lemonade!");
        addIngMsg.exec();
        return;
    }

    ui->LemonSpinBox->setValue(lemonade.getLemon());
    ui->sugarSpinBox->setValue(lemonade.getSugar());
    ui->iceSpinBox->setValue(lemonade.getIce());
    ui->priceSpinBox->setValue(lemonade.getPricePerCup());

    ui->pitchersSpinBox->setValue(lemonade.getNumPitchers());
    updateIngredientsFrameCost();

    ui->startButton->setEnabled(true);
}

/// Updates the game data for the player
/// \brief MainWindow::updateData
///
void MainWindow::updateData()
{
    ui->ingDayLabel->setVisible(true);
    // Increments current date to add weekends
    if (game.currentDate < 5 )
    {
        displayDate = game.currentDate + 2;
    } else if (game.currentDate < 10)
    {
        displayDate = game.currentDate + 4;
    } else if (game.currentDate < 15)
    {
        displayDate = game.currentDate + 6;
    }
    ui->ingDayLabel->setText("Day: " + QString::number(game.currentDate));
    ui->walletLabel->setText("Wallet: $ " + QString::number(game.stand.wallet, 'f', 2));

    ui->ingDayLabel->setText("Day: " + QString::number(displayDate));
    ui->profitLabel->setText("Profit: $" + QString::number(game.yesterday().profit));
    ui->salesLabel->setText("Sales: $"   + QString::number(game.yesterday().sales));
    ui->costLabel->setText("Cost: $"     + QString::number(game.yesterday().cost));
    ui->demandLabel->setText("Demand: "  + QString::number(game.yesterday().demanded));
    ui->incomeDisplay->setText(QString::number(game.yesterday().income));
    ui->costDisplay->setText(QString::number(game.yesterday().cost));
    ui->profitDisplay->setText(QString::number(game.yesterday().profit));

    ui->demandDisplay->setText(QString::number(game.yesterday().demanded));
    ui->salesDisplay->setText(QString::number(game.yesterday().sales));

    ui->soldOutLabel->setVisible(game.yesterday().soldOut);
    checkAffordablilityOfUpgrades();
}

/// A method that checks the affordability of upgrades to ensure
/// that buttons are only enabled if they can afford them, disabled otherwise.
/// It also checks to ensure that the player only buys one of each upgrade.
/// \brief MainWindow::checkAffordablilityOfUpgrades
///
void MainWindow::checkAffordablilityOfUpgrades()
{
    int wallet = game.stand.wallet;
    if (wallet > 75 && hasBoughtBoomBox == false)
    {
        ui->BuyBoomBox -> setEnabled(true);
    }
    if (wallet > 50 && hasBoughtSign == false)
    {
        ui ->BuyNeonSIgn ->setEnabled(true);
    }
    if (wallet > 150 && hasBoughtLemon == false)
    {
        ui->BuyLemons->setEnabled(true);
    }
    if (wallet > 150 && hasBoughtSugar == false)
    {
        ui->BuySugar->setEnabled(true);
    }
    if (wallet > 250 && hasBoughtInsurance ==false)
    {
        ui->BuyInsurance->setEnabled(true);
    }
    if (wallet > 250 && hasBoughtPitcher == false)
    {
        ui->BuyPitcher-> setEnabled(true);
    }
    if (wallet > 2000&& hasBoughtUmbrella == false)
    {
        ui->BuyUmbrella->setEnabled(true);
    }
    if (wallet < 2000)
    {
        ui->BuyUmbrella->setEnabled(false);
    }
    if (wallet < 150)
    {
        ui->BuySugar->setEnabled(false);
        ui->BuyLemons->setEnabled(false);        
    }

    if (wallet < 250)
    {
        ui->BuyInsurance->setEnabled(false);
        ui->BuyPitcher-> setEnabled(false);
    }
    if (wallet < 75)
    {
        ui->BuyBoomBox -> setEnabled(false);
    }
    if (wallet < 50)
    {
        ui ->BuyNeonSIgn ->setEnabled(false);
    }
}

/// Gives the opportunity for a user to click redirected link to learn more about microeconomics.
/// \brief MainWindow::redirectKhanAcademy A method that pops open a hyperlink to khanacademy to learn more about microeconomics.
///
void MainWindow::redirectKhanAcademy()
{
    QMessageBox msgBox;
    msgBox.setText("<a href='https://www.khanacademy.org/economics-finance-domain/microeconomics'>Khan Academy</a>");
    msgBox.exec();
}

/// Gives the opportunity for users to learn more about Business at the University of Utah.
/// \brief MainWindow::redirectKhanAcademy
///
void MainWindow::redirectUofU()
{
    QMessageBox msgBox;
    msgBox.setText("<a href='https://eccles.utah.edu/programs/online-courses/'>UofU Business Courses</a>");
    msgBox.exec();
}

/// Redirect to Youtube Tutorial as to how to play the game.
/// \brief MainWindow::redirectKhanAcademy
///
void MainWindow::redirectYoutube()
{
    QMessageBox msgBox;
    msgBox.setText("<a href='https://youtu.be/wMYyAY4ZB00'>Tutorial</a>");
    msgBox.exec();
}

/// A method to play music.
/// \brief MainWindow::playMusic
///

void MainWindow::playMusic()
{
    // Create music playlist to repeat the song.
    QMediaPlaylist* playlist= new QMediaPlaylist;
    playlist->addMedia(QUrl("qrc:/music/ducksong.mp3"));
    playlist->addMedia(QUrl("qrc:/music/ducksong.mp3"));
    playlist->addMedia(QUrl("qrc:/music/ducksong.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    noise ->setMedia(playlist);
    noise ->play();
}

/// Updates game upon completion of a day/simulation.
/// \brief MainWindow::onSimulationComplete
///
void MainWindow::onSimulationComplete()
{
    //this->updateData();
    this->animationForDay();
    ui->walletLabel->setText("Wallet: $ " + QString::number(game.stand.wallet, 'f', 2));
}

/// Creates the proper background and crowd size for the daily animation
/// Starts Crowdtimer
/// \brief MainWindow::animationForDay
///
void MainWindow::animationForDay()
{
    //Sets all the calendar information to not visible
    ui->calendarLabel->setVisible(false);
    ui->demandLabel->setVisible(false);
    ui->profitLabel->setVisible(false);
    ui->salesLabel->setVisible(false);
    ui->costLabel->setVisible(false);
    ui->incomeLabel->setVisible(false);

    ui->demandDisplay->setVisible(false);
    ui->profitDisplay->setVisible(false);
    ui->salesDisplay->setVisible(false);
    ui->costDisplay->setVisible(false);
    ui->incomeDisplay->setVisible(false);
    ui->soldOutLabel->setVisible(false);

    ui->simulationFrame->setVisible(true);
    ui->day1Label->setVisible(false);
    ui->day2Label->setVisible(false);
    ui->day3Label->setVisible(false);
    ui->day4Label->setVisible(false);
    ui->day5Label->setVisible(false);

    //Shows the simulation frame
    ui->simulationFrame->setVisible(true);
    QRect backgroundDimensions(350, 100, ui->welcomeBackground->width(), ui->welcomeBackground->height());
    QPixmap background;
    //Sets the background QPixmap to the correct weather image
    // We have to create a temp pixmap and set it to our default image
    // because there is no obvious way to set a pixmap to a image
    if(game.yesterday().disaster == game.world.TORNADO)
    {
        QPixmap backgroundTemp(":/img/Images/Background Tornado.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == game.world.RAINY)
    {
        // Rainy weather.
        QPixmap backgroundTemp(":/img/Images/Background Rain.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == game.world.SNOWY)
    {
        // Snowy weather.
        QPixmap backgroundTemp(":/img/Images/Background Snow.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == game.world.CLOUDY)
    {
        // Cloudy weather.
        QPixmap backgroundTemp(":/img/Images/Background Overcast.png");
        background = backgroundTemp;
    } else if (game.yesterday().weatherState == game.world.SUNNY)
    {
        // Sunny weather.
        QPixmap backgroundTemp(":/img/Images/Background Default.png");
        background = backgroundTemp;
    }

    ui->simulationPicture->setPixmap(background.copy(backgroundDimensions));
    QRect dimensions(0, 0, ui->crowdLabel->width(), ui->crowdLabel->height());
    QPixmap defaultImage;
    // Set the crowd image to the correct size of crowd
    // We have to create a temp pixmap and set it to our default image
    // because there is no obvious way to set a pixmap to a image
    if(game.yesterday().demanded < 44)
    {
        //Light crowd
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Light.png");
        defaultImage = temp;
    }
    else if(game.yesterday().demanded < 74)
    {
        //Medium crowd
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Medium.png");
        defaultImage = temp;
    }
    else
    {
        //Heavy crowd
        QPixmap temp(":/img/Images/Crowd_Levels/Crowd Heavy.png");
        defaultImage = temp;
    }
    ui->crowdLabel->setPixmap(defaultImage.copy(dimensions));
    ui->simulationPicture->setVisible(true);

    // Whale drops on day 14, else crowd moves across screen
    if (game.world.WHALE == game.yesterday().disaster)
    {
        whaleTimer.start();
    } else
    {
        // Crowd begins moving across screen.
        crowdTimer.start();
    }
}

/// Displays the correct calendar and weather information
/// Creates the pop up windows on Day 1 and 5
/// \brief MainWindow::on_progress_start
///
void MainWindow::on_progress_start()
{
     enablePitcherBody(false);
  
    // Shows the calendar days.
    ui->day1Label->setVisible(true);
    ui->day2Label->setVisible(true);
    ui->day3Label->setVisible(true);
    ui->day4Label->setVisible(true);
    ui->day5Label->setVisible(true);
    QPixmap calendar;
    int currWeek = -99;
    // Displays the correct calendar dates based on the current date of the game.
    if (game.currentDate <= 4)
    {
        // Week 1 Days 1-7.
        QPixmap calendarImage(":/img/Images/Calendars/lemonomicsCalendarWeek1Short.png");
        ui->calendarLabel->setPixmap(calendarImage);
        currWeek = 0;
    }
    else if (game.currentDate > 4 && game.currentDate <= 9)
    {
        // Week 2 Days 8-14.
        QPixmap calendarImage(":/img/Images/Calendars/lemonomicsCalendarWeek2Short.png");
        ui->calendarLabel->setPixmap(calendarImage);
        currWeek = 1;
    }
    else
    {
        // Week 3 Days 15-21.
        QPixmap calendarImage(":/img/Images/Calendars/lemonomicsCalendarWeek3Short.png");
        ui->calendarLabel->setPixmap(calendarImage);
        currWeek = 2;
    }

    calendarWeather(currWeek);
    ui->simulationPicture->setVisible(false);
    ui->calendarLabel->setVisible(true);

    // Displays the informational pop up windows on Day 1 and 5.
    if (game.currentDate == 1)
    {
        EPrompt::displayEduPrompt(EPrompt::P_REVENUE_COST_PROFIT);
    }
    else if (game.currentDate == 5)
    {
        EPrompt::displayEduPrompt(EPrompt::P_PRICE_EFFECT);
    }


}

/// Sets the day of each calendar to the correct weather forecast.
/// \brief MainWindow::calendarWeather
/// \param currWeek
///
void MainWindow::calendarWeather(int currWeek)
{
    if(currWeek == -99)
    {
        return;
    }
    // Load initial images into QPixmaps.
    QPixmap sunnyDay(":/img/Images/Weather_Images/Sunny.png");
    QPixmap rainyDay(":/img/Images/Weather_Images/Rainy.png");
    QPixmap cloudyDay(":/img/Images/Weather_Images/Cloudy.png");
    QPixmap tornadoDay(":/img/Images/Weather_Images/Tornado.png");
    QPixmap snowyDay(":/img/Images/Weather_Images/Snowy.png");

    std::vector<QPixmap> currWeekWeather;
    for (unsigned int i = 0 ; i < 5 ; i++)
    {
        if (game.days[i+currWeek*5].disaster == 1)
        {
            //Tornado weather.
            currWeekWeather.push_back(tornadoDay);
        } else if (game.days[i+currWeek*5].weatherState == 0)
        {
            // Rainy weather.
            currWeekWeather.push_back(rainyDay);
        } else if (game.days[i+currWeek*5].weatherState == 1)
        {
            // Snowy weather.
            currWeekWeather.push_back(snowyDay);
        } else if (game.days[i+currWeek*5].weatherState == 2)
        {
            // Cloudy weather.
            currWeekWeather.push_back(cloudyDay);
        } else if (game.days[i+currWeek*5].weatherState == 3)
        {
            // Sunny weather.
            currWeekWeather.push_back(sunnyDay);
        }
    }
    // Set each day label to correct item in the vector.
    ui->day1Label->setPixmap(currWeekWeather[0].scaled(150, 235, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->day2Label->setPixmap(currWeekWeather[1].scaled(150, 235, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->day3Label->setPixmap(currWeekWeather[2].scaled(150, 235, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->day4Label->setPixmap(currWeekWeather[3].scaled(150, 235, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->day5Label->setPixmap(currWeekWeather[4].scaled(150, 235, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

/// Enables and sets default starting images for game.
/// \brief MainWindow::loadStartImages
///
void MainWindow::loadStartImages()
{
    // QLabel rectangle dimensions, and start x/y coordinate for 1920x1080p images.
    QRect dimensions(350, 100, ui->welcomeBackground->width(), ui->welcomeBackground->height());

    // Creates background color and fills with light blue.
    QPixmap startBackground(ui->welcomeBackground->width(), ui->welcomeBackground->height());
            startBackground.fill(QColor(47, 191, 235));
    QPixmap defaultImage(":/img/Images/Background Default.png");
    QPixmap startLogo(":/img/Images/logo.png");

    // Sets each image to corresponding label.
    ui->welcomeBackground->setPixmap(startBackground);
    ui->welcomeLogo->setPixmap(startLogo);
    ui->simulationPicture->setPixmap(defaultImage.copy(dimensions));

    loadUpgradeImages();
    ui->ingDayLabel->setVisible(false);
}

/// Loads the images for upgrades panel for user to buy.
/// \brief MainWindow::loadUpgradeImages Helper method for loading upgrade images.
///
void MainWindow::loadUpgradeImages()
{
    QPixmap boomBox(":/img/Images/Upgrades/Boom Box.png");
    QPixmap neonSign(":/img/Images/Upgrades/Neon Sign.png");
    QPixmap insuranceSign(":/img/Images/Upgrades/Insurance Sign.png");
    QPixmap orgLemons(":/img/Images/Upgrades/organic lemons.png");
    QPixmap sugarDealer(":/img/Images/Upgrades/Sugar Dealer.png");
    QPixmap umbrella(":/img/Images/Upgrades/Umbrella.png");
    QPixmap pitcher(":/img/Images/ClipArtPitcher.png");

    ui->boomBoxImage->setPixmap(boomBox.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->bigPitcherImage->setPixmap(pitcher.scaled(540, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->neonSignImage->setPixmap(neonSign.scaled(540, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->insuranceImage->setPixmap(insuranceSign.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->lemonsImage->setPixmap(orgLemons.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->sugarImage->setPixmap(sugarDealer.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->umbrellaImage->setPixmap(umbrella.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

///
/// Below are methods that occur due to a button being clicked in the ui.
///

/// Enables and Disables all necessary buttons upon starting the simulation.
/// Sets up game and emits simulation to start.
/// \brief MainWindow::on_startButton_clicked
///
void MainWindow::on_startButton_clicked()
{

    ui->welcomeFrame->setVisible(false);
    ui->dayFrame->setVisible(false);
    ui->progressFrame->setVisible(true);
    ui->progressFrame->raise();
    enablePitcherBody(true);
    enableLemonBody(true);
    jump=true;


    if(game.currentDate != 0)
        if(lemonade.getLemon() == 0 && lemonade.getIce() == 0 && lemonade.getSugar() == 0)
        {
            QMessageBox makeLemMessage;
            makeLemMessage.setWindowTitle("Make some lemonade!");
            makeLemMessage.setText("Make some lemonade first!\nLemonade must have at least one ingredient!");
            makeLemMessage.exec();
            return;
        }

    // Sets new story semi-randomly via current time in miliseconds.
    QTime now = QTime::currentTime();
    int storyIndex = now.msec() % newsStories->size();
    QString story = newsStories->at(storyIndex);
    changeNewsText(story);

    ui->startButton->setEnabled(false);
    ui->CreateLemonadeButton->setEnabled(false);
    ui->yesterdayButton->setEnabled(false);

    emit sigStartSimulation(this->lemonade);
}

/// Makes the welcome check disappear when checked.
/// \brief MainWindow::on_welcomeCheck4_clicked
/// \param checked
///
void MainWindow::on_welcomeCheck4_clicked(bool checked)
{
    if (checked)
    {
        ui->welcomeCheck4->setVisible(false);
    }
}

/// Makes the welcome check disappear when checked.
/// \brief MainWindow::on_welcomeCheck3_clicked
/// \param checked
///
void MainWindow::on_welcomeCheck3_clicked(bool checked)
{
    if (checked)
    {
        ui->welcomeCheck3->setVisible(false);
    }
}

/// Makes the welcome check disappear when checked.
/// \brief MainWindow::on_welcomeCheck2_clicked
/// \param checked
///
void MainWindow::on_welcomeCheck2_clicked(bool checked)
{
    if (checked)
    {
        ui->welcomeCheck2->setVisible(false);
    }
}

/// Check if the music is playing, if so, mute it. Otherwise, play it again.
/// \brief MainWindow::on_MuteMusic_clicked
///
void MainWindow::on_MuteMusic_clicked()
{
    if (isMusicPlaying)
    {
        ui ->MuteMusic->setText("Unmute Music");
        isMusicPlaying = false;
        noise-> stop();
    }
    else
    {
        ui ->MuteMusic->setText("Mute Music");
        isMusicPlaying = true;
        noise -> play();
    }
}

/// Sets newsScroll text.
/// \brief MainWindow::changeNewsText
/// \param scrollText
///
void MainWindow::changeNewsText(QString scrollText)
{
    // ScrollText method to change text being painted
    news->setText(scrollText);
}


///
/// BELOW are methods that are button clicks of upgrades.
///

/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyUmbrella_clicked When buy umbrella button is clicked.
///
void MainWindow::on_BuyUmbrella_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->umbrellaImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    emit updateWallet(1);
    ui ->BuyUmbrella ->setEnabled(false);
    hasBoughtUmbrella = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet, 'f', 2));
}

/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyPitcher_clicked When buy pitcher button is clicked.
///
void MainWindow::on_BuyPitcher_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->bigPitcherImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(7);
    ui ->BuyPitcher ->setEnabled(false);
    hasBoughtPitcher = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet, 'f', 2));
}

/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyBoomBox_clicked When buy boom box button is clicked.
///
void MainWindow::on_BuyBoomBox_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->boomBoxImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(4);
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet, 'f', 2));

    // Start RickRolling.
    QMediaPlaylist* playlist= new QMediaPlaylist;
    playlist -> addMedia(QUrl("qrc:/music/rickroll.mp3"));
    playlist -> addMedia(QUrl("qrc:/music/rickroll.mp3"));
    playlist -> addMedia(QUrl("qrc:/music/rickroll.mp3"));

    noise -> stop();
    noise ->setMedia(playlist);
    noise ->play();
    hasBoughtBoomBox = true;
    ui->BuyBoomBox->setEnabled(false);
}

/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuySugar_clicked When buy sugar dealer button is clicked.
///
void MainWindow::on_BuySugar_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->sugarImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(2);
    ui ->BuySugar->setEnabled(false);
    hasBoughtSugar = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet, 'f', 2));
}

/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyLemons_clicked When buy lemons button is clicked.
///
void MainWindow::on_BuyLemons_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->lemonsImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(3);
    ui ->BuyLemons-> setEnabled(false);
    hasBoughtLemon = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet, 'f', 2));
}

/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyNeonSIgn_clicked When buy neon sign button is clicked.
///
void MainWindow::on_BuyNeonSIgn_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->neonSignImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(0);
    ui ->BuyNeonSIgn ->setEnabled(false);
    hasBoughtSign = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet, 'f', 2));
}

///
/// Change image, wallet, and change backend.
/// \brief MainWindow::on_BuyInsurance_clicked When buy insurance button is clicked.
///
void MainWindow::on_BuyInsurance_clicked()
{
    QPixmap purchased(":/img/Images/Upgrades/Purchased.png");
    ui->insuranceImage->setPixmap(purchased.scaled(540, 250, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    emit updateWallet(6);
    ui->BuyInsurance ->setEnabled(false);
    hasBoughtInsurance = true;
    ui->walletLabel -> setText("Wallet: $ " + QString::number(game.stand.wallet, 'f', 2));
}

/// Sets up the game when the begin button is clicked.
/// \brief MainWindow::on_beginButton_clicked
///
void MainWindow::on_beginButton_clicked()
{
    emit showCalendar();
    ui->CreateLemonadeButton->setEnabled(true);
    ui->yesterdayButton->setEnabled(false);
    ui->welcomeFrame->setVisible(false);
    ui->welcomeLabel1->setVisible(false);
    ui->welcomeCheck2->setVisible(false);
    ui->welcomeCheck3->setVisible(false);
    ui->welcomeCheck4->setVisible(false);
}

/// Moves crowd image from left to right in simulation.
/// Also, adjusts the amount of poeple displayed based on demand.
/// \brief MainWindow::imageScroll
///
void MainWindow::imageScroll()
{
    int x = ui->crowdLabel->x();
    int y = ui->crowdLabel->y();
    int width = ui->crowdLabel->width();
    int height = ui->crowdLabel->height();
    int demand = game.yesterday().demanded;

    // How much the crowd moves per interval.
    x += 25;

    // Moves label and updates crowd image.
    ui->crowdLabel->setGeometry(x, y, width, height);
    if (demand < 44)
    {
        QPixmap lightCrowd(":/img/Images/Crowd_Levels/Crowd Light.png");
        ui->crowdLabel->setPixmap(lightCrowd);
    }
    else if (demand < 74)
    {
        QPixmap mediumCrowd(":/img/Images/Crowd_Levels/Crowd Medium.png");
        ui->crowdLabel->setPixmap(mediumCrowd);
    }
    else
    {
        QPixmap heavyCrowd(":/img/Images/Crowd_Levels/Crowd Heavy.png");
        ui->crowdLabel->setPixmap(heavyCrowd);
    }

    // Once label is completely off screen, resets label position and stops timer.
    if (x > width)
    {
        crowdTimer.stop();
        ui->crowdLabel->setGeometry(-1200, 450, 1147, 369);
        emit showCalendar();

        ui->demandLabel->setVisible(true);
        ui->profitLabel->setVisible(true);
        ui->salesLabel->setVisible(true);
        ui->costLabel->setVisible(true);
        ui->incomeLabel->setVisible(true);

        ui->demandDisplay->setVisible(true);
        ui->profitDisplay->setVisible(true);
        ui->salesDisplay->setVisible(true);
        ui->costDisplay->setVisible(true);
        ui->incomeDisplay->setVisible(true);

        ui->CreateLemonadeButton->setEnabled(true);
        ui->yesterdayButton->setEnabled(true);

        // Checks the current date, if the game should end, opens the end game dialog.
        if(game.currentDate == 15)
        {
            openEndGameDialog("");
        }
        if(game.currentDate == 14)
        {
            if (game.days[14].disaster == 3)
            {
                if(!hasBoughtUmbrella)
                {
                    openEndGameDialog("A whale fell on your stand, and because\nyou didn't buy an umbrella, you lose!\n");
                }
            }
        }
        if(game.currentDate == 10)
        {
            if(!hasBoughtInsurance)
            {
                openEndGameDialog("A tornado hit your stand, and because\nyou didn't buy any insurance, you lose!\n");
            }
        }
    }
}

/// Calls the close game dialog when the end game dialog has been closed.
/// \brief MainWindow::closeDialogClosed
/// \param i -- Not used, required to connect slot to the signal.
///
void MainWindow::closeDialogClosed(int i)
{
    closeGame();
}

/// Closes the game and the end game popup.
/// \brief MainWindow::closeGame
///
void MainWindow::closeGame()
{
    egPopup.close();
    this->close();
}

/// Gets all of the information needed for the total player statistics,
/// adds them to the end game popup, and shows the dialog.
/// \brief MainWindow::openEndGameDialog
///
void MainWindow::openEndGameDialog(QString message)
{
    QString playerStats = message + "Total Income: ";
    double income = 0;
    double cost = 0;
    int sales = 0;
    int daysSoldOut = 0;
    double profit = 0;
    // Get player stats for the whole game
    for(int i = 0; i < 15; i++)
    {
        income += game.days[i].income;
        cost += game.days[i].cost;
        sales += game.days[i].sales;
        profit += game.days[i].profit;
        if(game.days[i].soldOut)
            daysSoldOut++;

    }
    // Set the string for all of the stats and add it to the dialog
    playerStats.append(QString::number(income) +
                       "\nTotal Cost: " + QString::number(cost) +
                       "\nTotal Sales: " + QString::number(sales) +
                       "\nDays Sold Out: " + QString::number(daysSoldOut) +
                       "\nTotal Profit/Loss: " + QString::number(profit));
    egd.playerStatsLabel->setText(playerStats);
    // Show the popup
    egPopup.show();
}

/// Updates the cost label in the ingredients frame.
/// \brief MainWindow::updateIngredientsFrameCost
///
void MainWindow::updateIngredientsFrameCost()
{
    QString cost = QString::number(uiLemonadeCurrCost());
    ui->ingredientCostLabel->setText("$" + cost);
}

/// Updates the amount of cups of lemonade made per day.
/// \brief MainWindow::updateCupsofLemonadeMade
///
void MainWindow::updateCupsofLemonadeMade()
{
    int numPitchers = ui->pitchersSpinBox->value();
    int cupsMade = game.stand.cupsPerPitcher;
    QString cupsOfLemonade = QString::number(cupsMade * numPitchers);
    ui->cupsOfLemonadeText->setText(cupsOfLemonade);

}

/// Gets all of the information from the ingredients panel and calculates the total cost.
/// \brief MainWindow::uiLemonadeCurrCost
/// \return Total cost of all ingredients and #pitchers for the lemonade
///
double MainWindow::uiLemonadeCurrCost()
{
    int lemons = ui->LemonSpinBox->value();
    double lemonsCost = lemons * game.world.priceLemons();

    int sugar = ui->sugarSpinBox->value();
    double sugarCost = sugar * game.world.priceSugar();

    int ice = ui->iceSpinBox->value();
    double iceCost = ice * game.world.priceIce();

    int numPitchers = ui->pitchersSpinBox->value();

    double totalCost = (lemonsCost + sugarCost + iceCost) * numPitchers;

    return totalCost;
}

/// The below 4 slots all will update the cost on the ingredients panel
/// \brief MainWindow::lemonSpinBox_valueChanged, iceSpinBox_valueChanged, sugarSpinBox_valueChanged, pitcherSpinBox_valueChanged
/// \param i -- Not used, signal requires slot to have an int param
///
void MainWindow::lemonSpinBox_valueChanged(int i)
{
    updateIngredientsFrameCost();
}
void MainWindow::iceSpinBox_valueChanged(int i)
{
    updateIngredientsFrameCost();
}
void MainWindow::sugarSpinBox_valueChanged(int i)
{
    updateIngredientsFrameCost();
}
void MainWindow::pitcherSpinBox_valueChanged(int i)
{
    updateIngredientsFrameCost();
    updateCupsofLemonadeMade();
}

/// Gets all of the news stories to be displayed.
/// \brief MainWindow::getNewsStories
/// \param filePath
/// \return
///
QVector<QString>* MainWindow::getNewsStories(QString filePath)
{
    QFile storiesFile(filePath);
    QVector<QString>* storiesArray = new QVector<QString>;

    // Makes sure stories file can be opened
    if(!storiesFile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", storiesFile.errorString());
    }

    QTextStream input(&storiesFile);

    // While there are still stories in the file
    while(!input.atEnd())
    {
        QString story = input.readLine();
        storiesArray->append(story);
    }

    return storiesArray;
}

/// Creates whale animation in simulation.
/// \brief MainWindow::animationForWhale
///
void MainWindow::animationForWhale()
{
    int x = ui->whaleLabel->x();
    int y = ui->whaleLabel->y();
    int width = ui->whaleLabel->width();
    int height = ui->whaleLabel->height();

    y += 25;

    ui->whaleLabel->setGeometry(x, y, width, height);
    QPixmap whale(":/img/Images/Whale.png");
    ui->whaleLabel->setPixmap(whale);

    if (y > 300)
    {
        whaleTimer.stop();
    }
}
