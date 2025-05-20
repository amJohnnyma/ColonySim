#include "UIManager.h"

UIManager::UIManager(World* world)
{       

    
    UIElement* fpsc = new FPSCounter(world, 58, sf::Color::Red, sf::Vector2f{10.f, 10.f});
    UIElement* rec = new WorldUIElement(world, 0,2,4,5,100,50, "spawnEntity");

    FunctionArgs args = {
        "Orc",
        5
    };    
    rec->setArgs(args);

    UIElement* simControl = new WorldUIElement(world, 0,3,4,5,100,50, "toggleSimState");
    simControl->setColor(sf::Color::Red);
    FunctionArgs sArgs;
  //  std::cout << "simControl pointer: " << simControl << std::endl;
    sArgs.element = simControl;   
    simControl->setArgs(sArgs);
  
    
    addElement(fpsc);
    addElement(rec);   
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