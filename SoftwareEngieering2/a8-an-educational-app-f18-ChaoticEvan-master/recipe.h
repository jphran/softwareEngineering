#ifndef RECIPE_H
#define RECIPE_H

#include "lemonade.h"

/**
 * @brief The LemonadeStats struct contains statistics for one day's
 * 		  lemonade composition, such as ingredient amounts and price
 * 		  per cup.
 */
struct LemonadeRecipe
{
    /**
     * @brief The amount of sugar in one pitcher of lemonade.
     */
    int sugar	= 0;

    /**
     * @brief The amount of ice in one pitcher of lemonade.
     */
    int ice		= 0;

    /**
     * @brief The amount of lemons in one pitcher of lemonade.
     */
    int lemons	= 0;

    /**
     * @brief The price of one cup of lemonade.
     */
    float pricePerCup	= 1.00;

    /**
     * @brief The number of pitchers of lemonade the player chose to create.
     */
    int pitchers	= 0;

    /**
     * @brief LemonadeRecipe() Constructs a default LemonadeRecipe.
     */
    LemonadeRecipe(){}

    /**
     * @brief LemonadeStats(Lemonade) Converts a Lemonade object into a
     *        LemonadeStats struct.
     * @param newLemonade
     */
    LemonadeRecipe(Lemonade newLemonade){
        this->ice			= newLemonade.getIce();
        this->sugar			= newLemonade.getSugar();
        this->lemons		= newLemonade.getLemon();
        this->pricePerCup 	= newLemonade.getPricePerCup();
        this->pitchers		= newLemonade.getNumPitchers();
    }

};

#endif // RECIPE_H
