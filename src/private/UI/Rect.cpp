#include "Rect.h"

Rect::Rect(int x, int y, int resolution, int radius, int width, int height)
{
    shape = new RoundedRectangle(x,y,radius,resolution,width, height);
    shape->setFillColor(sf::Color::White);
}

Rect::~Rect()
{
    delete shape;
}
