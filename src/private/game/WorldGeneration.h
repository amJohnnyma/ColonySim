#ifndef WORLDGENERATION_H
#define WORLDGENERATION_H

#include <iostream>
#include <array>
#include <random>

#include "../structs/worldObjects.h"
#include "../game/Cell.h"
#include "../utils/TextureManager.h"
#include "../entities/Entity.h"
#include "../shapes/Shape.h"
#include "../shapes/Triangle.h"
#include "../shapes/Circle.h"
#include "../shapes/RoundedRectangle.h"
#include "../shapes/Rectangle.h"
#include "../entities/Ant.h"
#include "../entities/Location.h"
#include "../entities/FoodLocation.h"
#include "../entities/BuildingLocation.h"
#include "PerlinNoise.h"

class Chunk;
class ChunkManager;
class World;
class WorldGeneration
{
    private:
        PerlinNoise* perlinNoise;
        unsigned int seed;
        //std::map<std::pair<int, int>, std::unique_ptr<Chunk>> grid;
        std::unordered_map<std::pair<int, int>, uint32_t, pair_hash> grid; //throwaway int value
        ChunkManager* cm;
        uint32_t chunkCount = 0;
        int cellSize;

    public:
        WorldGeneration(unsigned int seed, ChunkManager* cm, int cellSize);
        ~WorldGeneration();
        std::unordered_map<std::pair<int, int>, Chunk*, pair_hash> getResult();
        void createChunk(int chunkX, int chunkY, World* world);
        const std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, pair_hash> &getGridRef() const;

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
        std::unique_ptr<Location> createLocation(int x, int y, double difficulty);
        void logAllEntities();
        void generateEntities(int num, int col);
        void assignTextures();
        std::unique_ptr<sf::Sprite> createLocationShape(int x, int y, float cellSize, double difficulty);
        void generateLocations(int num);
        float getDifficulty(int x, int y);
        void createBuilding(int x, int y, std::string type);
        BiomeData getBiome(float val);
        sf::Color colorFromBiome(Biome b);
};

#endif

