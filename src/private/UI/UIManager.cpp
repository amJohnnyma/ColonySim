#include "UIManager.h"
#include "WorldUIElement.h"
#include "FPSCounter.h"
#include "TextBoxSubmit.h"
#include <vector>
#include <memory>
UIManager::UIManager(World* world)
{       
    width = conf::window_size.x;
    height = conf::window_size.y;
    midScreen = {
        (conf::window_size.x / conf::cellSize) / 2,
        (conf::window_size.y / conf::cellSize) / 2
    };
    
    UIElement* fpsc = new FPSCounter(world, static_cast<int>(conf::window_size.x / conf::cellSize) - 10, sf::Color::Red, sf::Vector2f{conf::window_size.x - (conf::window_size.x * 0.1), (conf::window_size.y * 0.02)});
    UIElement* stats = new WorldUIElement(world, 0,0,4,5,conf::cellSize*4,conf::cellSize*6, "updateWorldStats", "Stats:");
    UIElement* simControl = new WorldUIElement(world, 0,6,4,5,conf::cellSize,conf::cellSize, "toggleSimState", "S");
    UIElement* incPher = new WorldUIElement(world, 1,7,4,5,conf::cellSize,conf::cellSize, "incrementPheremone", "+p");
    UIElement* decPher = new WorldUIElement(world, 0,7,4,5,conf::cellSize,conf::cellSize, "decrementPheremone", "-p");
    UIElement* incHeur = new WorldUIElement(world, 1,8,4,5,conf::cellSize,conf::cellSize, "incrementHeuristic", "+h");
    UIElement* decHeur = new WorldUIElement(world, 0,8,4,5,conf::cellSize,conf::cellSize, "decrementHeuristic", "-h");
    UIElement* buildMenu = new WorldUIElement(world, 0, 9, 4,5 , conf::cellSize, conf::cellSize, "openBuildMenu", "Build");
    UIElement* deleteBuilding = new WorldUIElement(world, 1, 9, 4,5 , conf::cellSize, conf::cellSize, "deleteBuilding", "Destroy");
    UIElement* pauseButton = new WorldUIElement
    (
        world, //world
        static_cast<int>(conf::window_size.x / conf::cellSize) - 1, //x
        0, //y
        4, //resolution
        5, //radius (size)
        conf::cellSize, //width
        conf::cellSize, //height
        "pauseGame",  //function
        "Pause" //displayed text
    );
    UIElement* pauseMenu = new WorldUIElement
    (
        world,
        (2),
        (1),
        4,
        5,
        (conf::window_size.x - conf::cellSize*4),
        (conf::window_size.y - conf::cellSize*4),
        "null", 
        ""
    );
    UIElement* numAntsInput = new TextBoxSubmit
    (
        world,
        (2),
        (1),
        4,
        5,
        conf::cellSize*2,
        conf::cellSize,
        "null", 
        "ants",
        "getNumAnts" //purpose
    );

    FunctionArgs args;
    args.element = stats;
    stats->setArgs(args);

    simControl->setColor(sf::Color::Red);
    FunctionArgs sArgs;
    sArgs.element = simControl;   
    simControl->setArgs(sArgs);

    FunctionArgs buildMenuArgs;
    buildMenuArgs.name = "DefaultNonBlocking"; //this could be a bit string    
    buildMenuArgs.element = buildMenu;
    buildMenu->setArgs(buildMenuArgs);

    FunctionArgs deleteBuildArgs;
    deleteBuildArgs.name = "DefaultNonBlocking"; //this could be a bit string    
    deleteBuildArgs.element = buildMenu;
    deleteBuilding->setArgs(deleteBuildArgs);

    incPher->setColor(sf::Color::Green);
    decPher->setColor(sf::Color::Red);
    incHeur->setColor(sf::Color::Green);
    decHeur->setColor(sf::Color::Red);
    buildMenu->setColor(sf::Color::Green);
    deleteBuilding->setColor(sf::Color::Red);
    incPher->setFontSize(40);
    decPher->setFontSize(40);
    incHeur->setFontSize(40);
    decHeur->setFontSize(40);
    simControl->setFontSize(40);
    buildMenu->setFontSize(10);
    deleteBuilding->setFontSize(10);
    

  
    //DEFAULT PRIO = 10 -> very unimportant
    addElement(fpsc); //0
    addElement(stats);   //1
    addElement(simControl); //2
    addElement(incPher); //3
    addElement(decPher); //4
    addElement(incHeur); //5
    addElement(decHeur); //6
    addElement(buildMenu); //7
    addElement(deleteBuilding); //8
    addElement(pauseButton); //9
    pauseButton->setPriority(9);    
    addElement(pauseMenu); //10 
    pauseMenu->setVisible(false);
    addElement(numAntsInput); //11
    numAntsInput->setPriority(9);

}

UIManager::~UIManager()
{
    //IF THIS IS HERE YOU DIDNT RUN VALGRIND TO ENSURE NO LEAKS
    for (auto& elem : elements)        
        delete elem;
}

void UIManager::update(sf::RenderWindow& window)
{

    for (auto& elem : elements)
        if(elem->isVisible())
            elem->update(window);
    
}

void UIManager::draw(sf::RenderWindow& window)
{
    for(int i = 10; i >= 0; i--)
    {
        for (auto& elem : elements) {
            if (!elem) {
                std::cerr << "Null element in UIManager::elements!\n";
                continue;
            }
            if(elem->isVisible() && elem->getPrio()==i)
                elem->draw(window);
        }

    }
}

void UIManager::setVisibilityForState(State gameState)
{
    for (auto& elem : elements)
        elem->setVisible(false); // Hide everything by default

    int BASEUI = 9;
    int PauseMenu = 15;
    switch (gameState){
    case State::RUNNING: {
        int num = 0;        
        for(auto& elem : elements)
        {
            if(num <= BASEUI)
                elem->setVisible(true);
            else
                elem->setVisible(false);
            num++;
        }
        elements[9]->move(static_cast<int>(conf::window_size.x / conf::cellSize) - 1, 0);
        elements[9]->setVisible(true);
        break;
    }

    case State::PAUSED: {
        int num = 0;
        for(auto& elem : elements)
        {
            if(num <= BASEUI)
                elem->setVisible(false);
            else
                elem->setVisible(true);
            num++;
        }
        elements[9]->move(midScreen.first, midScreen.second);
        elements[9]->setVisible(true);
        break;
    }

    case State::IDLE: {
        int num = 0;
        for(auto& elem : elements)
        {
            if(num <= BASEUI)
                elem->setVisible(true);
            else
                elem->setVisible(false);
            num++;
        }
        elements[9]->move(static_cast<int>(conf::window_size.x / conf::cellSize) - 1, 0);
        elements[9]->setVisible(true);
        break;
    }

    case State::STOPPED: {
        int num = 0;
        for(auto& elem : elements)
        {
            if(num <= BASEUI)
                elem->setVisible(false);
            else
                elem->setVisible(true);
            num++;
        }
        break;
    }
}

}
