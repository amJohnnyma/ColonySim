#include "Rect.h"

Rect::Rect(World* world, int x, int y, int resolution, int radius, int width, int height) : UIElement(world)
{
   // std::cout << "rect" << std::endl;
    shape = new RoundedRectangle(x,y,radius,resolution,width, height);
    shape->setFillColor(sf::Color::White);

    button = new Button(x * conf::cellSize, y * conf::cellSize, width, height, [world](){
        world->testClick();
    });
}

Rect::~Rect()
{
    delete shape;
    delete button;
}

void Rect::draw(sf::RenderWindow& window)
{
    // Draw in default view so it overlays UI properly
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());
    shape->draw(window);
    window.setView(originalView);
}

void Rect::update(sf::RenderWindow& window)
{
    if(button)
    {
        if(button->isClicked(window))
        {
            onClick();
        }
    }
}

void Rect::onClick()
{
   // std::cout << "Rect clicked" << std::endl;
}
