#ifndef GAMESTATE_H
#define GAMESTATE_H

#define DEFAULT_GAME_LENGTH 15
#define STARTING_PLAYER_MONEY 100.00

#include "lemonade.h"
#include "recipe.h"
#include <functional>

/**
 * A struct to store all the upgrades.
 */
struct Upgrade;

/**
 * @brief The UPGRADE_ENUM enum provides an enumeration of all
 * 		  purchasable upgrades.
 */
enum UPGRADE_ENUM
{
    NEON_SIGN,
    WHALE_UMBRELLA,
    SUGAR_DEALER,
    ORGANIC_LEMONS,
    BOOM_BOX,
    GRAPE,
    INSURANCE,
    BIG_PITCHER,

    // Maximum enum value. Keep at end of enum.
    // TECHNICALLY UNSAFE.
    NUM_UPGRADES
};

class Upgrades
{

private:
    /**
     * A list to store an enumerable of all of the upgrades.
     * @brief m_upgradeList
     */
    Upgrade* m_upgradeList[UPGRADE_ENUM::NUM_UPGRADES];

public:
    /**
     * A constructor for Upgrades.
     * @brief Upgrades
     */
    Upgrades();

    /**
     * A method to access upgrades at certain indices.
     * @brief operator []
     * @param i
     * @return
     */
    Upgrade*& operator[](int i) { return m_upgradeList[i]; }

};

/**
 * @brief The StandStats struct contains statistics for the current
 * 		  state of the player's stand, such as the stand's reputation.
 */
struct Stand
{
    /**
     * @brief Reputation points to represent the current reputation of
     * 		  the player's stand.
     */
    int reputation = 0;

    /**
     * @brief Marketing points to represent how much draw the player has
     * 		  to new customers.
     */
    int marketing	= 0;

    /**
     * @brief The number of cups the player can make out of one pitcher
     * 		  of lemonade.
     */
    int cupsPerPitcher = 8;

    /**
     * @brief The current amount of money in the player's wallet.
     */
    float wallet = STARTING_PLAYER_MONEY;

    /**
     * @brief the upgrades the player has bought for their stand.
     */
    Upgrades upgrades;
};

/**
 * @brief The DayStats struct contains the statistics for one day of
 * 		  a lemonade stand.
 */
struct Day
{
    /**
     * @brief Describes whether or not this day has been simulated.
     */
    bool  complete = false;

    /**
     * @brief Temperature of the day.
     */
    int   temperature = 70;

    /**
     * @brief Number of sales for this day. 0 prior to simulation.
     */
    int   sales	= 0;

    /**
     * @brief Number of units demanded for this day. 0 prior to simulation.
     */
    int   demanded = 0;

    /**
     * @brief Describes if the player sold out of lemonade that day.
     */
    bool  soldOut = false;

    /**
     * @brief Total cost of lemonade materials that day. 0 prior to simulation.
     */
    float cost	= 0;

    /**
     * @brief Total income from sales for that day. 0 prior to simulation.
     */
    float income = 0;

    /**
     * @brief Total profit for that day. 0 prior to simulation.
     */
    float profit = 0;

    /**
     * @brief Recipe for lemonade that day.
     */
    LemonadeRecipe lemonade;

    /**
     * @brief WeatherState Stores the weather for the day, the default is Sunny.
     */
    int weatherState = 3;

    /**
     * @brief Disaster Stores the disaster, by default the disaster is 0.
     */
    int disaster = 0;

};

/**
 * @brief The WorldStats struct contains statistics for the current
 * 		  state of the game world, such as weather.
 */
struct World
{
    /**
     * A variablet to store the severity of the weather.
     * @brief weatherSeverity
     */
    int weatherSeverity	= 0;

    /**
     * @brief BaseLemonPrice is the base price of a lemon unit.
     */
    float basePriceLemons = 0.50;

    /**
     * @brief BaseSugarPrice is the base price of a sugar unit.
     */
    float basePriceSugar = 0.40;

    /**
     * @brief BaseIcePrice is the base price of an ice unit.
     */
    float basePriceIce = 0.10;

    /**
     * @brief PriceLemonMultiplier is the multiplier for lemon prices.
     */
    float multiplierPriceLemons = 1.00;

    /**
     * @brief PriceSugarMultiplier is the multiplier for sugar prices.
     */
    float multiplierPriceSugar = 1.00;

    /**
     * @brief PriceIceMultiplier is the multiplier for ice prices.
     */
    float multiplierPriceIce = 1.00;

    /**
     * @brief Current price per unit of lemons.
     */
    float priceLemons()	{ return basePriceLemons * multiplierPriceLemons; }

    /**
     * @brief Current price per unit of sugar.
     */
    float priceSugar() { return basePriceSugar * multiplierPriceSugar; }

    /**
     * @brief Current price per unit of ice.
     */
    float priceIce() { return basePriceIce * multiplierPriceIce; }

    /**
     * @brief The weather enum Stores all the possible types of weather.
     */
    enum weather{
        RAINY,
        SNOWY,
        CLOUDY,
        SUNNY
    };

    /**
     * @brief The disasters enum Stores all the possible types of diaster.
     */
    enum disasters{
        NO_DISASTER,
        TORNADO,
        DUCK,
        WHALE
    };
};

/**
 * @brief The Weights struct includes all of the weights that may be
 * 		  used in the demand model.
 */
struct Weights
{
    /**
     * @brief Demand increase per reputation point.
     */
    float reputation = 0.1;

    /**
     * @brief Demand increase per marketing unit.
     */
    float marketing = 0;
};

/**
 * @brief The GameStats struct is the top-level struct for all game info
 * 		  and statistics.
 */
struct GameState
{
    /**
     * @brief Current in-game date, starting at 0.
     */
    int currentDate   = 0;

    /**
     * @brief Current in-game level, starting at 0 and increasing with
     *        every 5 days.
     */
    int currentLevel = this->currentDate / 5;

    /**
     * @brief GameLength represents the length of the game, in days.
     */
    int gameLength = DEFAULT_GAME_LENGTH;

    /**
     * @brief Statistics for the player's stand.
     */
    Stand stand;

    /**
     * @brief Statistics for the current state of the world.
     */
    World world;

    /**
     * @brief An array of all the stats for every in-game day.
     */
    Day days[DEFAULT_GAME_LENGTH];

    /**
     * @brief The currentDate's statistics.
     */
    Day& today()
    {
        return days[currentDate];
    }

    /**
     * A method ot retrieve the datay for the previous day.
     * @brief yesterday
     * @return
     */
    Day& yesterday()
    {
        return (currentDate > 0) ? days[currentDate - 1] : days[currentDate];
    }

    /**
     * @brief The current day's recipe for lemonade.
     */
    LemonadeRecipe& currentLemonade() { return this->today().lemonade; }

    /**
     * @brief The ideal recipe for lemonade.
     */
    LemonadeRecipe perfectLemonade;

    /**
     * @brief The weights used in the demand calculations during the simulation.
     */
    Weights weights;
};

#endif // GAMESTATE_H
