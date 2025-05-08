#include "../includes/ACO.h"

ACO::ACO(Cell *startCell, std::vector<Cell *> &goals, std::vector<Cell *> &world, int width, int height) 
{
/*
    std::cout<< "Aco construction" << std::endl;
    std::string conns = " ";
    for(auto &i: startCell->data.entities)
    {
        if(i->name == "ant")
        {
            conns += "Ant: (" + std::to_string(i->x) + ", " + std::to_string(i->y) + ")\n";            
        }
    }
    for(auto &g:goals)
    {
        for(auto &i: g->data.entities)
        {
            if(i->name == "location")
            {
                conns += "->location: (" + std::to_string(i->x) + ", " + std::to_string(i->y) + ")\n";
            }
        }
    }
*/
    this->startCell = startCell;
    //goals are entities not specific cells (the entity could be moving around)
    this->world = world;
    worldWidth = width;
    worldHeight = height;
    curCell = startCell;

  //  std::cout << conns << std::endl;

}

void ACO::update()
{
    //from startcell, look at each entity (ant)
    for(auto &e : curCell->data.entities)
    {
        if(e->name == "ant")
        {
            std::vector<Cell*> visited;
            getAdjCells(e->x, e->y); 
            //compare all adjacent squares
            std::vector<std::pair<Cell*, double>> scores;
            for(auto & ac: adjCells)
            {
                //Look at distance to goal, difficulty of terrain -> heuristic
                //check pheromones and bias toward following already successful pheromones
                scores.push_back({ac, pheromoneCalc(ac->data.p.strength, calculateHeuristic(curCell, ac), curCell)});
            }       
            //temp
            for(auto &s : scores)
            {
                std::cout << s.first->data.difficulty << std::endl;
                std::cout << "Cell: " << std::to_string(s.second) << std::endl;
            }
            //
            //update pheremones of adj cells
            //update all pheremones

  

        }
    }



    

    

}

void ACO::getAdjCells(int x, int y)
{
    const int dx[] = {0,0,-1,1};
    const int dy[] = {1,-1,0,0};

    adjCells.clear();

    for(int i =0; i < 4; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        // Check bounds
        if (nx >= 0 && nx < worldWidth && ny >= 0 && ny < worldHeight) {
            int index = ny * worldWidth + nx;
            adjCells.push_back(world[index]);
        }
    }

}

/*
• τij (t) is the pheromone value on edge (i, j) at time t.
• ηij is the heuristic value (often the inverse of the distance) for edge (i, j).
• α is a parameter that controls the influence of the pheromone trail.
• β is a parameter that controls the influence of the heuristic value.
• Ni is the set of feasible nodes that can be visited from node i.
*/
double ACO::pheromoneCalc(double Tij, double Nij,Cell *cur)
{
    
    double top = std::pow(Nij, pF) * std::pow(Tij, hF);
    double bottom = sumOfFeasiblePheremoneProb(cur);

    return top / bottom;

}

double ACO::sumOfFeasiblePheremoneProb(Cell *cur)
{
    double sum = 0.0;
    for(auto &ac : adjCells)
    {
        double heuristic = calculateHeuristic(cur, ac);
        double pheromone = ac->data.p.strength;
        sum += std::pow(heuristic, pF) * std::pow(pheromone, hF);
    }
    return sum + 1e-10;
}


double ACO::calculateHeuristic(Cell *cur, Cell *next)
{
    double dist = std::abs(cur->x - next->x) + std::abs(cur->y - next->y);
    double difficulty = next->data.difficulty; // in range [0, 1]
    double heuristic = (1.0 - difficulty) / (dist + 1e-5);
    
    return heuristic;
}