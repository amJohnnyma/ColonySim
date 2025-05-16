#include "Location.h"

Location::Location(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::RectangleShape> hitbox, Cell* currentCell)
: Entity(x,y,name,maxResource,std::move(hitbox), currentCell)
{
}