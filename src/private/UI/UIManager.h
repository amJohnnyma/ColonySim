#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include "UIElement.h"
#include "FPSCounter.h"
#include "Rect.h"
#include <vector>
#include <memory>
class UIManager
{
    private:
            std::vector<UIElement*> elements;
    public:
        UIManager();
        ~UIManager();
        void draw(sf::RenderWindow& window);
        void update();
        void addElement(UIElement* elem) {
            elements.push_back(elem);
    }

    /*
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        for (auto& elem : elements)
            elem->handleEvent(event, window);
    }*/

};

#endif