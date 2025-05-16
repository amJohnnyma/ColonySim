#include "UIManager.h"

UIManager::UIManager(World* world)
{       

    UIElement* fpsc = new FPSCounter(world, 58, sf::Color::Red, sf::Vector2f{10.f, 10.f});
    UIElement* rec = new Rect(world, 0,2,4,5,100,50);
    addElement(fpsc);
    addElement(rec);   


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