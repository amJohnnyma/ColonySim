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
            inputManager->processEvent(event,*wind->wndw);
            if (event.type == sf::Event::Closed)
                wind->wndw->close();
        }
        world->handleInput(*inputManager, *wind->wndw);
        inputManager->update(*wind->wndw);

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
        inputManager->draw(*wind->wndw); 
        wind->wndw->display();



    }

}

Game::Game(int windowWidth, int windowHeight, int worldWidth, int worldHeight)
{
    wind = new window(windowWidth,windowHeight);
    world = new World(worldWidth,worldHeight,*wind->wndw);
    uiMan = new UIManager(world);
    inputManager = new InputManager(world);    
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
    
    sf::Event event;   

    while (wind->wndw->isOpen())
    {
        inputManager->processEvent(event,*wind->wndw);
        while (wind->wndw->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                wind->wndw->close();
        }

        world->handleInput(*inputManager, *wind->wndw);
        inputManager->update(*wind->wndw);


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
        inputManager->draw(*wind->wndw); 
        wind->wndw->display();
    }
}
