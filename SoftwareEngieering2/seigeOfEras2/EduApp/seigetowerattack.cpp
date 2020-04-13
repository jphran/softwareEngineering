#include "seigetowerattack.h"

/*
 * Constructor to set up senario
 */
seigeTowerAttack::seigeTowerAttack() : world(b2Vec2(0.0f, -10.0f))
{
    // Create ground
    this->groundBodyPtr = this->createGround();

    // Create Box2D bodies in a wall formation
    this->createWall(); // assigns to this->wallBlockPtrs

    // Initalize with 0 hits
    this->numHits = 0;

    // set starting location
    this->siegeTowerStartingX = WALL_X_LOCATION - DISTANCE_TO_WALL;

    // Create the battering ram body
    this->siegeTowerPtr = this->createSiegeTower();

    // Run the simulation for a little bit to let the blocks fall into place
    for (int i = 0; i < INITIAL_NUM_TIMESTEPS; ++i)
    {
        // Tell the world to run a timestep
        world.Step(this->timeStep, this->velocityIterations, this->positionIterations);
    }
}

b2Body* seigeTowerAttack::createSiegeTower()
{
    float siegeTowerLocationX = this->siegeTowerStartingX;
    float siegeTowerLocationY = BOX_SIZE_Y * 2;

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(siegeTowerLocationX, siegeTowerLocationY);
    b2Body* body = this->world.CreateBody(&bodyDef);
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(BOX_SIZE_X, BOX_SIZE_Y*NUM_BLOCKS_IN_WALL);
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f; // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.restitution = BOUNCYNESS; // Set restitution for bouncy ness
    fixtureDef.friction = 0.0f; // Override the default friction.
    body->CreateFixture(&fixtureDef); // Add the shape to the body.
    return body;
}


b2Body* seigeTowerAttack::createGround()
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

/*
 * Generate the box2D b2Bodies that make up the wall
 */
void seigeTowerAttack::createWall()
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
void seigeTowerAttack::moveTower(vector<b2Vec2>& towerLocations, vector<float32>& towerAngles, vector<b2Vec2>& wall0Pos, vector<float32>& wall0Angle, vector<b2Vec2>& wall1Pos, vector<float32>& wall1Angle, vector<b2Vec2>& wall2Pos, vector<float32>& wall2Angle, vector<b2Vec2>& wall3Pos, vector<float32>& wall3Angle)
{
    towerAngles.clear(); towerLocations.clear(); wall0Pos.clear(); wall0Angle.clear(); wall1Pos.clear(); wall1Angle.clear(); wall2Pos.clear(); wall2Angle.clear(); wall3Pos.clear(); wall3Angle.clear();


    this->moveTowerTowardsWall(towerLocations, towerAngles, wall0Pos, wall0Angle, wall1Pos, wall1Angle, wall2Pos, wall2Angle, wall3Pos, wall3Angle);

    if (this->numHits >= 4) // first three hits
    {
        this->destroyWall(towerLocations, towerAngles, wall0Pos, wall0Angle, wall1Pos, wall1Angle, wall2Pos, wall2Angle, wall3Pos, wall3Angle);
    }
    this->numHits++;


    // Buffer
    // run simulation
    for (int i = 0; i < int(SENARIO_TIMESTEPS/3.0f); ++i)
    {
        // Get current location
        float currentX = siegeTowerPtr->GetPosition().x;
        float currentY = siegeTowerPtr->GetPosition().y;

        // Tell the world to run a timestep
        world.Step(timeStep, velocityIterations, positionIterations);

        // Get the position and angle of the ram box
        towerLocations.push_back(siegeTowerPtr->GetPosition());
        towerAngles.push_back(siegeTowerPtr->GetAngle());

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

// TODO: create a file with box2d helper stuff. Move this function to there
b2Body* seigeTowerAttack::createBody(float boxLocationX, float boxLocationY)
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

void seigeTowerAttack::moveTowerTowardsWall(vector<b2Vec2>& siegeTowerPosition, vector<float32>& siegeTowerAngle, vector<b2Vec2>& wall0Pos, vector<float32>& wall0Angle, vector<b2Vec2>& wall1Pos, vector<float32>& wall1Angle, vector<b2Vec2>& wall2Pos, vector<float32>& wall2Angle, vector<b2Vec2>& wall3Pos, vector<float32>& wall3Angle)
{
    float increment = (WALL_X_LOCATION - this->siegeTowerStartingX - BOX_SIZE_X) / (SENARIO_TIMESTEPS*4);

    // run simulation
    for (int i = 0; i < SENARIO_TIMESTEPS; ++i)
    {
        // Get current location
        float currentX = siegeTowerPtr->GetPosition().x;
        float currentY = siegeTowerPtr->GetPosition().y;

        // Move batteringram by increment
        this->siegeTowerPtr->SetTransform(b2Vec2(currentX + increment, currentY), this->siegeTowerPtr->GetAngle());

        // Tell the world to run a timestep
        world.Step(timeStep, velocityIterations, positionIterations);

        // Get the position and angle of the ram box
        siegeTowerPosition.push_back(siegeTowerPtr->GetPosition());
        siegeTowerAngle.push_back(siegeTowerPtr->GetAngle());

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

void seigeTowerAttack::destroyWall(vector<b2Vec2>& siegeTowerPosition, vector<float32>& siegeTowerAngle, vector<b2Vec2>& wall0Pos, vector<float32>& wall0Angle, vector<b2Vec2>& wall1Pos, vector<float32>& wall1Angle, vector<b2Vec2>& wall2Pos, vector<float32>& wall2Angle, vector<b2Vec2>& wall3Pos, vector<float32>& wall3Angle)
{
    // run simulation
    for (int i = 0; i < SENARIO_TIMESTEPS*3; ++i)
    {
        this->wallBlockPtrs[0]->ApplyForce(b2Vec2(1000, -200), this->wallBlockPtrs[0]->GetWorldCenter(), true);
        this->wallBlockPtrs[1]->ApplyForce(b2Vec2(3000, -200), this->wallBlockPtrs[1]->GetWorldCenter(), true);
        this->wallBlockPtrs[2]->ApplyForce(b2Vec2(5000, -200), this->wallBlockPtrs[0]->GetWorldCenter(), true);
        this->wallBlockPtrs[3]->ApplyForce(b2Vec2(7000, -200), this->wallBlockPtrs[1]->GetWorldCenter(), true);

        // Tell the world to run a timestep
        world.Step(timeStep, velocityIterations, positionIterations);

        // Get the position and angle of the ram box
        siegeTowerPosition.push_back(siegeTowerPtr->GetPosition());
        siegeTowerAngle.push_back(siegeTowerPtr->GetAngle());

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

