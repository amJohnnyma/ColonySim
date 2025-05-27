#include "BuildingLocation.h"

BuildingLocation::BuildingLocation(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::RectangleShape> hitbox, Cell *currentCell)
: Location(x,y,name,maxResource,std::move(hitbox), currentCell)
{
}

BuildingLocation::BuildingLocation(int x, int y, std::string name)
{
    this->x = x;
    this->y = y;
    this->name = name;

    //depending on type change the hitbox
    int cellSize = conf::cellSize;
    auto rs = std::make_unique<sf::RectangleShape>(sf::Vector2f(cellSize, cellSize));
    rs->setPosition(x * cellSize, y * cellSize);
    rs->setOutlineThickness(5.f);
    rs->setOutlineColor(sf::Color::White);
    rs->setFillColor(sf::Color(255, 150, 150, 255));

    this->hitbox = std::move(rs);
}
