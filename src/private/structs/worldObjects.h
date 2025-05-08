#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H


#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "GlobalVars.h"

struct GridObject
{
    virtual ~GridObject() = default;
};

struct pheromone
{
    double strength;
    int x,y; //coords
    std::string type;
};

struct entity //Make this a class
{
    int x,y;
    std::string name;
    int resource;
    int maxResource;
    std::unique_ptr<sf::RectangleShape> hitbox = std::make_unique<sf::RectangleShape>();

    void setPos(int x, int y)
    {
        hitbox.get()->setPosition(x * cellSize, y * cellSize);  
        this->x = x;
        this->y = y;
    }

 //   std::unique_ptr<sf::Sprite> sprite = std::make_unique<sf::Sprite>();
    int takeResource(int amount)
    {
        if(amount <= resource)
        {
            resource -= amount;
            return amount;
        }
        else{
            resource = 0;
            return resource;
        }
    }
//r = 5
//mr = 10
//add 7 //add 3


int giveResource(int amount)
{
    int afterAdd = amount + resource; // amount to be added to the current resource

    // If adding the resource would exceed maxResource
    if(afterAdd > maxResource)
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

};

struct CellData
{
    std::string type = "Default";
    std::string terrain ="flat";
    double difficulty = 0;
    pheromone p;
    std::vector<std::unique_ptr<entity>> entities;

    
};

struct Cell
{
    std::unique_ptr<sf::CircleShape> cs = std::make_unique<sf::CircleShape>();
    //grid position
    int x,y;
    CellData data;

    //need copy constructor and operator=
    void setColor(sf::Color col)
    {
        this->cs->setFillColor(col);
    }
};







#endif