#ifndef ACO_H
#define ACO_H

#include "../../structs/worldObjects.h"
#include <iostream>
#include <cmath>




class ACO
{

    private:
        Cell* startCell;
        Cell* curCell;
        std::vector<Cell*> goals;
        std::vector<Cell*> world;
        float theta = 0.0f;
        std::vector<Cell*> adjCells;
        int worldWidth;
        int worldHeight;
        double pF = 0.5; //pheremone influnce
        double hF = 0.5; // heuristic influence

    private:
        void getAdjCells(int x, int y);
        /*
        • τij (t) is the pheromone value on edge (i, j) at time t.
        • ηij is the heuristic value (often the inverse of the distance) for edge (i, j).
        • α is a parameter that controls the influence of the pheromone trail.
        • β is a parameter that controls the influence of the heuristic value.
        • Ni is the set of feasible nodes that can be visited from node i.
        */
        double pheromoneCalc(double Tij, double Nij, Cell *cur);   
        double sumOfFeasiblePheremoneProb(Cell *cur);     
        double calculateHeuristic(Cell* cur, Cell* next);
        //pair<cell, score>
        std::vector<std::pair<Cell*, double>> computeScores();


        




        
    public:
        ACO(Cell* startCell, std::vector<Cell*>& goals, std::vector<Cell*>& world, int width, int height);
        void update();
};

#endif