/**
  * This class stores the information for upgrades.
  * Created by Serena Aeschilman, Spencer Elkington, Andrew Stender, Evan Voordeckers, Ryan Williamson, Keegan Spencer, and Theaux Masqulier.
  */

#include "educationalprompter.h"
#include "gamestate.h"
#include "upgrades.h"

/*
 * PROCESS FOR CREATING UPGRADES:
 *
 * 1. Add a new enumerable to the UPGRADE_ENUM enumerable in gamestate.h
 * 2. Create a new entry below, using the enumerable ID you just created.
 * 		- Give the upgrade a name, description, cost.
 * 		- Provide the new upgrade its Enumeration ID.
 * 		- Create a new effect using a void(GameState&) lambda function.
 */
Upgrades::Upgrades()
{
        this->m_upgradeList[UPGRADE_ENUM::NEON_SIGN] = new Upgrade(
                    "Neon Sign", // Name.
                    "Throw a neon sign on that dingy stand to attract some new customers!", // Description.
                    50.00, // Cost
                    UPGRADE_ENUM::NEON_SIGN, // Enum ID.
                    [] (GameState& game) // Effect.
                    {
                        game.stand.marketing += 10;
                        EPrompt::displayEduPrompt(EPrompt::P_MARKETING);
                    });

        this->m_upgradeList[UPGRADE_ENUM::WHALE_UMBRELLA] = new Upgrade(
                    "Whale Umbrella",
                    "Life might give you a lot more than lemons. Be sure you can bounce whatever life gives you out of the sky!",
                    2000.00,
                    UPGRADE_ENUM::WHALE_UMBRELLA,
                    [] (GameState& game)
                    {
                        EPrompt::displayEduPrompt(EPrompt::P_INSURANCE);
                    });

        this->m_upgradeList[UPGRADE_ENUM::SUGAR_DEALER] = new Upgrade(
                    "Sugar Dealer",
                    "You call up an old friend from college who can get you some superb, high-quality sugar. He won't tell you where he gets them, and they cost an arm and a leg, but they're the bees knees!",
                    150.00,
                    UPGRADE_ENUM::SUGAR_DEALER,
                    [] (GameState& game)
                    {
                        game.stand.reputation += 200;
                        game.world.multiplierPriceSugar += 0.50;

                        EPrompt::displayEduPrompt(EPrompt::P_PRODUCT_DIFFERENTIATION);
                    });

        this->m_upgradeList[UPGRADE_ENUM::ORGANIC_LEMONS] = new Upgrade(
                    "Organic Lemons",
                    "You call up an old friend from college who can get you some superb, high-quality lemons. She won't tell you where she gets them, and they cost an arm and a leg, but they're the bees knees!",
                    150.00,
                    UPGRADE_ENUM::ORGANIC_LEMONS,
                    [] (GameState& game)
                    {
                        game.stand.reputation += 200;
                        game.world.multiplierPriceLemons += 0.50;

                        // Display prompt for Product Differentiation
                        EPrompt::displayEduPrompt(EPrompt::P_PRODUCT_DIFFERENTIATION);
                    });

        this->m_upgradeList[UPGRADE_ENUM::BOOM_BOX] = new Upgrade(
                    "Boom Box",
                    "Invest in the sound of music. Gather the kiddos to drink to the sound of some sick tunes.",
                    75.00,
                    UPGRADE_ENUM::BOOM_BOX,
                //TODO put in music
                    [](GameState& game)
                    {
                        game.stand.reputation += 50;
                        EPrompt::displayEduPrompt(EPrompt::P_MARKETING);
                    });      

        this->m_upgradeList[UPGRADE_ENUM::INSURANCE] = new Upgrade(
                    "Insurance",
                    "Want to ensure you don't have to rebuy your decked out lemonade stand? Invest in Insurance!",
                    250.00,
                    UPGRADE_ENUM::INSURANCE,
                    [] (GameState& game)
                    {
                        game.stand.reputation += 20;
                        game.world.multiplierPriceSugar += 0.50;
                        game.world.multiplierPriceLemons += 0.50;

                        EPrompt::displayEduPrompt(EPrompt::P_INSURANCE);
                    });

        this->m_upgradeList[UPGRADE_ENUM::BIG_PITCHER] = new Upgrade(
                    "Bigger Pitcher",
                    "Create more lemonade with fewer resources! Each pitcher will now create 12 cups!",
                    250.00,
                    UPGRADE_ENUM::BIG_PITCHER,
                    [] (GameState& game)
                    {
                        game.stand.cupsPerPitcher = 12;
                    });
}
