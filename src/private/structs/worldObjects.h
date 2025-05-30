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

struct perTeamPheromone
{

};

struct pheromone
{

    std::map<int, double> pheromoneMap;

};


struct CellData
{
    std::string type = "Default";
    std::string terrain ="flat";
    double difficulty = 0;
    pheromone p; 
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
    void setFilterOverlay(sf::Color overlay, float weight = 0.5f)
    {
        sf::Color base = this->cellShape->getColor();
        this->cellShape->setFillColor(lerpColor(base, overlay, weight));
    }
        sf::Color lerpColor(sf::Color base, sf::Color overlay, float weight)
    {
        sf::Uint8 r = static_cast<sf::Uint8>(base.r * (1.0f - weight) + overlay.r * weight);
        sf::Uint8 g = static_cast<sf::Uint8>(base.g * (1.0f - weight) + overlay.g * weight);
        sf::Uint8 b = static_cast<sf::Uint8>(base.b * (1.0f - weight) + overlay.b * weight);
        sf::Uint8 a = static_cast<sf::Uint8>(base.a * (1.0f - weight) + overlay.a * weight);
        return sf::Color(r, g, b, a);
    }
    void restoreColor()
    {
        this->cellShape->setFillColor(originalColor);
    }
    ~Cell() {}
};







#endif