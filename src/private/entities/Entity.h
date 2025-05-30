#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "../game/headers/GlobalVars.h"

class Cell;

class Entity
{
protected:
    int x, y;
    std::string name;
    double resource = 0;
    double maxResource =0;
    std::unique_ptr<sf::RectangleShape> hitbox;

public:
    // Constructor
    Entity(int x, int y, std::string name, double maxResource);
    Entity(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::RectangleShape> hitbox, Cell* currentCell);
    virtual ~Entity() {}

    // Getter and setter for position
    void setPos(int x, int y);
    // Function to take resources
    int takeResource(int amount);
    // Function to give resources
    int giveResource(int amount);
    // Additional getter methods for private members
    int getX() const { return x; }
    int getY() const { return y; }
    double getResource() const { return resource; }
    double getMaxResource() const { return maxResource; }
    const std::string& getName() const { return name; }
    sf::RectangleShape* getHitbox() { return hitbox.get(); }
    virtual std::vector<Cell*>& getPath() = 0;

    // Setters

    void setHitbox(std::unique_ptr<sf::RectangleShape> newHitbox) {
        hitbox = std::move(newHitbox);
    }

    
void setX(int newX) { 
    x = newX; 
    hitbox->setPosition(x * conf::cellSize, y * conf::cellSize);
}

void setY(int newY) { 
    y = newY; 
    hitbox->setPosition(x * conf::cellSize, y * conf::cellSize);
}

void setName(const std::string& newName) { name = newName; }

void setResource(double newResource) {
    resource = std::min(newResource, maxResource);
}

void setMaxResource(double newMax) {
    maxResource = newMax;
    if (resource > maxResource) resource = maxResource;
}





};
#endif