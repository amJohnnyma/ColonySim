#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "Controller.h"

class PlayerController : public Controller
{
    private:
    public:
        PlayerController(World* world, sf::Color overlay = sf::Color(0,0,255,128));
        void update() override;
        void selectCells(sf::Vector2i from, sf::Vector2i to) override;

    private:

};

#endif