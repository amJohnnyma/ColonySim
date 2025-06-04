#ifndef WORLDGENERATION_H
#define WORLDGENERATION_H

#include <iostream>
#include <array>
#include <random>

//can all be forward decalred??
#include "../../structs/worldObjects.h"
#include "../../utils/headers/TextureManager.h"
#include "../../entities/Entity.h"
#include "../../shapes/Shape.h"
#include "../../shapes/Triangle.h"
#include "../../shapes/Circle.h"
#include "../../shapes/RoundedRectangle.h"
#include "../../shapes/Rectangle.h"
#include "../../entities/Ant.h"
#include "../../entities/Location.h"
#include "../../entities/FoodLocation.h"
#include "../../entities/BuildingLocation.h"
#include "Chunk.h"
#include "PerlinNoise.h"

enum Biome
{
    WATER,
    BEACH,
    FOREST,
    JUNGLE,
    WOODS
};

class WorldGeneration
{
    private:
        PerlinNoise* perlinNoise = new PerlinNoise(conf::seed);
        unsigned int seed;
        std::map<std::pair<int, int>, std::unique_ptr<Chunk>> grid;
        int width,height,cellSize;
    public:
        WorldGeneration(unsigned int seed, int xWidth, int yWidth, int cellSize);
        ~WorldGeneration();
        std::map<std::pair<int, int>, std::unique_ptr<Chunk>> getResult();

    private:
        pheromone createPheromones(int x, int y);
        double generateDifficulty();
        std::unique_ptr<Rectangle> createCellShape(int x, int y, float size);
        std::unique_ptr<Cell> createCell(int x, int y, float cellSize, float noise);
        void generateTerrain();
        std::unique_ptr<sf::Sprite> createAntShape(sf::Color fillColor, int x, int y, float cellSize);
        std::unique_ptr<sf::Sprite> createBaseShape(sf::Color fillColor, int x, int y, float cellSize);
        std::unique_ptr<Ant> createAnt(int x, int y);
        std::unique_ptr<Location> createBase(int x, int y, TeamInfo p);
        void logAllEntities();
        void generateEntities(int num, int col);
        void assignTextures();
        std::unique_ptr<sf::Sprite> createLocationShape(int x, int y, float cellSize, double difficulty);
        void generateLocations(int num);
        float getDifficulty(int x, int y);
        void createBuilding(int x, int y, std::string type);
        Biome getBiome(float val);
        sf::Color colorFromBiome(Biome b);
};

#endif