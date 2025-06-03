#include "../headers/Game.h"

Game* Game::instance = nullptr;

void Game::run()
{
    using clock = std::chrono::steady_clock;
    auto lastUpdate = clock::now();
    const std::chrono::milliseconds updateInterval(50); 
    while(wind->wndw->isOpen())
    {

        switch (currentState)
        {
            case State::IDLE:
                idleState();  
                break;          
            case State::PAUSED:
                pausedState();
                break;
            case State::RUNNING:
                runningState();
                break;
            case State::STOPPED:
                stoppedState();
                break;
            default:
                std::cerr << "Unkown state" << std::endl;
                wind->wndw->close();
                break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Prevents CPU spinning
}


}

void Game::renderFrame() {
    world->render(*wind->wndw);   
    uiMan->draw(*wind->wndw);   
    inputManager->draw(*wind->wndw); 
    wind->wndw->display();
}

void Game::idleState()
{
    sf::Event event;
    while (wind->wndw->pollEvent(event))
    {
        inputManager->processEvent(event, *wind->wndw);
        if (event.type == sf::Event::Closed)
            wind->wndw->close();
    }
    world->handleInput(*inputManager, *wind->wndw);
    inputManager->update(*wind->wndw);
    uiMan->update(*wind->wndw,event);
    renderFrame();
}

void Game::runningState()
{
    using clock = std::chrono::steady_clock;
    static auto lastUpdate = clock::now();
    const std::chrono::milliseconds updateInterval(50);

    sf::Event event;
    while (wind->wndw->pollEvent(event))
    {
        inputManager->processEvent(event, *wind->wndw);
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
    }

    uiMan->update(*wind->wndw, event);
    renderFrame();
    
}

void Game::pausedState()
{
    sf::Event event;
    while (wind->wndw->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            wind->wndw->close();
        inputManager->update(*wind->wndw);
        uiMan->update(*wind->wndw, event);
    }
    renderFrame();
}

void Game::stoppedState()
{
    wind->wndw->close();
}

void Game::handleEvent(Event event) {
    static const std::map<std::pair<State, Event>, State> transitions = {
        {{State::IDLE, Event::START}, State::RUNNING},
        {{State::IDLE, Event::PAUSE}, State::PAUSED},
        {{State::IDLE, Event::STOP}, State::STOPPED},
        {{State::IDLE, Event::UNPAUSE}, State::IDLE},

        {{State::RUNNING, Event::PAUSE}, State::PAUSED},
        {{State::RUNNING, Event::STOP}, State::STOPPED},
        {{State::RUNNING, Event::UNPAUSE}, State::RUNNING},

        {{State::PAUSED, Event::UNPAUSE}, State::IDLE},
        {{State::PAUSED, Event::START}, State::IDLE},
        {{State::PAUSED, Event::STOP}, State::STOPPED},

        // STOPPED state doesn't change
    };

    auto it = transitions.find({currentState, event});
    if (it != transitions.end()) {
        currentState = it->second;
        if(currentState == State::STOPPED)
        {
            std::cerr << "Stopped state" << std::endl;
        }
    } else {
        std::cerr << "Invalid event/state combo\n";
    }
    uiMan->setVisibilityForState(currentState);
}

Game::Game(int windowWidth, int windowHeight, int worldWidth, int worldHeight)
{
    wind = new window(windowWidth,windowHeight);
    world = new World(worldWidth,worldHeight,*wind->wndw);
    uiMan = new UIManager(world);
    inputManager = new InputManager(world);    
    handleEvent(Event::PAUSE);    
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

        uiMan->update(*wind->wndw, event);
        world->render(*wind->wndw);
        uiMan->draw(*wind->wndw);
        inputManager->draw(*wind->wndw); 
        wind->wndw->display();
    }
}
