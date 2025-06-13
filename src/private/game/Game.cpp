#include "Game.h"

Game* Game::instance = nullptr;

void Game::run()
{
    using clock = std::chrono::steady_clock;
    constexpr std::chrono::duration<double, std::milli> targetFrameTime(16.667); // ~60 FPS = 16.667

    while (wind->wndw->isOpen())
    {
        auto frameStart = clock::now();

        // Handle game state
        switch (currentState)
        {
            case State::IDLE:
            //std::cout << "Idle" << std::endl;
                idleState();  
                break;          
            case State::PAUSED:
            //std::cout << "paused" << std::endl;
                pausedState();
                break;
            case State::RUNNING:
           // std::cout << "running" << std::endl;
                runningState();
                break;
            case State::STOPPED:
          //  std::cout << "stopped" << std::endl;
                stoppedState();
                break;
            default:
                std::cerr << "Unknown state" << std::endl;
                wind->wndw->close();
                break;
        }

        // Calculate how long frame took
        auto frameEnd = clock::now();
        auto elapsed = frameEnd - frameStart;

        // Sleep to limit frame rate, if frame finished early
        if (elapsed < targetFrameTime)
        {
            std::this_thread::sleep_for(targetFrameTime - elapsed);
        }
             //   std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Prevents CPU spinning
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
    const std::chrono::milliseconds updateInterval(0);
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
    world = new World(*wind->wndw);
    uiMan = new UIManager(world, *wind->wndw);
    inputManager = new InputManager(world,*wind->wndw);    
    handleEvent(Event::UNPAUSE);   
    handleEvent(Event::START);   
    world->Init(); 
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
    currentState = State::RUNNING;
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
