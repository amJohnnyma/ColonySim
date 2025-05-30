#include "Ant.h"

Ant::Ant(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::Sprite> hitbox, Cell *currentCell)
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
 //   sf::Vector2f size = this->getHitbox()->getSize(); // local size of the rectangle
 //   this->getHitbox()->setOrigin(size.x / 2.0f, size.y / 2.0f); // set origin to center
  //  this->getHitbox()->setRotation(static_cast<float>(angle));
}

void Ant::startMovingTo(float x, float y)
{
    startPos = hitbox->getPosition();
    targetPos = sf::Vector2f(x, y);
    elapsedTime = 0.0f;
    isMoving = true;
}

void Ant::updateMovement(float dt)
{
    if (!isMoving) return;

    sf::Vector2f pos = hitbox->getPosition();
    sf::Vector2f direction = targetPos - pos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 1.0f)
    {
        hitbox->setPosition(targetPos); // Snap to final target
        isMoving = false;
        return;
    }

    sf::Vector2f normalizedDir = direction / distance;
    sf::Vector2f velocity = normalizedDir * conf::antSpeed * dt;

    hitbox->move(velocity);
}

double Ant::takeDamage(double amount)
{
    this->hitpoints-=amount;
    if(hitpoints <= 0)
    {
        std::cout << "Died" << std::endl;
    }

    return this->hitpoints; //if less than 0 then killed
}

double Ant::dealDamage()
{
    return this->damage;
}

double Ant::attack(Ant *target)
{
    if(target)
        return target->takeDamage(this->dealDamage());
    if(this->attackTarget)
        return this->attackTarget->takeDamage(this->dealDamage());
}

void Ant::setAttackTarget(Ant *ant)
{
    std::cout << "Atttack target set" << std::endl;
    this->attackTarget = ant;
}
