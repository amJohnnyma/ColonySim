#include "game/headers/Game.h"
#include "game/headers/GlobalVars.h"
#include <filesystem>

//temp include
#include "utils/headers/TextureManager.h"
int main()
{
    Game* game = new Game(conf::window_size.x,conf::window_size.y, conf::worldSize.x,conf::worldSize.y);
    game->run();
    delete game;
//    game.fixedrun(); //fixed run to help with memory leaks


    return 0;
}
