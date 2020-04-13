#include "catapultattack.h"
#include <iostream>

// TODO: make a box2d wrapper class that builds all the common elements. Import that class and use instead of copying and pasting code

catapultAttack::catapultAttack() : world(b2Vec2(0.0f, -10.0f))
{
    // Create ground
    this->groundBodyPtr = this->createGround();

    // Create Box2D bodies in a wall formation
    this->createWall(); // assigns to this->wallBlockPtrs

    // Create vectors to throw
    b2Vec2 projectile1(12.0f * 5.0f, 10.0f);
    projectileVectors.push_back(projectile1);
    b2Vec2 projectile2(14.0f * 5.0f, 2.0f);
    projectileVectors.push_back(projectile2);
    b2Vec2 projectile3(20.0f * 5.0f, 3.0f);
    projectileVectors.push_back(projectile3);
    b2Vec2 projectile4(30.0f * 5.0f, 0.0f);
    projectileVectors.push_back(projectile4);


    projectileNumber = 0;

    // Run the simulation for a little bit to let the blocks fall into place
    for (int i = 0; i < INITIAL_NUM_TIMESTEPS; ++i)
    {
        // Tell the world to run a timestep
        world.Step(this->timeStep, this->velocityIterations, this->positionIterations);
        wallBlockPtrs[0]->SetAwake(true);
        wallBlockPtrs[1]->SetAwake(true);
        wallBlockPtrs[2]->SetAwake(true);
        wallBlockPtrs[3]->SetAwake(true);
    }
}

void catapultAttack::throwProjectile(vector<b2Vec2>& projectilePosition, vector<float32>& projectileAngle, vector<b2Vec2>& wall0Pos, vector<float32>& wall0Angle, vector<b2Vec2>& wall1Pos, vector<float32>& wall1Angle, vector<b2Vec2>& wall2Pos, vector<float32>& wall2Angle, vector<b2Vec2>& wall3Pos, vector<float32>& wall3Angle)
{
    projectilePosition.clear(); projectileAngle.clear(); wall0Pos.clear(); wall0Angle.clear(); wall1Pos.clear(); wall1Angle.clear(); wall2Pos.clear(); wall2Angle.clear(); wall3Pos.clear(); wall3Angle.clear();

    float projectileInitialX = WALL_X_LOCATION-5;
    float projectileInitialY = BOX_SIZE_Y*1.5f;

    // Create a projectile
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(projectileInitialX, projectileInitialY);
    b2Body* projectile = this->world.CreateBody(&bodyDef);
    // Define another box shape for our dynamic body.

    b2CircleShape dynamicCircle;
    dynamicCircle.m_p.Set(projectileInitialX, projectileInitialY);
    dynamicCircle.m_radius = BOX_SIZE_X/2.0f;

    // Define the dynamic body fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicCircle;
    fixtureDef.density = 2.0f; // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.restitution = BOUNCYNESS*1.2f; // Set restitution for bouncy ness
    fixtureDef.friction = 1.0f; // Override the default friction.
    projectile->CreateFixture(&fixtureDef); // Add the shape to the body.


    b2Vec2 forceDirection = this->projectileVectors[projectileNumber];
    projectileNumber++;


    // Run simulation to throw projectile
    for (int i = 0; i < SENARIO_TIMESTEPS; ++i)
    {
        wallBlockPtrs[0]->SetAwake(true);
        wallBlockPtrs[1]->SetAwake(true);
        wallBlockPtrs[2]->SetAwake(true);
        wallBlockPtrs[3]->SetAwake(true);

        // Throw the block at the wall by applying a force on the projectile
        projectile->ApplyForce(forceDirection, projectile->GetWorldCenter(), true);

        // Tell the world to run a timestep
        world.Step(timeStep, velocityIterations, positionIterations);

        // Get the position and angle of the ram box
        projectilePosition.push_back(projectile->GetPosition());
        projectileAngle.push_back(projectile->GetAngle());

        // Get the position and angle of each block in the wall
        wall0Pos.push_back(wallBlockPtrs[0]->GetPosition());
        wall0Angle.push_back(wallBlockPtrs[0]->GetAngle());

        wall1Pos.push_back(wallBlockPtrs[1]->GetPosition());
        wall1Angle.push_back(wallBlockPtrs[1]->GetAngle());

        wall2Pos.push_back(wallBlockPtrs[2]->GetPosition());
        wall2Angle.push_back(wallBlockPtrs[2]->GetAngle());

        wall3Pos.push_back(wallBlockPtrs[3]->GetPosition());
        wall3Angle.push_back(wallBlockPtrs[3]->GetAngle());

        std::cout << wallBlockPtrs[3]->GetPosition().x << std::endl;

    }
    // Grab the next projectile
    projectileNumber++;
}

/*
 * Generate the box2D b2Bodies that make up the wall
 */
void catapultAttack::createWall()
{
    // Drop boxes into place
    float xPos = WALL_X_LOCATION;
    float yPos = BOX_SIZE_Y * 2;
    for(int i = 0; i < NUM_BLOCKS_IN_WALL; i++)
    {
        this->wallBlockPtrs[i] = this->createBody(xPos, yPos);
        yPos = yPos + BOX_SIZE_Y;
    }
}


// TODO: create a file with box2d helper stuff. Move this function to there
b2Body* catapultAttack::createBody(float boxLocationX, float boxLocationY)
{
    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(boxLocationX, boxLocationY);
    b2Body* body = this->world.CreateBody(&bodyDef);
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(BOX_SIZE_X, BOX_SIZE_Y);
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f; // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.restitution = BOUNCYNESS; // Set restitution for bouncy ness
    fixtureDef.friction = 0.0f; // Override the default friction.
    body->CreateFixture(&fixtureDef); // Add the shape to the body.
    return body;
}


b2Body* catapultAttack::createGround()
{
    b2BodyDef groundBodyDef; // Define the ground body.
    groundBodyDef.position.Set(0.0f, -10.0f);
    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = this->world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox; // Define the ground box shape.
    groundBox.SetAsBox(50.0f, 10.0f); // The extents are the half-widths of the box.
    groundBody->CreateFixture(&groundBox, 0.0f); // Add the ground fixture to the ground body.
    return groundBody;
}
