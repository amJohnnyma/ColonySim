#ifndef ENEMY_CONTROLLER_H
#define ENEMY_CONTROLLER_H

#include "Controller.h"

class EnemyController : public Controller
{
    private:
    public:
        EnemyController(World* world,sf::Color overlay = sf::Color(0,0,255,128));
        void update() override;
        void selectCells(sf::Vector2i from, sf::Vector2i to,std::vector<std::string> filter) override;
    private:
};

#endif