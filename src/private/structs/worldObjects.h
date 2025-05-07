#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H


#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

struct GridObject
{
    virtual ~GridObject() = default;
};


struct CellData
{
    std::string type = "Default";
    std::string terrain ="flat";
    double difficulty = 0;
};

struct Cell
{
    //std::shared_ptr<GridObject> object;
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

struct tempentitiy
{
    int x,y;
    std::unique_ptr<sf::CircleShape> cs;
};




#endif