#ifndef RECT_H
#define RECT_H
#include "../shapes/RoundedRectangle.h"
#include "UIElement.h"
#include <iostream>

class Rect : public UIElement
{
    private:
        RoundedRectangle* shape;
    public:
        Rect(int x, int y, int resolution, int radius, int width, int height);
        ~Rect();
        void update() {

        }
    

    void draw(sf::RenderWindow& window) {
        // Draw in default view so it overlays UI properly
        sf::View originalView = window.getView();
        window.setView(window.getDefaultView());
        shape->draw(window);
        window.setView(originalView);
    }
};

#endif