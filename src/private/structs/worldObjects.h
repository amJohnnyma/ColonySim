#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H


#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

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

struct entity //Should probably be a class
{
    int x,y;
    std::string name;
    std::unique_ptr<sf::RectangleShape> hitbox = std::make_unique<sf::RectangleShape>();

 //   std::unique_ptr<sf::Sprite> sprite = std::make_unique<sf::Sprite>();

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