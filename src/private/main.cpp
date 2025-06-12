#include "game/Game.h"
#include "game/GlobalVars.h"
#include <filesystem>

int main()
{
    Game::init(conf::window_size.x,conf::window_size.y, conf::worldSize.x,conf::worldSize.y);
    Game& game = Game::getInstance();
    game.run();
    //game.fixedrun();
    Game::destroy();
//    game.fixedrun(); //fixed run to help with memory leaks


    return 0;
}
