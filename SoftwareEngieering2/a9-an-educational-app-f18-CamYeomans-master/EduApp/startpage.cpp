#include "startpage.h"
#include "ui_startpage.h"
#include <QMessageBox>
#include <QDesktopWidget>
#include "Box2D/Box2D.h"
#include <QTimer>


startPage::startPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::startPage), world(b2Vec2(0.0f,-10.0f))
{

    ui->setupUi(this);
    //ui->newGameButton->setEnabled(false);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    QRect desktopRect = QApplication::desktop()->availableGeometry(this);
    QPoint center = desktopRect.center();

    move(center.x() - width() * 0.5, center.y() - height() * 0.5);

    // set background image
    QPixmap background("../EduApp/splash_screen_image.jpg");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, background);
    this->setPalette(palette);
    setupWorld();

}

startPage::~startPage()
{
    delete ui;
}

/**
* overrides showEvent to play music when window opens
* */
void startPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
}

/**
 * overrides closeEvent to stop music when window closes
 */
void startPage::closeEvent(QCloseEvent* event) {
    QWidget::closeEvent(event);
}

/**
 * closes start page and opens view
 */
void startPage::on_newGameButton_clicked()
{
    this->close();
    emit openView();
}



//////////////////////////////////////////physics 2d
void startPage::setupWorld()
{
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 10.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    body = world.CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;

    fixtureDef.restitution = 0.9;

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    //connect(this, &startPage::newHeight, ui->verticalSlider,&QSlider::setValue);
    QTimer::singleShot(30,this,&startPage::updateWorld);
}

void startPage::updateWorld()
{
    world.Step(1.0f /60.f,6,2);
    b2Vec2 position = body->GetPosition();
    emit newHeight(position.y*100);
    QTimer::singleShot(30,this,&startPage::updateWorld);
}

void startPage::on_helpButton_clicked()
{
    QMessageBox msg;
    msg.setText("Welcome to Siege of Eras! In this mini-game, you will be learning about the siege tactics of different major kingdoms throughout history! You will first learn about a location after clicking it, and after reading (or listening) to the facts, you can select the trivia option to enter the game (or select images to looks at some cool pictures relating to the location). The more questions you get right, the higher your score, and the more likely you'll succeed when you siege an enemy castle! Good luck and have fun :)");
    QFont newFont("URW Chancery L", 13, QFont::Bold, true);
    msg.setFont(newFont);
    msg.exec();
}

//void startPage::on_textEdit_textChanged()
//{
//    QString text =ui->textEdit->toPlainText();
//    if(text == "")
//    {
//        ui->newGameButton->setEnabled(false);
//    }
//    else
//    {
//        ui->newGameButton->setEnabled(true);
//    }
//}
