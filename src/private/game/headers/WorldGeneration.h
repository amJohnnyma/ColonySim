#ifndef WORLDGENERATION_H
#define WORLDGENERATION_H

#include <iostream>
#include "../../structs/worldObjects.h"
#include <random>
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
#include <array>


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
        std::array<pheromone, 2> createPheromones(int x, int y);
        double generateDifficulty();
        std::unique_ptr<Rectangle> createCellShape(int x, int y, float size, double difficulty);
        std::unique_ptr<Cell> createCell(int x, int y, float cellSize);
        void generateTerrain();
        std::unique_ptr<sf::RectangleShape> createShape(sf::Color fillColor, int x, int y, float cellSize);
        std::unique_ptr<Ant> createAnt(int x, int y);
        std::unique_ptr<Location> createBase(int x, int y);
        void logAllEntities();
        void generateEntities(int num, int col);
        void assignTextures();
        std::unique_ptr<sf::RectangleShape> createLocationShape(int x, int y, float cellSize, double difficulty);
        void generateLocations(int num);
};

#endif