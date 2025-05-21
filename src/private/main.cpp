#include "game/headers/Game.h"
#include "game/headers/GlobalVars.h"

int main()
{
    Game game(conf::window_size.x,conf::window_size.y, conf::worldSize.x,conf::worldSize.y);
    game.run();
//    game.fixedrun(); //fixed run to help with memory leaks
}
