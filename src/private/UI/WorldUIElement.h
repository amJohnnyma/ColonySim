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


    public:
        WorldUIElement(World* world, int x, int y, int resolution, int radius, int width, int height, std::string function);
        ~WorldUIElement();
        void update(sf::RenderWindow& window) override;
        void onClick() override;

    static const std::unordered_map<std::string, std::function<void(World*, const FunctionArgs&)>>& getFunctionMap();
        void draw(sf::RenderWindow &window) override;



};

#endif