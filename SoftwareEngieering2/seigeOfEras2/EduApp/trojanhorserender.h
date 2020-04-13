#ifndef TROJANHORSERENDER_H
#define TROJANHORSERENDER_H

#include "trojanhorseattack.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <QLabel>

#define WINDOW_WIDTH 600
#define WINDOW_HIEGHT 400

namespace Ui {
class TrojanHorseRender;
}

class TrojanHorseRender : public QLabel
{
    Q_OBJECT

public:
    explicit TrojanHorseRender(QWidget *parent = 0);
    ~TrojanHorseRender();
    trojanHorseAttack thorse;
signals:
    void goBackSignal();

private:
    sf::RenderWindow window;
    std::vector<sf::Sprite> wall;
    sf::Sprite horse;
    vector<sf::Texture> textures;

    float transformCoords(float);

    vector<b2Vec2> horsePos;
    vector<float32> horseRot;
    vector<vector<b2Vec2>> wallPoss;
    vector<vector<float32>> wallRots;

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

#endif // TROJANHORSERENDER_H
