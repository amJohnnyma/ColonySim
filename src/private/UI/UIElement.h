#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../game/headers/World.h"
class UIElement
{
    protected:
        World* world;
    public:
        UIElement(World* world) : world(world) {}
        ~UIElement() {};
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual void update(sf::RenderWindow& window) = 0;
        virtual void onClick() = 0;
};

#endif