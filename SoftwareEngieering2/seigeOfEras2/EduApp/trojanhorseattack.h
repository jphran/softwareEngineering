#ifndef TROJANHORSEATTACK_H
#define TROJANHORSEATTACK_H

#include "Box2D/Box2D.h"
#include <stdio.h>
#include <vector>

#define INITIAL_NUM_TIMESTEPS 60
#define SENARIO_TIMESTEPS 100
#define DISTANCE_TO_WALL 10
#define NUM_BLOCKS_IN_WALL 4
#define BOUNCYNESS 0.2f
#define BOX_SIZE_X 1.0f
#define BOX_SIZE_Y 1.0f
#define WALL_X_LOCATION 0.0f

using namespace std;

class trojanHorseAttack
{
public:
    trojanHorseAttack();
    void moveTower(vector<b2Vec2>&, vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&);

private:
    b2Body* createGround();
    b2Body* createtrojanHorse();
    void createWall();
    b2Body* wallBlockPtrs[NUM_BLOCKS_IN_WALL]; // 0 index = bottom block, n index = highest up block
    b2World world;
    b2Body* groundBodyPtr;
    b2Body* trojanHorsePtr;
    int numHits;
    void moveTowerTowardsWall(vector<b2Vec2>&, vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&);
    void lowerWall(vector<b2Vec2>&, vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&);
    b2Body* createBody(float, float);
    float trojanHorseStartingX;



    // Idk where these constants came from, but box2d reccomends these numbers
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;


};

#endif // TROJANHORSEATTACK_H
