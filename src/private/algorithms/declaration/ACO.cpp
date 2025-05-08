#include "../includes/ACO.h"
#include "../../game/headers/GlobalVars.h"


ACO::ACO(Cell *startCell, std::vector<Cell *> &goals, std::vector<Cell *> &world, int width, int height) 
{
   // std::cout << "ACO construct statr" << std::endl;
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

    for(auto &g : goals)
    {
        this->goals.insert(g);
    }

    this->world = world;
    worldWidth = width;
    worldHeight = height;
    curCell = startCell;

    numberAnts = startCell->data.entities.size();


   // std::cout << "ACO construct end" << std::endl;

}

void ACO::update()
{
 //   std::cout << "Updating aco" << std::endl;
    //from start cell, look at each entity (ant)
    for(auto &cell:world)
    {
    for(auto &e : cell->data.entities)
    {
       // std::cout << "Checking entity" << std::endl;
        if(e->name == "ant")
        {
           // std::cout<<"Getting adj cells"<<std::endl;
            getAdjCells(e->x, e->y); 
            //compare all adjacent squares
            std::vector<std::pair<Cell*, double>> scores;
            for(const auto & ac: adjCells)
            {
              //  std::cout << "Checking adj cell" << std::endl;
                //Look at distance to goal, difficulty of terrain -> heuristic
                //check pheromones and bias toward following already successful pheromones
                if (visited.find(ac) == visited.end()) {
                  //  std::cout << "Not visited" << std::endl;
                    scores.push_back({ac, pheromoneCalc(ac->data.p.strength, calculateHeuristic(curCell, ac), curCell)});
                }
            }       

            //choose score
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.0,1.0);
            double randomVal = dis(gen);
            double cumulative = 0.0;
            for(const auto &s : scores)
            {
                cumulative += s.second;
                if(randomVal <= cumulative)
                {
                   // std::cout << "Chose random cell" << std::endl;
                    //choose the cell S as our next cell
                    visited.insert(s.first);
                    //if s is a goal then start heading back home to reinforce pheremones further
                    if(goals.find(s.first) != visited.end())
                    {
                        //go back home
                    }



                    //update pheremones for this cell
                    double updatedP = 0.0;
                    updatedP = Q*curCell->data.p.strength/s.second ;
                    curCell->data.p.strength = updatedP;

                    //move to new cell
                    //std::cout << "Moving entity: " << curCell->x << ", " << curCell->y << std::endl;
                    moveToCell(curCell, s.first, e.get());
                    //std::cout << "Moved entity: " << s.first->x << ", " << s.first->y << std::endl;

                    break;

                    
                }
            }  

        }
    }
        //update pheremones of all cells

    double pLevel = cell->data.p.strength;
    double updatedP = (1-pheremoneEvap) * pLevel + (std::abs(((1-pheremoneEvap) * pLevel) - pLevel));
    cell->data.p.strength = updatedP;

    if (cell->data.p.strength > maxPheromone) maxPheromone = cell->data.p.strength;
}

 



    

    

}

void ACO::moveToCell(Cell* from, Cell* to, entity* e)
{
    auto& fromEntities = from->data.entities;
    auto& toEntities = to->data.entities;

    for (auto it = fromEntities.begin(); it != fromEntities.end(); ++it)
    {
        if (it->get() == e) // compare raw pointer inside unique_ptr
        {
            toEntities.push_back(std::move(*it)); // move ownership
            fromEntities.erase(it);               // erase from original cell
            curCell = to;
            break;
        }
    }

    // Optionally update the entity's position
    e->x = to->x;
    e->y = to->y;
}

//not used currently
void ACO::depositPheremones(Cell *c)
{
    double pheremones = 0.0;

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
