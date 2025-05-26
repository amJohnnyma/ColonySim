#include "Ant.h"

Ant::Ant(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::RectangleShape> hitbox, Cell *currentCell)
: Entity(x,y,name,maxResource,std::move(hitbox),currentCell)
{

}

void Ant::addPath(Cell *newC)
{
    this->path.push_back(newC);

        if (path.size() >= conf::antPathDrawLength)
        {
            this->path.erase(path.begin());  // Removes the front element in a vector        
        }
}

void Ant::setRotation(double angle)
{
    this->hitbox.get()->setRotation(static_cast<float>(angle));
}
