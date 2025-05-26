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
    sf::Color originalColor;

    //need copy constructor and operator=
    void setColor(sf::Color col)
    {
        originalColor = col;
        this->cellShape->setFillColor(col);
    }
    void setFilterOverlay(sf::Color col)
    {
        sf::Color current = this->cellShape->getColor();
        if(current!=originalColor)
        {
            setColor(originalColor);
        }

        sf::Uint8 r = std::min(255, current.r + col.r);
        sf::Uint8 g = std::min(255, current.g + col.g);
        sf::Uint8 b = std::min(255, current.b + col.b);
        sf::Uint8 a = std::min(255, current.a + col.a);

        this->cellShape->setFillColor(sf::Color(r, g, b, a));
    }
    void restoreColor()
    {
        this->cellShape->setFillColor(originalColor);
    }
    ~Cell() {}
};







#endif