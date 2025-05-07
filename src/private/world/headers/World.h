#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include "../../structs/worldObjects.h"
#include <array>


class World
{
    private:
        int temp = 0;
        int width, height;
        std::vector<std::unique_ptr<Cell>> grid;
        std::vector<Cell> drawShapes; //drawShapes.push_back(*grid[i]);
        std::vector<Cell> base;
        const int cellSize = 50;
    public:
    private:
    public:
        World(int w, int h);
        ~World();
    public:
        int getWidth();
        int getHeight();
        std::vector<std::unique_ptr<Cell>> getGrid();
        std::vector<Cell> getBase();
        std::unique_ptr<Cell>& at(int x, int y);
        const std::unique_ptr<Cell>& at(int x, int y) const;
        void update();
        void render(sf::RenderWindow & window);
    
};


#endif