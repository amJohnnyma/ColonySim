#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "../game/headers/GlobalVars.h"
#include "../structs/worldObjects.h"

class Entity
{
private:
    int x, y;
    std::string name;
    int resource;
    int maxResource;
    Entity* target;
    Cell* currentCell;
    std::unique_ptr<sf::RectangleShape> hitbox;

public:
    // Constructor
    Entity(int x, int y, std::string name, int maxResource);
    Entity(int x, int y, std::string name, int maxResource, std::unique_ptr<sf::RectangleShape> hitbox, Cell* currentCell);

    // Getter and setter for position
    void setPos(int x, int y);


    // Function to take resources
    int takeResource(int amount);


    // Function to give resources
    int giveResource(int amount);


    // Additional getter methods for private members
    int getX() const { return x; }
    int getY() const { return y; }
    int getResource() const { return resource; }
    int getMaxResource() const { return maxResource; }
    const std::string& getName() const { return name; }
    Entity* getTarget() const { return target; }
    sf::RectangleShape* getHitbox() { return hitbox.get(); }


    // Setters

    void setHitbox(std::unique_ptr<sf::RectangleShape> newHitbox) {
        hitbox = std::move(newHitbox);
    }

    
void setX(int newX) { 
    x = newX; 
    hitbox->setPosition(x * cellSize, y * cellSize);
}

void setY(int newY) { 
    y = newY; 
    hitbox->setPosition(x * cellSize, y * cellSize);
}

void setName(const std::string& newName) { name = newName; }

void setResource(int newResource) {
    resource = std::min(newResource, maxResource);
}

void setMaxResource(int newMax) {
    maxResource = newMax;
    if (resource > maxResource) resource = maxResource;
}

void setTarget(Entity* newTarget) { target = newTarget; }
};


#endif