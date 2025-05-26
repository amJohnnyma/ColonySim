#ifndef ENEMY_CONTROLLER_H
#define ENEMY_CONTROLLER_H

#include "Controller.h"

class EnemyController : public Controller
{
    private:
    public:
        EnemyController(World* world);
        void update() override;
    private:
};

#endif