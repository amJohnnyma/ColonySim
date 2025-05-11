#include "../headers/Game.h"

void Game::run()
{
    using clock = std::chrono::steady_clock;
    auto lastUpdate = clock::now();
    const std::chrono::milliseconds updateInterval(500);  // 500ms
    ////////////////
    sf::Clock fpsClock;
    sf::Font font;
    if (!font.loadFromFile("src/fonts/pixel.ttf")) {
        std::cerr << "Could not load font\n";
        return;
    }

    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(58);
    fpsText.setFillColor(sf::Color::Red);
    fpsText.setPosition(10.f, 10.f);

    int frameCount = 0;
    float elapsedTime = 0.f; 
//////////////////////

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
        if (now - lastUpdate >= updateInterval && world->isRunning())
        {
            world->update();
            lastUpdate = now;
        }
        world->render(*wind->wndw);


    /////////////////
     elapsedTime += fpsClock.restart().asSeconds();
     frameCount++;
     if (elapsedTime >= 1.0f) {
         float fps = frameCount / elapsedTime;
         fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
       //  std::cout << "FPS: " << fps << std::endl;
         frameCount = 0;
         elapsedTime = 0.f;
     }
     sf::View originalView = wind->wndw->getView();
     wind->wndw->setView(wind->wndw->getDefaultView());
     wind->wndw->draw(fpsText);
     wind->wndw->setView(originalView);
     wind->wndw->display();
     ////////////////////


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