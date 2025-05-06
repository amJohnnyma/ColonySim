#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H


#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

struct GridObject
{
    virtual ~GridObject() = default;
};

struct Cell
{
    //std::shared_ptr<GridObject> object;
    std::unique_ptr<sf::CircleShape> cs = std::make_unique<sf::CircleShape>();
};

struct World
{
    int width, height;
    std::vector<Cell> grid;

    World(int w, int h): width(w), height(h), grid(w*h) {}

    Cell& at(int x, int y)
    {
        return grid[y * width + x];
    }

    const Cell& at(int x, int y) const
    {
        return grid[y*width+x];
    }
};

#endif