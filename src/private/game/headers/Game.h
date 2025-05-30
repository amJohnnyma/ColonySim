#ifndef GAME_H
#define GAME_H
#include "World.h"
#include "../../structs/gfx.h"
#include "../../UI/UIManager.h"
#include "../../controller/InputManager.h"
#include "GlobalVars.h"

#include <thread>
#include <chrono>
enum State 
{
    IDLE,
    RUNNING,
    PAUSED,
    STOPPED
};
enum Event
{
    START,
    STOP,
    PAUSE,
    UNPAUSE
};

class Game
{
    private:
        World* world;
        window* wind;        
        UIManager* uiMan;
        InputManager* inputManager; //How am i going to use this in other classes? -> great difficulty
        State currentState = State::RUNNING;

    public:
        Game(int windowWidth, int windowHeight, int worldWidth, int worldHeight);
        ~Game();
        void fixedrun();
        void run();
        void renderFrame();
        void idleState();
        void runningState();
        void pausedState();
        void stoppedState();
        void handleEvent(Event event);
};

#endif