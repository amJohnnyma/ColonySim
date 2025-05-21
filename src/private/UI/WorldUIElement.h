#ifndef WORLD_UI_ELEMENT_H
#define WORLD_UI_ELEMENT_H
#include "UIElement.h"
#include <iostream>
#include <unordered_map>


class WorldUIElement : public UIElement
{
    private:
        RoundedRectangle* shape;
        Button* button;
        sf::Font font;
        sf::Text text;
        std::function<void(World*, const FunctionArgs&)> updateFunc;
    public:
        WorldUIElement(World* world, int x, int y, int resolution, int radius, int width, int height, std::string function, std::string textArea);
        ~WorldUIElement() override;
        void update(sf::RenderWindow& window) override;
        void onClick() override;

    const std::unordered_map<std::string, std::function<void(World*, const FunctionArgs&)>>& getFunctionMap();
        void draw(sf::RenderWindow &window) override;

        void setColor(sf::Color col);
        void setText(std::string text);
        std::string getText();



};

#endif