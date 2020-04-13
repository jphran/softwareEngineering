#ifndef CATAPULTRENDER_H
#define CATAPULTRENDER_H

#include "catapultattack.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <QLabel>

#define WINDOW_WIDTH 600
#define WINDOW_HIEGHT 400

namespace Ui {
class CatapultRender;
}

class CatapultRender : public QLabel
{
    Q_OBJECT

public:
    explicit CatapultRender(QWidget *parent = 0);
    ~CatapultRender();
    catapultAttack catapult;

signals:
    void goBackSignal();

private:
    sf::RenderWindow window;
    std::vector<sf::Sprite> wall;
    sf::Sprite cata;
    sf::Sprite stand;
    vector<sf::Texture> textures;

    vector<b2Vec2> cataPos;
    vector<float32> cataRot;
    vector<vector<b2Vec2>> wallPoss;
    vector<vector<float32>> wallRots;

    float transformCoords(float);

    int itr = 0;
    int timesRan = 0;
    bool running = false;
    bool initialized = false;

    void render();
    void events();
    void loadTextures();

public slots:
    void init();
    void update();
};

#endif // CATAPULTRENDER_H
