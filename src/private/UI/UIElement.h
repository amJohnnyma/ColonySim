#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <SFML/Graphics.hpp>
#include <iostream>
class UIElement
{
    private:
    public:
        UIElement();
        ~UIElement() {};
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual void update(sf::RenderWindow& window) = 0;
        virtual void onClick() = 0;
};

#endif