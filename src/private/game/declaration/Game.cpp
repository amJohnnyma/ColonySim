#include "../headers/Game.h"
#include "Game.h"


void Game::run()
{
    using clock = std::chrono::steady_clock;
    auto lastUpdate = clock::now();
    const std::chrono::milliseconds updateInterval(50); 

    while (wind->wndw->isOpen())
    {
        sf::Event event;
        while (wind->wndw->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                wind->wndw->close();
        }

        // Always run input and render
        world->handleInput(*wind->wndw, event);

        // Only update every x ms
        auto now = clock::now();
        if (now - lastUpdate >= updateInterval && world->isRunning())
        {
            world->update();
            lastUpdate = now;

        }

        uiMan->update(*wind->wndw);
        world->render(*wind->wndw);   
        uiMan->draw(*wind->wndw);    
        wind->wndw->display();



    }

}

Game::Game(int windowWidth, int windowHeight, int worldWidth, int worldHeight)
{
    wind = new window(windowWidth,windowHeight);
    world = new World(worldWidth,worldHeight,*wind->wndw);
    inputManager = new inputs();    
    uiMan = new UIManager(world);
}

Game::~Game()
{   
    delete world;
    wind->wndw->close();   
    delete wind;
    delete inputManager;
    delete uiMan;
}

void Game::fixedrun()
{
    using clock = std::chrono::steady_clock;
    auto lastUpdate = clock::now();
    const std::chrono::milliseconds updateInterval(50);

    int updatesDone = 0;
    const int maxUpdates = 1;

    while (wind->wndw->isOpen())
    {
        sf::Event event;
        while (wind->wndw->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                wind->wndw->close();
        }

        world->handleInput(*wind->wndw, event);

        auto now = clock::now();
        if (now - lastUpdate >= updateInterval && world->isRunning())
        {
            world->update();
            lastUpdate = now;

            updatesDone++;
            if (updatesDone >= maxUpdates)
            {
                // Close window to exit loop
                wind->wndw->close();
            }
        }

        uiMan->update(*wind->wndw);
        world->render(*wind->wndw);
        uiMan->draw(*wind->wndw);
        wind->wndw->display();
    }
}
