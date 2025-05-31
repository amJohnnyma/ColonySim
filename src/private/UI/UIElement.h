#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../game/headers/World.h"
#include "../shapes/RoundedRectangle.h"
#include "Button.h"
#include <optional>
#include "../structs/enums.h"
#include "../game/headers/Game.h"
class UIElement;
struct FunctionArgs
{
    std::optional<std::string> name;
    std::optional<int> count;
    std::optional<UIElement*> element = nullptr;
    std::optional<int> x;
    std::optional<int> y;
};

using FunctionType = std::function<void(World*, const FunctionArgs&)>;
class UIElement
{
    protected:
        World* world;
        std::string function;
        FunctionArgs args;
        bool visible = true;
        int priority = 10;

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
        virtual void setFontSize(int size) = 0;
        void setVisible(bool val) {visible = val;}
        bool isVisible() {return visible;}
        virtual void move(int x, int y) = 0;
        void setPriority(int val) {priority=val;}
        int getPrio(){return priority;}
};

#endif