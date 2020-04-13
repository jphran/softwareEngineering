/*
 * Created with some help from the box2d hello world example
 */
#include "batteringramattack.h"

// TODO: probs dont need that many init timesteps
#define INITIAL_NUM_TIMESTEPS 60
#define SENARIO_TIMESTEPS 100

// TODO: Refactor by making functions for repeated blocks of code.
//       This code is a mess and needs refactoring anyways.


/*
 * Constructor to set up senario
 */
BatteringRamAttack::BatteringRamAttack() : world(b2Vec2(0.0f, -10.0f))
{   
    // Create ground
    this->groundBodyPtr = this->createGround();

    // Initalize with 0 hits
    this->numHits = 0;

    // Create Box2D bodies in a wall formation
    this->createWall(); // assigns to this->wallBlockPtrs

    // Create the battering ram body
    this->batteringRamBodyPtr = this->createBatteringRam();

    // Run the simulation for a little bit to let the blocks fall into place
    for (int i = 0; i < INITIAL_NUM_TIMESTEPS-10; ++i)
    {
        // Tell the world to run a timestep
        world.Step(timeStep, velocityIterations, positionIterations);
    }
}

b2Body* BatteringRamAttack::createBatteringRam()
{
    // vars declared here for quick changing
    float ramSizeX = 3.0f * BOX_SIZE_X;
    float ramSizeY = 0.5f * BOX_SIZE_Y;
    float boxLocationX = WALL_X_LOCATION - BOX_SIZE_X - ramSizeX;
    float boxLocationY = (BOX_SIZE_Y * NUM_BLOCKS_IN_WALL)/2.0f;
    float bouncynessOverride = BOUNCYNESS + 0.2f;
    float density = 2.0f;

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;

    // Save starting pos to reset ram position later
    this->startingRamPositionX = boxLocationX;
    this->startingRamPositionY = boxLocationY;

    bodyDef.position.Set(boxLocationX, boxLocationY);
    b2Body* body = this->world.CreateBody(&bodyDef);
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(ramSizeX, ramSizeY);
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = density; // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.restitution = bouncynessOverride; // Set restitution for bouncy ness
    fixtureDef.friction = 0.3f; // Override the default friction.
    body->CreateFixture(&fixtureDef); // Add the shape to the body.

    body->SetGravityScale(0); // disable gravity

    return body;
}


b2Body* BatteringRamAttack::createGround()
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

// TODO: create a file with box2d helper stuff. Move this function to there
b2Body* BatteringRamAttack::createBody(float boxLocationX, float boxLocationY)
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



/*
 * Generate the box2D b2Bodies that make up the wall
 */
void BatteringRamAttack::createWall()
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


/*
 * Call when user answers question correctly
 */
void BatteringRamAttack::hit(vector<b2Vec2>& batteringRamPos, vector<float32>& batteringRamAngle, vector<b2Vec2>& wall0Pos, vector<float32>& wall0Angle, vector<b2Vec2>& wall1Pos, vector<float32>& wall1Angle, vector<b2Vec2>& wall2Pos, vector<float32>& wall2Angle, vector<b2Vec2>& wall3Pos, vector<float32>& wall3Angle)
{

    batteringRamPos.clear(); batteringRamAngle.clear(); wall0Pos.clear(); wall0Angle.clear(); wall1Pos.clear(); wall2Angle.clear(); wall3Pos.clear(); wall3Angle.clear();

    if(this->numHits < 3)
    {
        this->move(batteringRamPos, batteringRamAngle, wall0Pos, wall0Angle, wall1Pos, wall1Angle, wall2Pos, wall2Angle, wall3Pos, wall3Angle);
    }
    else
    {
        this->hitAndbreakWall(batteringRamPos, batteringRamAngle, wall0Pos, wall0Angle, wall1Pos, wall1Angle, wall2Pos, wall2Angle, wall3Pos, wall3Angle);
    }
        numHits++;
}

void BatteringRamAttack::hitAndbreakWall(vector<b2Vec2>& batteringRamPos, vector<float32>& batteringRamAngle, vector<b2Vec2>& wall0Pos, vector<float32>& wall0Angle, vector<b2Vec2>& wall1Pos, vector<float32>& wall1Angle, vector<b2Vec2>& wall2Pos, vector<float32>& wall2Angle, vector<b2Vec2>& wall3Pos, vector<float32>& wall3Angle)
{
    // run simulation
    for (int i = 0; i < SENARIO_TIMESTEPS; ++i)
    {
        // Push ram towards the wall
        batteringRamBodyPtr->ApplyForce(b2Vec2(100, 0), batteringRamBodyPtr->GetWorldCenter(), true);

        // Tell the world to run a timestep
        world.Step(timeStep, velocityIterations, positionIterations);

        // Get the position and angle of the ram box
        batteringRamPos.push_back(batteringRamBodyPtr->GetPosition());
        batteringRamAngle.push_back(batteringRamBodyPtr->GetAngle());

        // Get the position and angle of each block in the wall
        wall0Pos.push_back(wallBlockPtrs[0]->GetPosition());
        wall0Angle.push_back(wallBlockPtrs[0]->GetAngle());

        wall1Pos.push_back(wallBlockPtrs[1]->GetPosition());
        wall1Angle.push_back(wallBlockPtrs[1]->GetAngle());

        wall2Pos.push_back(wallBlockPtrs[2]->GetPosition());
        wall2Angle.push_back(wallBlockPtrs[2]->GetAngle());

        wall3Pos.push_back(wallBlockPtrs[3]->GetPosition());
        wall3Angle.push_back(wallBlockPtrs[3]->GetAngle());

    }
}

void BatteringRamAttack::move(vector<b2Vec2>& batteringRamPos, vector<float32>& batteringRamAngle, vector<b2Vec2>& wall0Pos, vector<float32>& wall0Angle, vector<b2Vec2>& wall1Pos, vector<float32>& wall1Angle, vector<b2Vec2>& wall2Pos, vector<float32>& wall2Angle, vector<b2Vec2>& wall3Pos, vector<float32>& wall3Angle)
{
    // run simulation
    for (int i = 0; i < SENARIO_TIMESTEPS; ++i)
    {
        // Push ram towards the wall
        batteringRamBodyPtr->ApplyForce(b2Vec2(2, 0), batteringRamBodyPtr->GetWorldCenter(), true);


        // Tell the world to run a timestep
        world.Step(timeStep, velocityIterations, positionIterations);

        // Get the position and angle of the ram box
        batteringRamPos.push_back(batteringRamBodyPtr->GetPosition());
        batteringRamAngle.push_back(batteringRamBodyPtr->GetAngle());

        // Get the position and angle of each block in the wall
        wall0Pos.push_back(wallBlockPtrs[0]->GetPosition());
        wall0Angle.push_back(wallBlockPtrs[0]->GetAngle());

        wall1Pos.push_back(wallBlockPtrs[1]->GetPosition());
        wall1Angle.push_back(wallBlockPtrs[1]->GetAngle());

        wall2Pos.push_back(wallBlockPtrs[2]->GetPosition());
        wall2Angle.push_back(wallBlockPtrs[2]->GetAngle());

        wall3Pos.push_back(wallBlockPtrs[3]->GetPosition());
        wall3Angle.push_back(wallBlockPtrs[3]->GetAngle());
    }
}


