#ifndef ACO_H
#define ACO_H

//#include "../../structs/worldObjects.h"
#include <iostream>
#include <cmath>
#include <random>
#include <unordered_set>
#include "../../entities/Entity.h"
#include "../../entities/Ant.h"
#include "../../entities/Location.h"
#include "../../entities/FoodLocation.h"
#include "../../entities/BuildingLocation.h"
#include "../../game/headers/World.h"

class World;
class ACO
{

    private:
        std::pair<int,int> startCell;
        std::pair<int,int> curCell;
        std::vector<std::pair<std::string, std::pair<int,int>>> goals;
        World* world;
        float theta = 0.0f;
        std::vector<std::pair<int, int>> adjCells;
        int worldWidth;
        int worldHeight;
        std::vector<std::pair<int, int>> visited;
        int numberAnts = 0;
        Entity* base;
        std::vector<std::pair<int,int>> tl; // goals
        bool possibleLocations = true;
        TeamInfo team;



    private:
        void handleEnemiesInCell(Cell *tile, Entity *e);
        void getAdjCells(int x, int y, Entity *e);
        /*
        • τij (t) is the pheromone value on edge (i, j) at time t.
        • ηij is the heuristic value (often the inverse of the distance) for edge (i, j).
        • α is a parameter that controls the influence of the pheromone trail.
        • β is a parameter that controls the influence of the heuristic value.
        • Ni is the set of feasible nodes that can be visited from node i.
        */
        double pheromoneCalc(Cell* cell, Entity* target, bool flag);   
        double sumOfFeasiblePheremoneProb(Entity *target, bool flag);     
        double calculateHeuristic(Cell* next, Entity* target);
        //pair<cell, score>
        void moveToCell(Cell* from, Cell * to, Entity* e);
        void depositPheremones(Cell* c);
        void findFood(Cell* cell, Ant* e);
        void returnHome(Cell* cell, Ant* e);
        bool isInBounds(int x, int y) const;
        bool isCellBlocked(Cell *tile) const;
        void getNewTarget(Ant *e);
        void transferResource(Entity* from, Entity* to, int amount);      




        
    public:
        ACO(std::vector<Cell*>& goals, World* world, int width, int height, Entity* base);
        ~ACO();
        void update();
        void assignRandomTarget(std::vector<Cell*> &raw_goals);
        int getNumberAnts() {return numberAnts;}
 
};

#endif