#include "Entity.h"


int Entity::takeResource(int amount)
{
    if (amount <= resource)
    {
        resource -= amount;
        return amount;
    }
    else
    {
        int taken = resource;
        resource = 0;
        return taken;
    }
}

int Entity::giveResource(int amount)
{
    int afterAdd = amount + resource; // amount to be added to the current resource

    // If adding the resource would exceed maxResource
    if (afterAdd > maxResource)
    {
        int excess = afterAdd - maxResource; // calculate how much is excess
        resource = maxResource; // set resource to maxResource (it cannot go beyond this)
        return excess; // return the excess that couldn't be added
    }
    else
    {
        resource += amount; // add the amount to the resource if it doesn't exceed maxResource
    }

    return 0; // if no excess, return 0 (no remaining)
}

Entity::Entity(int x, int y, std::string name, int maxResource)
{
    this->x = x;
    this->y = y;
    this->name = name;
    this->maxResource = maxResource;
}


Entity::Entity(int x, int y, std::string name, int maxResource, std::unique_ptr<sf::RectangleShape> hitbox, Cell* curCell)
{
    this->x = x;
    this->y = y;
    this->name = name;
    this->maxResource = maxResource;
    this->hitbox = std::move(hitbox);
  //  this->currentCell = curCell;
}

void Entity::setPos(int x, int y)
{
    hitbox->setPosition(x * conf::cellSize, y * conf::cellSize);  
    this->x = x;
    this->y = y;
}