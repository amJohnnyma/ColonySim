#include "UIManager.h"

UIManager::UIManager()
{       

    FPSCounter* fpsc = new FPSCounter(58, sf::Color::Red, sf::Vector2f{10.f, 10.f});
    Rect* rec = new Rect(0,2,4,5,100,50);
    addElement(fpsc);
    addElement(rec);   


}

UIManager::~UIManager()
{
    for (auto& elem : elements)
        delete elem;
}

void UIManager::update()
{
    for (auto& elem : elements)
        elem->update();
    
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