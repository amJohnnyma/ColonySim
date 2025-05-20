#include "UIManager.h"

UIManager::UIManager(World* world)
{       

    
    UIElement* fpsc = new FPSCounter(world, 12, sf::Color::Red, sf::Vector2f{conf::window_size.x - (conf::window_size.x * 0.1), (conf::window_size.y * 0.02)});
    UIElement* stats = new WorldUIElement(world, 0,0,4,5,200,300, "testClick", "Stats");

    FunctionArgs args;
    args.element = stats;
    stats->setArgs(args);

    UIElement* simControl = new WorldUIElement(world, 0,6,4,5,70,70, "toggleSimState", "Start");
    simControl->setColor(sf::Color::Red);
    FunctionArgs sArgs;
  //  std::cout << "simControl pointer: " << simControl << std::endl;
    sArgs.element = simControl;   
    simControl->setArgs(sArgs);
  
    
    addElement(fpsc);
    addElement(stats);   
    addElement(simControl);


}

UIManager::~UIManager()
{
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