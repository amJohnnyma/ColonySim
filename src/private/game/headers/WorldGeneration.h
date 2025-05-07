#ifndef WORLDGENERATION_H
#define WORLDGENERATION_H

#include <iostream>
#include "../../structs/worldObjects.h"
#include <random>

class WorldGeneration
{
    private:
        unsigned int seed;
        std::vector<std::unique_ptr<Cell>> grid;
    public:
        WorldGeneration(unsigned int seed, int xWidth, int yWidth, int cellSize);
        ~WorldGeneration();
        std::vector<std::unique_ptr<Cell>> getResult();
};

#endif