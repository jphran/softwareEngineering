/**
  * This class stores the information behind the lemonade object.
  * Created by Serena Aeschilman, Spencer Elkington, Andrew Stender, Evan Voordeckers, Ryan Williamson,Keegan Spencer, and Theaux Masqulier.
  */

#include "lemonade.h"
#include "recipe.h"

/// Basic constructor to build a lemonade object with the input values of lemons, sugar, and ice.
/// Sets price per cup and number of pitchers to 0 and 1 respectively by default.
/// \brief Lemonade::Lemonade
/// \param lemons
/// \param sugar
/// \param ice
///
Lemonade::Lemonade(int lemons,int sugar,int ice, double price, int pitchers)
{
    this->lemon = lemons;
    this->sugar = sugar;
    this->ice = ice;
    this->pricePerCup = price;
    this->numPitchers = pitchers;
}

/// Creates a lemonade with 0 in all params
/// \brief Lemonade::Lemonade
///
Lemonade::Lemonade()
{
    reset();
}

/// Creates a lemonade object with the same values as the one that's being copied.
/// \brief Lemonade::Lemonade
/// \param lemonade
///
Lemonade::Lemonade(Lemonade *lemonade)
{
    this->lemon = lemonade->lemon;
    this->ice = lemonade->ice;
    this->sugar = lemonade->sugar;
    this->pricePerCup = lemonade->pricePerCup;
    this->numPitchers = lemonade->numPitchers;
}

/// Sets the recipe for the current lemonade
/// \brief Lemonade::setRecipe
/// \param lemons
/// \param sugar
/// \param ice
/// \param price
/// \param pitchers
///
void Lemonade::setRecipe(int lemons, int sugar, int ice, double price, int pitchers)
{
    this->lemon = lemons;
    this->sugar = sugar;
    this->ice = ice;
    this->pricePerCup = price;
    this->numPitchers = pitchers;
}

/// Sets the lemonade recipe from a LemonadeRecipe
/// \brief Lemonade::setRecipe
/// \param recipe
///
void Lemonade::setRecipe(const LemonadeRecipe recipe)
{
    this->lemon = recipe.lemons;
    this->sugar = recipe.sugar;
    this->ice = recipe.ice;
    this->pricePerCup = recipe.pricePerCup;
    this->numPitchers = recipe.pitchers;
}

/// Sets the price per cup of the current lemonade.
/// \brief Lemonade::setPricePerCup
/// \param pricePerCup
///
void Lemonade::setPricePerCup(double pricePerCup)
{
    this->pricePerCup = pricePerCup;
}

/// Sets the value of the lemon ingredient for the current lemonade.
/// \brief Lemonade::setLemon
/// \param lemon
///
void Lemonade::setLemon(int lemon)
{
    this->lemon = lemon;
}

/// Sets the value of the sugar ingredient of the current lemonade.
/// \brief Lemonade::setSugar
/// \param sugar
///
void Lemonade::setSugar(int sugar)
{
    this->sugar = sugar;
}

/// Sets the value of the ice ingredient of the current lemonade.
/// \brief Lemonade::setIce
/// \param ice
///
void Lemonade::setIce(int ice)
{
    this->ice = ice;
}

/// Sets the number of pitchers for the current lemonade.
/// \brief Lemonade::setNumPitchers
/// \param numPitchers
///
void Lemonade::setNumPitchers(int numPitchers)
{
    this->numPitchers = numPitchers;
}

/// Increments the lemon ingredient in the current lemonade.
/// \brief Lemonade::addOneLemon
///
void Lemonade::addOneLemon()
{
    ++lemon;
}

/// Increments the sugar ingredient in the current lemonade.
/// \brief Lemonade::addOneSugar
///
void Lemonade::addOneSugar()
{
    ++sugar;
}

/// Increments the ice ingredient in the current lemonade.
/// \brief Lemonade::addOneIce
///
void Lemonade::addOneIce()
{
    ++ice;
}

/// Increments the number of pitchers in the current lemonade.
/// \brief Lemonade::addOnePitcher
///
void Lemonade::addOnePitcher()
{
    ++numPitchers;
}

/// Sets the recipe of the current lemonade to all 0's.
/// \brief Lemonade::reset
///
void Lemonade::reset()
{
    this->setRecipe(0,0,0,0,0);
}
