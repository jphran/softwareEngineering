#include "trojanhorserender.h"

TrojanHorseRender::TrojanHorseRender(QWidget *parent) :
    QLabel(parent)
{
    //create sprites
    sf::Texture horseTexture;
    horseTexture.loadFromFile("../EduApp/smallthorse.jpg", sf::IntRect(0, 0, 285, 222));
    textures.push_back(horseTexture);
    sf::Texture blockTexture;
    blockTexture.loadFromFile("../EduApp/smallblock.png", sf::IntRect(0, 0, 80, 80));
    textures.push_back(blockTexture);

    vector<b2Vec2> blockPos;
    vector<float32> blockRot;

    textures[0].setSmooth(true);
    textures[1].setSmooth(true);
    textures[0].setRepeated(true);
    textures[1].setRepeated(true);

    //create sprites and set positions
    horse.setTexture(textures[0]);
    horse.setOrigin(sf::Vector2f(25.f, 25.f));
    sf::Sprite block;
    block.setTexture(textures[1]);
    for (int i = 0; i < 4; i++){
        wallPoss.push_back(blockPos);
        wallRots.push_back(blockRot);
        block.setOrigin(sf::Vector2f(25.f, 25.f));
        wall.push_back(block);
    }

}

void TrojanHorseRender::init(){
    if(timesRan < 4){
        //create the window
        window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HIEGHT), "Greek Siege");
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(24);
        window.setPosition(sf::Vector2i(1050, 50));
        itr = 0;
        while(window.isOpen()){
            if(!running){
                thorse.moveTower(horsePos, horseRot, wallPoss.at(0), wallRots.at(0),
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

float TrojanHorseRender::transformCoords(float input)
{
    float windowHeight = float(WINDOW_HIEGHT);
    return windowHeight - input;
}

void TrojanHorseRender::update(){
    b2Vec2 position;

    position = horsePos.at(itr);
    horse.setPosition((-position.x*30+80), transformCoords(position.y+120));
    horse.setRotation(horseRot.at(itr));
    for (int i = 0; i < 4; i++) {
        position = wallPoss[i].at(itr);
        wall.at(i).setPosition(position.x+80, transformCoords(position.y*40));
        wall.at(i).setRotation(wallRots[i].at(itr));
    }
    itr++;
    if (itr == horsePos.size() - 1){
        running = false;
        emit goBackSignal();
        window.close();
    }
}

void TrojanHorseRender::events(){
    sf::Event event;
    while(window.pollEvent(event)){
        if(event.type == sf::Event::Closed){
            emit goBackSignal();
            window.close();
        }
    }
}

void TrojanHorseRender::render(){
    window.clear(sf::Color::White);
    window.draw(horse);
    for (sf::Sprite block : wall) window.draw(block);
    window.display();
}

TrojanHorseRender::~TrojanHorseRender(){
}
