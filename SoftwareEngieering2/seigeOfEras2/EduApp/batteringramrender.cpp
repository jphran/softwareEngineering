#include "batteringramrender.h"

BatteringRamRender::BatteringRamRender(QWidget *parent) :
    QLabel(parent)
{
    //create sprites
    sf::Texture ramTexture;
    ramTexture.loadFromFile("../EduApp/smallram.png", sf::IntRect(0, 0, 167, 120));
    textures.push_back(ramTexture);
    sf::Texture blockTexture;
    blockTexture.loadFromFile("../EduApp/smallblock.png", sf::IntRect(0, 0, 80, 80));
    textures.push_back(blockTexture);

    vector<b2Vec2> blockPos;
    vector<float32> blockRot;

    textures[0].setSmooth(true);
    textures[1].setSmooth(true);
    textures[0].setRepeated(true);
    textures[1].setRepeated(true);

    ram.setTexture(textures[0]);
    ram.setPosition(600, 360);
    ram.setOrigin(sf::Vector2f(25.f, 25.f));
    sf::Sprite block;
    block.setTexture(textures[1]);
    for (int i = 0; i < 4; i++){
        wallPoss.push_back(blockPos);
        wallRots.push_back(blockRot);
        block.setPosition(50, 330 - (80*i));
        block.setOrigin(sf::Vector2f(25.f, 25.f));
        wall.push_back(block);
    }

}

void BatteringRamRender::init()
{
    if(timesRan < 4)
    {
        //create the window
        window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HIEGHT), "French Siege");
        window.setVerticalSyncEnabled(true);
        window.setFramerateLimit(24);
        window.setPosition(sf::Vector2i(1050, 50));
        itr = 0;
        while(window.isOpen())
        {
            if(!running)
            {
                btrm.hit(ramPos, ramRot, wallPoss.at(0), wallRots.at(0),
                         wallPoss.at(1), wallRots.at(1), wallPoss.at(2),
                         wallRots.at(2), wallPoss.at(3), wallRots.at(3));
                timesRan++;
                running = true;
            }
            else
            {
                events();
                update();
                render();
            }
         }
     }
}

float BatteringRamRender::transformCoords(float input)
{
    float windowHeight = float(WINDOW_HIEGHT);
    return windowHeight - input;
}

void BatteringRamRender::update()
{
    b2Vec2 position = ramPos.at(itr);
    ram.setPosition((((-position.x)*100)-200), transformCoords(position.y+80));
    ram.setRotation(ramRot.at(itr));
    for (int i = 0; i < 4; i++)
    {
        position = wallPoss[i].at(itr);
        wall.at(i).setPosition(position.x+80, transformCoords(position.y*40));
        wall.at(i).setRotation(wallRots[i].at(itr));
    }
    itr++;

    if (itr == ramPos.size() - 1)
    {
        running = false;
        emit goBackSignal();
        window.close();
    }

}



void BatteringRamRender::events()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            emit goBackSignal();
            window.close();
        }
    }
}

void BatteringRamRender::render()
{
    window.clear(sf::Color::White);
    window.draw(ram);
    for (sf::Sprite block : wall) window.draw(block);
    window.display();
}

BatteringRamRender::~BatteringRamRender(){
}
