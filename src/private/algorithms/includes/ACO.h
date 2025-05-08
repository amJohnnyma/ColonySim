#ifndef ACO_H
#define ACO_H

#include "../../structs/worldObjects.h"
#include <iostream>

class ACO
{
    //start
    
    //food nodes
    
    //restrictions
    /*
    private:
    int numNodes; 
        int numVehicles;
        std::string tMaxString;
        double tmax;
        int score;
        std::vector<Cell> visited;
        std::vector<vehicle> vehicles;
        std::pair<std::string, coord> depot;
        std::map<std::string, coord> node_coord_section;
        std::string imageName;

        //id, vector<{id , distance}>
        std::map<std::string, std::vector<std::pair<std::string, double>>> adjList;
        std::string debugfile = "TOP";
        int id = 1;
        bool changes = true;        
        double n; //heuristic value
        double s; //score
        double c; // travel cost

        std::vector<std::pair<double, sf::Vertex>> edges; 
      //  std::vector<std::pair<double, std::pair<coord,sf::Vertex>>> alledges;   
        std::vector<std::pair<coord, std::vector<std::pair<double, std::pair<coord, sf::Vertex>>>>> alledges;
        int runNum = 1;  
        param params;
        std::mt19937 rng;
        double getBestScore();
        double prevBestScore = 0;
        int maxItNoImpro = 0;
        seedResult result;
    */




        




        void update();
    public:
        ACO(Cell* startCell, std::vector<Cell*> & goals);
};

#endif