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
    for(auto &e: startCell->data.entities)
    {
        if(e.get()->getName() != "default")
        {
            std::cout << e.get()->getName() << std::endl;
        }
        if(e.get()->getName() == "Base")
        {
            std::cout << "Ants have base" << std::endl;
            base = e.get();
        }

    }

    for(auto &g : goals)
    {
        this->goals.insert(g);
        
    }


    this->world = world;
    worldWidth = width;
    worldHeight = height;
    curCell = startCell;
    numberAnts = startCell->data.entities.size();
    startCell->setColor(sf::Color::Yellow);

    assignRandomTarget(goals);


   // std::cout << "ACO construct end" << std::endl;

}

void ACO::assignRandomTarget(std::vector<Cell*> &raw_goals)
{
    for(auto &e: startCell->data.entities)
    {
        if(e)
        if(e.get()->getName() == "ant")
        {
            double randomVal;
            double randomVal2;
            std::random_device rd;
            std::mt19937 gen(rd());
            Entity* target = new Entity(1,1,"base",-999);
            while(target->getName() != "location")
            {

            std::uniform_int_distribution<> dis(0.0,startCell->data.entities.size()-1);
            std::uniform_int_distribution<> dis2(0.0,raw_goals[randomVal]->data.entities.size()-1);

            std::cout << "dis1 max: " << startCell->data.entities.size() << std::endl;
            std::cout << "dis2 max: " << raw_goals[randomVal]->data.entities.size() << std::endl;

             randomVal = dis(gen);
             randomVal2 = dis2(gen);
             
             if(target->getMaxResource() == -999)
             {
                delete target;
                target = nullptr;
             }
             target = raw_goals[randomVal]->data.entities[randomVal2].get();
            }
            std::cout << randomVal << ", " << randomVal2 << std::endl;
            e.get()->setTarget(target);
            std::cout << "Set target to: " << target->getName() << "( " << target->getX() << ", " << target->getY()<<" )"<<std::endl;
        }


    }

} 

void ACO::update()
{
    bool noBetterF = true;
    double runBest = 0;
    std::cout << "Updating aco" << std::endl;
    //from start cell, look at each entity (ant)
    for(auto &cell:world)
    {
    for(auto &e : cell->data.entities)
    {
        std::cout << "Checking entity" << std::endl;
        if(e)
        if(e->getName() == "ant")
        {
            std::cout << "found ant" << std::endl;  
            std::cout << "Target: " << e.get()->getTarget()->getName() << std::endl;
            if(e.get()->getTarget() && e.get()->getTarget()->getName() == "Base")
            {
                returnHome(cell, e.get());
            }
            else{
                findFood(cell, e.get());
            }    

        }
    }
        //update pheremones of all cells

    double pLevel = cell->data.p.strength;
    double updatedP = (1-pheremoneEvap) * pLevel + (std::abs(((1-pheremoneEvap) * pLevel) - pLevel));
    cell->data.p.strength = updatedP;

    if (cell->data.p.strength > maxPheromone)
    {
        maxPheromone = cell->data.p.strength;
        noBetterF = false;
    } 
    if (cell->data.p.strength > runBest)
    {
        runBest = cell->data.p.strength;
    } 
}

if(noBetterF)
{
    maxPheromone = runBest;
}

 



    

    

}

/*
 void ACO::moveToCell(Cell* from, Cell* to, entity* e)
 {

     for (auto & ptr : from->data.entities)
     {
         if (ptr.get() == e) // compare raw pointer inside unique_ptr
         {
             ptr->x = to->x;
             ptr->y = to->y;
             to->data.entities.push_back(std::move(ptr));

             break;
         }
     }


 }
 */

void ACO::moveToCell(Cell* from, Cell* to, Entity* e)
{
    // Loop through the entities in the "from" cell
    for (auto it = from->data.entities.begin(); it != from->data.entities.end(); ++it)
    {
        if (it->get() == e) // Compare raw pointer inside unique_ptr
        {
            // Move ownership of the entity to the "to" cell
            to->data.entities.push_back(std::move(*it));
            
            // Adjust entity position to match the "to" cell
            // Assuming you're using grid positions for x, y (change logic if needed)
            e->setPos(to->x, to->y);
            
            // Optionally, erase the entity from the "from" cell after moving
            from->data.entities.erase(it);

            //curCell = to;
            break; // Exit the loop after moving the entity
        }
    }
}
//not used currently
void ACO::depositPheremones(Cell *c)
{
    double pheremones = 0.0;

}

void ACO::findFood(Cell* cell, Entity *e)
{

    curCell = cell;
            std::cout<<"Getting adj cells"<<std::endl;
            getAdjCells(cell->x, cell->y); 
            if(adjCells.size() == 0)
            {
                std::cout << "No adj cells" << std::endl;
                return; // very temporary
            }
            //compare all adjacent squares
            std::vector<std::pair<Cell*, double>> scores;
            for(const auto & ac: adjCells)
            {
                std::cout << "Checking adj cell" << std::endl;
                //Look at distance to goal, difficulty of terrain -> heuristic
                //check pheromones and bias toward following already successful pheromones
                if (visited.find(ac) == visited.end()) {
                    std::cout << "Not visited" << std::endl;
                    if(goals.find(ac) != goals.end())
                    {
                        scores.push_back({ac, pheromoneCalc(ac->data.p.strength, calculateHeuristic(curCell, ac), curCell)*2});
                    }
                    else{
                        scores.push_back({ac, pheromoneCalc(ac->data.p.strength, calculateHeuristic(curCell, ac), curCell)});

                    }
                }
                else{
                    std::cout<<"Already visited"<<std::endl;
                    visited.erase(ac); //can visit after a run
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
                    std::cout << "Chose random cell" << std::endl;
                    //choose the cell S as our next cell
                    visited.insert(s.first);
                    //if s is a goal then start heading back home to reinforce pheremones further
                    double goalImpact = 0.0;
                    if(goals.find(s.first) != visited.end())
                    {
                        //go back home
                        std::cout << "Found goal" << std::endl;
                        s.first->data.entities[0].get()->getHitbox()->setFillColor(sf::Color::Magenta);
                        goalImpact = 0.5 * s.first->data.entities[0].get()->getResource();
                        //return home -> this will reinforce paths even more
                        e->setTarget(base);
                        moveToCell(curCell, s.first, e);


                        break;

                    }



                    //update pheremones for this cell
                    double updatedP = 0.0;
                    updatedP = Q/s.first->data.p.strength * goalImpact;
                    s.first->data.p.strength += updatedP;

                    //move to new cell
                    std::cout << "Moving entity: " << curCell->x << ", " << curCell->y << std::endl;
                    moveToCell(curCell, s.first, e);
                    std::cout << "Moved entity: " << s.first->x << ", " << s.first->y << std::endl;

                    break;

                    
                }
            } 

}
void ACO::returnHome(Cell* cell, Entity * e)
{
    std::cout << "Returning home!" << std::endl;
}
void ACO::getAdjCells(int x, int y)
{
    const int dx[] = {0,0,-1,1};
    const int dy[] = {1,-1,0,0};

    for(auto & ac:adjCells)
    {
       // ac->cs.get()->setFillColor(sf::Color(0,255,0,ac->data.difficulty));
    }
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
    for(auto & ac:adjCells)
    {
      //  ac->cs.get()->setFillColor(sf::Color::Red);
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
    double dist = std::abs(next->x - cur->x) + std::abs(cur->y - next->y);
    double difficulty = next->data.difficulty; // in range [0, 1]
    double heuristic = (1.0 - difficulty) * (dist + 1e-5);
    
    return heuristic;
}
