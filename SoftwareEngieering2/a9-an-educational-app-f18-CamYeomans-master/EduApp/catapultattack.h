#ifndef CATAPULTATTACK_H
#define CATAPULTATTACK_H

#include "Box2D/Box2D.h"
#include <stdio.h>
#include <vector>

#define INITIAL_NUM_TIMESTEPS 60
#define SENARIO_TIMESTEPS 100

#define NUM_BLOCKS_IN_WALL 4
#define BOUNCYNESS 0.2f
#define BOX_SIZE_X 1.0f
#define BOX_SIZE_Y 1.0f
#define WALL_X_LOCATION 0.0f

using namespace std;

class catapultAttack
{
public:
    catapultAttack();
    void throwProjectile(vector<b2Vec2>&, vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&);

private:
    b2Body* createGround();
    b2Body* groundBodyPtr;
    vector<b2Vec2> projectileVectors;
    b2Body* wallBlockPtrs[NUM_BLOCKS_IN_WALL]; // 0 index = bottom block, n index = highest up block
    void createWall();
    b2Body* createBody(float, float);
    b2World world;
    int projectileNumber;


    // Idk where these constants came from, but box2d reccomends these numbers
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
};

#endif // CATAPULTATTACK_H
