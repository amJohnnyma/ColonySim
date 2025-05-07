#include "../headers/Game.h"

void Game::run()
{
    while(wind->wndw->isOpen())
    {

        sf::Event event;
        while(wind->wndw->pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                wind->wndw->close();
            }
        }
        //inputManager->update(event);
        world->handleInput(*wind->wndw);
        world->update();
        world->render(*wind->wndw);

    }
}

Game::Game(int windowWidth, int windowHeight, int worldWidth, int worldHeight)
{
    wind = new window(windowWidth,windowHeight);
    world = new World(worldWidth,worldHeight,*wind->wndw);
    inputManager = new inputs();
}

Game::~Game()
{
    delete wind;
    delete world;
}