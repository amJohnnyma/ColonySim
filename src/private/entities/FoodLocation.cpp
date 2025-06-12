#include "FoodLocation.h"

FoodLocation::FoodLocation(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::Sprite> hitbox, Cell *currentCell)
: Location(x,y,name,maxResource,std::move(hitbox), currentCell)
{
}