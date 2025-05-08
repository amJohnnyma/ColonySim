#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include "../../structs/worldObjects.h"
#include "WorldGeneration.h"
#include "../../algorithms/includes/ACO.h"
#include <array>
#include <algorithm>



class World
{
    private:
        int temp = 0;
        int width, height;
        std::vector<std::unique_ptr<Cell>> grid;
     //   std::vector<Cell> drawShapes; //drawShapes.push_back(*grid[i]);
      //  std::vector<Cell> base; //still needs to be assigned
        const int cellSize = 50;
        sf::View view;
        float currentZoom = 1.0f;
        float baseSpeed = 1.0f;
        float speed;
        std::vector<ACO> sims;
        bool running = false;

    public:
    private:
        void drawGrid(sf::RenderWindow & window);
        void drawTerrain(sf::RenderWindow & window);
        void drawEntities(sf::RenderWindow& window);
        void createACO();
    public:
        World(int w, int h, sf::RenderWindow& window);
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
        //temp input
        void handleInput(sf::RenderWindow& window);
        bool isRunning() { return running;}

    
};


#endif