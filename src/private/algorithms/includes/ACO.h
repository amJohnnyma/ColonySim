#ifndef ACO_H
#define ACO_H

#include "../../structs/worldObjects.h"
#include <iostream>
#include <cmath>
#include <random>
#include <unordered_set>





class ACO
{

    private:
        Cell* startCell;
        Cell* curCell;
        std::unordered_set<Cell*> goals;
        std::vector<Cell*> world;
        float theta = 0.0f;
        std::vector<Cell*> adjCells;
        int worldWidth;
        int worldHeight;
        std::unordered_set<Cell*> visited;
        int numberAnts = 1;
        Entity* base;
        Entity* target;
        std::vector<Entity*> tl; // goals
        bool possibleLocations = true;



    private:
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
        void findFood(Cell* cell, Entity* e);
        void returnHome(Cell* cell, Entity* e);
        void getNewTarget(Entity* e);
        void transferResource(Entity* from, Entity* to, int amount);



        




        
    public:
        ACO(Cell* startCell, std::vector<Cell*>& goals, std::vector<Cell*>& world, int width, int height);
        ~ACO();
        void update();
        void assignRandomTarget(std::vector<Cell*> &raw_goals);
 
};

#endif