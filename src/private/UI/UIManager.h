#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include "UIElement.h"
#include "FPSCounter.h"
#include "WorldUIElement.h"
#include <vector>
#include <memory>



class UIManager
{
    private:
            std::vector<UIElement*> elements;
    public:
        UIManager(World* world);
        ~UIManager();
        void draw(sf::RenderWindow& window);
        void update(sf::RenderWindow& window);
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