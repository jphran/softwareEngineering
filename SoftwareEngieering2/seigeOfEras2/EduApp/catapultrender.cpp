#include "catapultrender.h"

#include "catapultrender.h"

CatapultRender::CatapultRender(QWidget *parent) :
    QLabel(parent)
{
    //create sprites
    sf::Texture catapultTexture;
    catapultTexture.loadFromFile("../EduApp/smallBoulder.png", sf::IntRect(0, 0, 285, 222));
    textures.push_back(catapultTexture);
    sf::Texture blockTexture;
    blockTexture.loadFromFile("../EduApp/smallblock.png", sf::IntRect(0, 0, 80, 80));
    textures.push_back(blockTexture);
    sf::Texture standTexture;
    blockTexture.loadFromFile("../EduApp/smallcatapult.jpg", sf::IntRect(0, 0, 144, 102));
    textures.push_back(blockTexture);

    vector<b2Vec2> blockPos;
    vector<float32> blockRot;

    textures[0].setSmooth(true);
    textures[1].setSmooth(true);
    textures[2].setSmooth(true);
    textures[0].setRepeated(true);
    textures[1].setRepeated(true);
    textures[2].setRepeated(true);

    //create sprites and set positions
    cata.setTexture(textures[0]);
    cata.setOrigin(sf::Vector2f(25.f, 25.f));
    sf::Sprite block;
    block.setTexture(textures[1]);
    for (int i = 0; i < 4; i++){
        wallPoss.push_back(blockPos);
        wallRots.push_back(blockRot);
        block.setOrigin(sf::Vector2f(25.f, 25.f));
        wall.push_back(block);
    }
    stand.setTexture(textures[2]);
    stand.setOrigin(sf::Vector2f(25.f, 25.f));
    stand.setPosition(480, 340);

}

void CatapultRender::init(){
    if(timesRan < 4){
        //create the window
        window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HIEGHT), "English Siege");
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(24);
        window.setPosition(sf::Vector2i(1050, 50));
        itr = 0;
        while(window.isOpen()){
            if(!running){
                catapult.throwProjectile(cataPos, cataRot, wallPoss.at(0), wallRots.at(0),
                         wallPoss.at(1), wallRots.at(1), wallPoss.at(2),
                         wallRots.at(2), wallPoss.at(3), wallRots.at(3));
                timesRan++;
                running = true;
            }else{
                events();
                update();
                render();
            }
         }
     }
}

float CatapultRender::transformCoords(float input)
{
    float windowHeight = float(WINDOW_HIEGHT);
    return windowHeight - input;
}

//These are the working numbers
void CatapultRender::update(){
    b2Vec2 position;

    position = cataPos.at(itr);
    cata.setPosition((((-position.x*20) + 360)), transformCoords((position.y+3)*30));
    cata.setRotation(cataRot.at(itr));
    for (int i = 0; i < 4; i++) {
        position = wallPoss[i].at(itr);
        wall.at(i).setPosition(((position.x+22)*(-10)+450), transformCoords(position.y*40));
        wall.at(i).setRotation(wallRots[i].at(itr));
    }
    itr++;
    if (itr == cataPos.size() - 1){
        running = false;
        emit goBackSignal();
        window.close();
    }
}

void CatapultRender::events(){
    sf::Event event;
    while(window.pollEvent(event)){
        if(event.type == sf::Event::Closed){
            emit goBackSignal();
            window.close();
        }
    }
}

void CatapultRender::render(){
    window.clear(sf::Color::White);
    for (sf::Sprite block : wall) window.draw(block);
    window.draw(cata);
    window.draw(stand);
    window.display();
}

CatapultRender::~CatapultRender(){
}
