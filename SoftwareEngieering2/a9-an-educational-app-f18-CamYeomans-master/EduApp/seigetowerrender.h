#ifndef SEIGETOWERRENDER_H
#define SEIGETOWERRENDER_H


#include "seigetowerattack.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <QLabel>

#define WINDOW_WIDTH 600
#define WINDOW_HIEGHT 400

namespace Ui {
class SeigeTowerRender;
}

class SeigeTowerRender : public QLabel
{
    Q_OBJECT

public:
    explicit SeigeTowerRender(QWidget *parent = 0);
    ~SeigeTowerRender();
    seigeTowerAttack stower;

signals:
    void goBackSignal();

private:
    sf::RenderWindow window;
    std::vector<sf::Sprite> wall;
    sf::Sprite st;
    vector<sf::Texture> textures;

    vector<b2Vec2> stPos;
    vector<float32> stRot;
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

#endif // SEIGETOWERRENDER_H
