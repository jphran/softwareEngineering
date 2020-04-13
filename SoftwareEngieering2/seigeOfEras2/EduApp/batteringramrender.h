#ifndef BATTERINGRAMRENDER_H
#define BATTERINGRAMRENDER_H


#include "batteringramattack.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <QLabel>
#include <QTimer>

#define WINDOW_WIDTH 600
#define WINDOW_HIEGHT 400

namespace Ui {
class BatteringRamRender;
}

class BatteringRamRender : public QLabel
{
    Q_OBJECT

public:
    explicit BatteringRamRender(QWidget *parent = 0);
    ~BatteringRamRender();
    BatteringRamAttack btrm;

signals:
    void goBackSignal();

private:
    sf::RenderWindow window;
    std::vector<sf::Sprite> wall;
    sf::Sprite ram;
    vector<sf::Texture> textures;

    vector<b2Vec2> ramPos;
    vector<float32> ramRot;
    vector<vector<b2Vec2>> wallPoss;
    vector<vector<float32>> wallRots;

    float transformCoords(float input);

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

#endif // BATTERINGRAMRENDER_H
