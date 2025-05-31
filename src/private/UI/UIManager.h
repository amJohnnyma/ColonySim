#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include "../structs/enums.h"
class UIElement;
class World;

class UIManager
{
    private:
            std::vector<UIElement*> elements;
            int width, height;
            std::pair<int,int> midScreen;
    public:
        UIManager(World* world);
        ~UIManager();
        void draw(sf::RenderWindow& window);
        void setVisibilityForState(State gameState);
        void update(sf::RenderWindow &window);
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