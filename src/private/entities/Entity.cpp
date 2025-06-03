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

void Entity::setTexture(const sf::Texture& texture)
{
    if (!hitbox)
        hitbox = std::make_unique<sf::Sprite>();

    std::cout << "Texturing entity" << std::endl;

    hitbox->setTexture(texture);

    float scale = conf::cellSize / texture.getSize().x;
    hitbox->setScale(scale, scale);

    hitbox->setPosition(this->x * conf::cellSize, this->y * conf::cellSize);

    std::cout << "Done" << std::endl;
}

Entity::Entity(int x, int y, std::string name, double maxResource)
{
    this->x = x;
    this->y = y;
    this->name = name;
    this->maxResource = maxResource;
}


Entity::Entity(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::Sprite> hitbox, Cell* curCell)
{
    this->x = x;
    this->y = y;
    this->name = name;
    this->maxResource = maxResource;
    this->hitbox = std::move(hitbox);
    this->currentCell = curCell;
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const 
{
    //std::cout << "Draw entityt" << std::endl;
    target.draw(*this->hitbox, states);
}

void Entity::setPos(int x, int y)
{
    this->x = x;
    this->y = y;
    hitbox->setPosition(x * conf::cellSize, y * conf::cellSize);  
}

