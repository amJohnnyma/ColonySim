#ifndef GAME_H
#define GAME_H
#include "World.h"
#include "../../structs/gfx.h"
#include "../../structs/inputManager.h"
#include "../../UI/UIManager.h"
#include "../../controller/InputManager.h"
#include "GlobalVars.h"

#include <thread>
#include <chrono>

class Game
{
    private:
        World* world;
        window* wind;        
        InputManager* inputManager; //How am i going to use this in other classes? -> great difficulty
        UIManager* uiMan;
        

    public:
        Game(int windowWidth, int windowHeight, int worldWidth, int worldHeight);
        ~Game();
        void fixedrun();
        void run();
};

#endif