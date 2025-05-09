#ifndef WORLDGENERATION_H
#define WORLDGENERATION_H

#include <iostream>
#include "../../structs/worldObjects.h"
#include <random>
#include "../../utils/headers/TextureManager.h"
#include "../../entities/Entity.h"

class WorldGeneration
{
    private:
        unsigned int seed;
        std::vector<std::unique_ptr<Cell>> grid;
        int width,height,cellSize;
    public:
        WorldGeneration(unsigned int seed, int xWidth, int yWidth, int cellSize);
        ~WorldGeneration();
        std::vector<std::unique_ptr<Cell>> getResult();
    private:
        void generateTerrain();
        void generateEntities(int num,int col);
        void assignTextures();
        void generateLocations(int num);

};

#endif