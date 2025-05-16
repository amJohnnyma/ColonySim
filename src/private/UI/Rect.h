#ifndef RECT_H
#define RECT_H
#include "../shapes/RoundedRectangle.h"
#include "UIElement.h"
#include "Button.h"
#include <iostream>

class Rect : public UIElement
{
    private:
        RoundedRectangle* shape;
        Button* button;
    public:
        Rect(World* world, int x, int y, int resolution, int radius, int width, int height);
        ~Rect();
        void update(sf::RenderWindow& window) override;
        void onClick() override;
    

    void draw(sf::RenderWindow& window) override;
};

#endif