#ifndef GAME_H
#define GAME_H
#include "World.h"
#include "../../structs/gfx.h"
#include "../../structs/inputManager.h"

class Game
{
    private:
        window* wind;
        World* world;
        

    public:
        Game(int windowWidth, int windowHeight, int worldWidth, int worldHeight);
        ~Game();
        void run();

};

#endif