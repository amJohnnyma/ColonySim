#include "../headers/WorldGeneration.h"

WorldGeneration::WorldGeneration(unsigned int seed, int xWidth, int yWidth, int cellSize)
{
    for(int x = 0; x < xWidth; x++)
    {
        for(int y = 0; y < yWidth; y++)
        {
            std::unique_ptr<sf::CircleShape> shape = std::make_unique<sf::CircleShape>(cellSize/4);
            shape->setOrigin(cellSize/4,cellSize/4);
            shape->setPosition(
                x * cellSize + cellSize / 2.f,
                y * cellSize + cellSize / 2.f
            );

            std::unique_ptr<Cell> newC = std::make_unique<Cell>();     
            CellData cd;


            cd.type = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
            //random between 0-1. 0 easy, 1 hard
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.0,1.0);
            double randomVal = dis(gen);
            cd.difficulty = randomVal;

            shape->setFillColor(sf::Color(0,255,0, (randomVal*255)));    

            newC->cs = std::move(shape);   
            newC->x = x;
            newC->y = y;
            newC->data = cd;
            grid.push_back(std::move(newC));
        }
    }
}

WorldGeneration::~WorldGeneration()
{
}

std::vector<std::unique_ptr<Cell>> WorldGeneration::getResult()
{
    return std::move(grid);
}
