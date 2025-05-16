#include "../headers/Game.h"
#include "../../UI/FPSCounter.h"

void Game::run()
{
    using clock = std::chrono::steady_clock;
    auto lastUpdate = clock::now();
    const std::chrono::milliseconds updateInterval(50); 
    sf::Font font;
    if (!font.loadFromFile("src/fonts/pixel.ttf")) {
        std::cerr << "Could not load font\n";
        return;
    }

    FPSCounter fpsCounter(font, 58, sf::Color::Red, {10.0f, 10.0f});

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

        // Only update every 500ms
        auto now = clock::now();
        if (now - lastUpdate >= updateInterval && world->isRunning())
        {
            world->update();
            lastUpdate = now;

        }

        world->render(*wind->wndw);
        fpsCounter.update();
        fpsCounter.draw(*wind->wndw);

        wind->wndw->display();



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