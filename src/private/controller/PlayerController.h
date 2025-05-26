#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "Controller.h"

class PlayerController : public Controller
{
    private:
    public:
        PlayerController(World* world);
        void update() override;
    private:
};

#endif