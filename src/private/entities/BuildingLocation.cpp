#include "BuildingLocation.h"

BuildingLocation::BuildingLocation(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::Sprite> hitbox, Cell *currentCell)
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

    auto sprite = std::make_unique<sf::Sprite>();

    this->hitbox = std::move(sprite);
}
