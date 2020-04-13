#ifndef BATTERINGRAMATTACK_H
#define BATTERINGRAMATTACK_H

#include "Box2D/Box2D.h"
#include <stdio.h>
#include <vector>
using namespace std;

// TODO: These defines should be changed to static class variables
// IMPORTANT: if you change the num blocks in wall, dont forget to change inputs of the hit() function
#define NUM_BLOCKS_IN_WALL 4
#define BOUNCYNESS 0.2f
#define BOX_SIZE_X 1.0f
#define BOX_SIZE_Y 1.0f
#define WALL_X_LOCATION 0.0f

class BatteringRamAttack
{
public:
    BatteringRamAttack();
    void hit(vector<b2Vec2>&, vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&);

private:
    int numHits;
    void move(vector<b2Vec2>&, vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&);
    void hitAndbreakWall(vector<b2Vec2>&, vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&, vector<b2Vec2>&,  vector<float32>&);
    void createWall();
    b2Body* wallBlockPtrs[NUM_BLOCKS_IN_WALL]; // 0 index = bottom block, n index = highest up block
    b2Body* createBody(float x, float y);
    b2Body* createGround();
    b2World world;
    b2Body* groundBodyPtr;
    b2Body* createBatteringRam();
    b2Body* batteringRamBodyPtr;
    float startingRamPositionX;
    float startingRamPositionY;

    // Idk where these constants came from, but box2d reccomends these numbers
    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
};

#endif // BATTERINGRAMATTACK_H
