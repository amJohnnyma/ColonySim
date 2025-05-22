#include "UIManager.h"

UIManager::UIManager(World* world)
{       

    
    UIElement* fpsc = new FPSCounter(world, 12, sf::Color::Red, sf::Vector2f{conf::window_size.x - (conf::window_size.x * 0.1), (conf::window_size.y * 0.02)});
    UIElement* stats = new WorldUIElement(world, 0,0,4,5,conf::cellSize*4,conf::cellSize*6, "updateWorldStats", "Stats:");
    UIElement* simControl = new WorldUIElement(world, 0,6,4,5,conf::cellSize,conf::cellSize, "toggleSimState", "S");
    UIElement* incPher = new WorldUIElement(world, 1,7,4,5,conf::cellSize,conf::cellSize, "incrementPheremone", "+p");
    UIElement* decPher = new WorldUIElement(world, 0,7,4,5,conf::cellSize,conf::cellSize, "decrementPheremone", "-p");
    UIElement* incHeur = new WorldUIElement(world, 1,8,4,5,conf::cellSize,conf::cellSize, "incrementHeuristic", "+h");
    UIElement* decHeur = new WorldUIElement(world, 0,8,4,5,conf::cellSize,conf::cellSize, "decrementHeuristic", "-h");

    FunctionArgs args;
    args.element = stats;
    stats->setArgs(args);

    simControl->setColor(sf::Color::Red);
    FunctionArgs sArgs;
    sArgs.element = simControl;   
    simControl->setArgs(sArgs);
    simControl->setFontSize(40);

    incPher->setColor(sf::Color::Green);
    decPher->setColor(sf::Color::Red);
    incHeur->setColor(sf::Color::Green);
    decHeur->setColor(sf::Color::Red);
    incPher->setFontSize(40);
    decPher->setFontSize(40);
    incHeur->setFontSize(40);
    decHeur->setFontSize(40);

  
    
    addElement(fpsc);
    addElement(stats);   
    addElement(simControl);
    addElement(incPher);
    addElement(decPher);
    addElement(incHeur);
    addElement(decHeur);


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
        elem->update(window);
    
}

void UIManager::draw(sf::RenderWindow& window)
{
    for (auto& elem : elements) {
        if (!elem) {
            std::cerr << "Null element in UIManager::elements!\n";
            continue;
        }
        elem->draw(window);
    }
}