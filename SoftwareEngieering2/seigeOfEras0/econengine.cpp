/**
  * This class is mimicing a "model".
  * Created by Serena Aeschilman, Spencer Elkington, Andrew Stender, Evan Voordeckers, Ryan Williamson, Keegan Spencer, and Theaux Masqulier.
  */

#include "econengine.h"
#include <math.h>
#include "upgrades.h"
#include <QDebug>

#define TEMPERATURE_AVERAGE 85
#define TEMPERATURE_VARIANCE 5
#define BASE_RECIPE_BONUS 50
#define REPUTATION_POINTS_PER_SALE 0.3
#define MAX_DEMAND 300
#define MIN_DEMAND 0
#define PERFECT_LEMONS 6
#define PERFECT_SUGAR  4
#define PERFECT_PRICE 1.5

// Initialize the instance pointer to null.
EconEngine* EconEngine::m_engineInstance = NULL;

///
/// \brief EconEngine::instance Creates an instance of our engine.
/// \return Returns the created instance.
///
EconEngine* EconEngine::instance()
{
    // If an engine instance has not been created yet, do so.
    if (!m_engineInstance)
    {
        m_engineInstance = new EconEngine();
    }

    // Return the pointer for the singleton instance of the engine.
    return m_engineInstance;
}

///
/// \brief EconEngine::gameState Gettter for game object.
/// \return The game object.
///
GameState* EconEngine::gameState()
{
    return &EconEngine::instance()->game;
}

///
/// \brief EconEngine::EconEngine Constructs an EconEngine object.
/// \param parent
///
EconEngine::EconEngine(QObject *parent) : QObject(parent)
{
    // Set the future weather for all days in the game.
    this->generateDays(game.days);

    game.perfectLemonade = LemonadeRecipe();
    game.perfectLemonade.lemons = PERFECT_LEMONS;
    game.perfectLemonade.sugar = PERFECT_SUGAR;
    game.perfectLemonade.pricePerCup = PERFECT_PRICE;
}

///
/// \brief EconEngine::onNewDayRecipe Runs simulation when given a new lemonade recipe.
/// \param newLemonadeRecipe Recipe created for the day.
///
void EconEngine::onNewDayRecipe(LemonadeRecipe newLemonadeRecipe)
{
    // Set today's lemonade recipe from the provided recipe.
    game.today().lemonade = newLemonadeRecipe;

    // Runs the simulation using the new LemonadeStats provided.
    this->runSimulation();

    // Mark the day as completed.
    game.today().complete = true;

    // Increment the day.
    game.currentDate++;

    // Signals that a simuation has been completed, providing
    // the stats for the current day.
    emit this->sigSimulationComplete(game);

    return;
}

///
/// \brief EconEngine::onNewDayLemonade Public method for running simulation with given lemonade object.
/// \param newLemonade lemonade to run the simulation on.
///
void EconEngine::onNewDayLemonade(Lemonade newLemonade)
{
    // Convert Lemonade to a LemonadeStats struct.
    LemonadeRecipe stats(newLemonade);

    // Use onNewDayStats to give the converted LemonadeStats
    // to the simulator.
    this->onNewDayRecipe(stats);

    return;
}

///
/// \brief EconEngine::onUpgradePurchased When an upgrade is purahased update backend values.
/// \param upgradeId ID of upgrade that was purchased.
///
void EconEngine::onUpgradePurchased(int upgradeId)
{

    // Get the upgrade from the stand Upgrades.
    Upgrade* upgrade = game.stand.upgrades[upgradeId];

    // Deduct the cost of the upgrade from the player's wallet.
    game.stand.wallet -= upgrade->cost;

    // Designate that this upgrade has now been purchased.
    upgrade->purchased = true;

    // Execute the upgrade's effect.
    upgrade->effect(game);

    return;
}

///
/// \brief EconEngine::runSimulation method for running simulation.
///
void EconEngine::runSimulation()
{
    // Perfect lemonade: One ice cube per 5 degrees over 70 degrees.
    game.perfectLemonade.ice = (game.today().temperature - 70) / 5;
    if (game.perfectLemonade.ice < 0)
    {
        game.perfectLemonade.ice = 0;
    }

    // Calculates demand based on internal states.
    int cupsDemanded = this->calculateDemand();

    // Set the number of cups sold to the number demanded.
    int cupsMade = game.today().lemonade.pitchers * game.stand.cupsPerPitcher;
    int cupsSold = cupsDemanded;

    // If more cups were demanded than made,
    // cap the demand to number made and flag
    // that the player sold out.
    if (cupsDemanded > cupsMade)
    {
        cupsSold = cupsMade;
        game.today().soldOut = true;
    }

    // Set the status of today's cups sold, cups demanded, and income.
    game.today().sales = cupsSold;
    game.today().demanded = cupsDemanded;
    game.today().income = cupsSold * game.today().lemonade.pricePerCup;

    // Calculate the total cost of the lemonade, and the profit.
    game.today().cost   = totalCostOfLemonade();
    game.today().profit = calculateProfit(game.today().cost, game.today().income);

    // Add the profit (whether it be positive or negative) to the player's
    // wallet.
    game.stand.wallet += game.today().profit;

    this->balanceWeights();

    return;
}

///
/// \brief EconEngine::calculateProfit Helper method for calculating profit,
/// \param cost Cost of running the Lemonade stand for a day,
/// \param income Money made from running the lemonade stand on the same day.
/// \return Returns the profit made from a day.
///
float EconEngine::calculateProfit(float cost, float income)
{
    return income - cost;
}

///
/// \brief EconEngine::calculateDemand Helper method for calculating the demand of the stand's patrons.
/// \return Returns today's demand.
///
int EconEngine::calculateDemand()
{
    int result = 0; // Default have it max demand (without upgrades).

    int temp = game.today().temperature;

    // Switched based off of the temperature.
    int baseDemand = 0;
    switch(temp){
    case 55:
        baseDemand = 10;
        break;
    case 25:
        baseDemand = 5;
        break;
    case 65:
        baseDemand = 15;
        break;
    case 72:
        baseDemand = 20;
        break;
    }

    float priceMult = this->determinePriceWeight();

    result = baseDemand;
    int sumReputation = int(game.stand.reputation  * game.weights.reputation);

    result = (baseDemand
            + sumReputation)
            * priceMult;

    if (result > MAX_DEMAND)
    {
        result = MAX_DEMAND;
    }
    else if (result < MIN_DEMAND)
    {
        result = MIN_DEMAND;
    }

    qDebug() << baseDemand
             << "\t"
             << game.stand.reputation
             << "\t"
             << sumReputation
             << "\t"
             << priceMult
             << "\t"
             << result;

    return result;
}

///
/// \brief EconEngine::balanceWeights helper method for balancing the game.
///
void EconEngine::balanceWeights()
{
    game.stand.reputation += int(game.today().sales * REPUTATION_POINTS_PER_SALE);
    game.stand.reputation += determineRecipeBonus();
}

///
/// \brief EconEngine::determineRecipeBonus Method for calculating the recipe's bonus.
/// \return returns the calculated bonus.
///
int EconEngine::determineRecipeBonus()
{
    int result = BASE_RECIPE_BONUS;

    int lemonsDiff = 	game.today().lemonade.lemons -
                        game.perfectLemonade.lemons;

    int sugarDiff = 	game.today().lemonade.sugar -
                        game.perfectLemonade.sugar;

    int iceDiff = 		game.today().lemonade.ice -
                        game.perfectLemonade.ice;

    int squaredDiffSum = lemonsDiff * lemonsDiff +
                         sugarDiff * sugarDiff +
                         iceDiff * iceDiff;

    result -= squaredDiffSum;

    return result;
}

///
/// \brief EconEngine::determinePriceWeight Determines the relative weight of price compared to ideal price
/// \return Returns the calculated weight
///
float EconEngine::determinePriceWeight()
{
    float idealPrice = game.perfectLemonade.pricePerCup;

    float diffRatio = game.today().lemonade.pricePerCup / idealPrice;

    return 1 / (diffRatio);
}

///
/// \brief EconEngine::generateDays Generates 3 weeks worth of weather simulation/disasters.
/// \param Array of days
///
void EconEngine::generateDays(Day* days)
{
    for (int i = 0; i < 14; i++)
    {
        // Skip day if it has already been simulated.
        if (days[i].complete)
        {
            continue;
        }
        if(i == 9){ // Tornado disaster.
            game.days[9].disaster = 1;
        }
        else{ // No Disaster.
            game.days[i].disaster = 0;
        }

        // Set the first week to sunny. Otherwise, randomize weather.
        if (i < 5)
        {
            game.days[i].temperature = 72;
        }
        else
        {
            int random = 0 + ( std::rand() % ( 3 - 0 + 1 ) );
            game.days[i].weatherState = random;
            switch(random){
            case 0:
                // Rainy weather.
                days[i].temperature = 55;
                break;
            case 1:
                // Snowy weather.
                days[i].temperature = 25;
                break;
            case 2:
                // Cloudy weather.
                days[i].temperature = 65;
                break;
            case 3:
                // Sunny weather.
                days[i].temperature = 72;
                break;
            }
        }
    }
    // Generate day 14.
    // Whale Disaster.
    game.days[14].disaster = 3;
    return;
}

///
/// \brief EconEngine::totalCostOfLemonade Calculates the total cost of today's lemonade recipe.
/// \return Returns the cost.
///
float EconEngine::totalCostOfLemonade()
{
    float costOfLemons = game.today().lemonade.lemons * game.world.priceLemons();
    float costOfSugar  = game.today().lemonade.sugar * game.world.priceSugar();
    float costOfIce    = game.today().lemonade.ice * game.world.priceIce();

    float totalCostOfIngredients = costOfIce + costOfSugar + costOfLemons;

    // Calculate cost in relation to number of pitchers.
    totalCostOfIngredients = game.today().lemonade.pitchers * totalCostOfIngredients;
    return totalCostOfIngredients;
}
