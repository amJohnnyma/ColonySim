#include "../headers/Game.h"

void Game::run()
{
    using clock = std::chrono::steady_clock;
    auto lastUpdate = clock::now();
    const std::chrono::milliseconds updateInterval(50);  // 500ms

    while (wind->wndw->isOpen())
    {
        sf::Event event;
        while (wind->wndw->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                wind->wndw->close();
        }

        // Always run input and render
        world->handleInput(*wind->wndw);

        // Only update every 500ms
        auto now = clock::now();
        if (now - lastUpdate >= updateInterval)
        {
            world->update();
            lastUpdate = now;
        }

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
    delete world;
    wind->wndw->close();   
    delete wind;
}