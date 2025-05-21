#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H


#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "GlobalVars.h"
#include "../entities/Entity.h"
#include "../shapes/Shape.h"

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


struct CellData
{
    std::string type = "Default";
    std::string terrain ="flat";
    double difficulty = 0;
    pheromone p[2];
    std::vector<std::unique_ptr<Entity>> entities;

    
};

struct Cell
{
 //   std::unique_ptr<sf::CircleShape> cs = std::make_unique<sf::CircleShape>();
    std::unique_ptr<Shape> cellShape;
    //grid position
    int x,y;
    CellData data;

    //need copy constructor and operator=
    void setColor(sf::Color col)
    {
        this->cellShape->setFillColor(col);
    }
    ~Cell() {}
};







#endif