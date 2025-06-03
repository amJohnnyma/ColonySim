#include "game/headers/Game.h"
#include "game/headers/GlobalVars.h"
#include <filesystem>

//temp include
#include "utils/headers/TextureManager.h"
int main()
{
    Game::init(conf::window_size.x,conf::window_size.y, conf::worldSize.x,conf::worldSize.y);
    Game& game = Game::getInstance();
    game.run();
    Game::destroy();
//    game.fixedrun(); //fixed run to help with memory leaks


    return 0;
}
