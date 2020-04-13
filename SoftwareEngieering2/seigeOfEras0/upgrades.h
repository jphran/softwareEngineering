#ifndef UPGRADE_H
#define UPGRADE_H

#include "gamestate.h"
#include <functional>
#include <QString>

struct Upgrade
{
    /**
     * @brief Name of the upgrade.
     */
    QString name;

    /**
     * @brief Fun description about the upgrade.
     */
    QString description = "";

    /**
     * @brief Cost of the upgrade, in dollars.
     */
    float cost;

    /**
     * @brief Enumerable id of the upgrade, as listed in UPGRADE_ENUM.
     */
    int id;

    /**
     * @brief Purchased status of the uprade.
     */
    bool purchased = false;

    /**
     * @brief An effect function, stored as a variable, to run when the Upgrade's
     * 		  effect is applied.
     * @note  Remember delegates from CS3500? This is one of those, but in C++!
     *
     * 		  Hurray!
     *
     * 		  What you'll need to do is create a lambda function that takes the
     * 		  signature "void method(GameState&)". This function should go in,
     * 	      mess around with the GameState (such as adjusting weights, variables,
     * 		  prices, etc.) as necessary.
     *
     * 		  Read the C++ documentation on how to create lambda functions, as that
     * 		  is the most obvious way to create this. However, a brief synopsis below:
     *
     * 		  LAMBDA FUNCTION:
     *
     * 		  	  [] (GameState game) -> void
     * 			  {
     * 			      ***FUNCTION STUFF HERE ***
     * 			  }
     *
     * 		  Ask Spencer if you have any questions :)
     */
     std::function<void(GameState&)> effect;

    /**
     * @brief Upgrade will contain all the information required for an upgrade
     * 		  the user can make to their stand.
     * @param _name, The name of the upgrade.
     * @param _cost, The cost of the upgrade.
     * @param _id,   The enum ID of the upgrade in UPGRADE_ENUM.
     * @param _effect, The effect of the upgrade. READ UPGRADE::EFFECT NOTE
     */
    Upgrade(QString _name, float _cost, int _id, std::function<void(GameState&)> _effect):
        name(_name),
        cost(_cost),
        id(_id),
        effect(_effect)
    {}

    /**
     * @brief Upgrade will contain all the information required for an upgrade
     * 		  the user can make to their stand.
     * @param _name, The name of the upgrade.
     * @param _description, the description of the upgrade.
     * @param _cost, The cost of the upgrade.
     * @param _id, The enum ID of the upgrade in UPGRADE_ENUM.
     * @param _effect, The effect of the upgrade. READ UPGRADE::EFFECT NOTE
     */
    Upgrade(QString _name, QString _description, float _cost, int _id, std::function<void(GameState&)> _effect):
        name(_name),
        description(_description),
        cost(_cost),
        id(_id),
        effect(_effect)
    {}

};

#endif // UPGRADE_H
