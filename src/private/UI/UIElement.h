#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../game/headers/World.h"
#include "../shapes/RoundedRectangle.h"
#include "Button.h"
#include <optional>

class UIElement;
struct FunctionArgs
{
    std::optional<std::string> name;
    std::optional<int> count;
    std::optional<UIElement*> element = nullptr;
};

using FunctionType = std::function<void(World*, const FunctionArgs&)>;
class UIElement
{
    protected:
        World* world;
        std::string function;
        FunctionArgs args;

    public:
        UIElement(World* world) : world(world) {}
        virtual ~UIElement();
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual void update(sf::RenderWindow& window) = 0;
        virtual void onClick() = 0;
        void setArgs(const FunctionArgs& newArgs) {
            args = newArgs;
        
        }
        virtual void setColor(sf::Color col) = 0;
        virtual void setText(std::string text) = 0;
        virtual std::string getText() = 0;
};

#endif