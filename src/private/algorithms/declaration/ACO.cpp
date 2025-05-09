#include "../includes/ACO.h"
#include "../../game/headers/GlobalVars.h"

ACO::ACO(Cell *startCell, std::vector<Cell *> &goals, std::vector<Cell *> &world, int width, int height) 
{

for(auto &cell : world)
{
    for(auto &e: cell->data.entities)
    {
        if(e)
        {
            if(e.get()->getName() != "default")
            {
                std::cout << cell->x << ", " << cell->y << ": " << e.get()->getName() << std::endl;
            }
            if(e.get()->getName() == "Base")
            {
                std::cout << "Ants have base" << std::endl;
                base = e.get();
            }
        }


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

    assignRandomTarget(goals);


}

ACO::~ACO()
{
}
void ACO::assignRandomTarget(std::vector<Cell *> &raw_goals)
{
    for(auto &e: startCell->data.entities)
    {
        if(e)
        if (e.get()->getName() == "location")
        {
            // Check if e.get() is already in tl
            bool alreadyAdded = std::any_of(tl.begin(), tl.end(), [&](Entity* existing) {
                return existing == e.get();
            });
        
            if (!alreadyAdded)
            {
                tl.push_back(e.get());
                std::cout << "Added possible location: " << e->getName()
                          << " ( " << e->getX() << ", " << e->getY() << " )" << std::endl;
            }
        }

    }

    for(auto &cell : world)
    {
        for(auto &entity : cell->data.entities)
        {
        //    std::cout << "e" << std::endl;
            if(entity.get()->getName() == "ant")
            {
                getNewTarget(entity.get());
                entity.get()->getPath().push_back(cell);
            }

        }
    }

} 

void ACO::getNewTarget(Entity* e)
{
    std::random_device rd;
    std::mt19937 gen(rd());
 //   std::cout << "tl.size(): " << tl.size() << std::endl;
    if (tl.empty()) return;
    std::uniform_int_distribution<> dis(0, tl.size() - 1);
    int randomIndex = dis(gen);
 //   std::cout << "Assigned index: " << randomIndex << std::endl;

    e->setTarget(tl[randomIndex]);
    target = tl[randomIndex];
}

void ACO::update()
{
  //  std::cout << base->getResource() << std::endl;
    bool noBetterF = true;
    double runBest = 0;
 //   std::cout << "Updating aco" << std::endl;
    //from start cell, look at each entity (ant)
    for(auto &cell:world)
    {
    for(auto &e : cell->data.entities)
    {
      //  std::cout << "Checking entity" << std::endl;
        if(e)
        if(e->getName() == "ant")
        {
         //   std::cout << "found ant" << std::endl;  
         //   std::cout << "Target: " << e.get()->getTarget()->getName() << std::endl;
         if(e->getTarget()->getName() == "Base")
         {
         //   std::cout << "Goinghome" << std::endl;
       //  std::cout << "Target: (" << e.get()->getTarget()->getX() << ", " << e.get()->getTarget()->getY() << ")" << std::endl;
         }
         else{
        //    std::cout << "finding food" << std::endl;
         }
          findFood(cell,e.get());
          /*
            if(e.get()->getTarget() && e.get()->getTarget()->getName() == "Base")
            {
                returnHome(cell, e.get());
            }
            else{
                findFood(cell, e.get());
            }    
*/
        }
    }
        //update pheremones of all cells
        double pLevel = cell->data.p.strength;
        double updatedP = (1 - pheremoneEvap) * pLevel + 0.01;
        cell->data.p.strength = updatedP;

    if (cell->data.p.strength > maxPheromone)
    {
        maxPheromone = cell->data.p.strength;
    } 

}



 



    

    

}



void ACO::moveToCell(Cell* from, Cell* to, Entity* e)
{
  //  std::cout << "Moving from: (" << from->x << ", " << from->y << ") to: (" << to->x << ", " << to->y << ")" << std::endl;

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

          //  e->getPath().push_back(to);

          //  curCell = to;

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
         //   std::cout<<"Getting adj cells"<<std::endl;
            getAdjCells(cell->y, cell->x); 
            if(adjCells.size() == 0)
            {
           //     std::cout << "No adj cells" << std::endl;
                return; // very temporary
            }
            //compare all adjacent squares
            std::vector<std::pair<Cell*, double>> scores;
            for(const auto & ac: adjCells)
            {
                double heuristic = calculateHeuristic(ac, e->getTarget());
                double pheromone = ac->data.p.strength;
                double score = std::pow(pheromone, pF) * std::pow(heuristic, hF);
                scores.push_back({ac, score});
                

            }       

            // Normalize scores
            double totalScore = 0.0;
            for (auto& score : scores) totalScore += score.second;

            // Choose a random cell based on the weighted scores
            double randomVal = rand() / (double)RAND_MAX * totalScore;
            double cumulative = 0.0;
          //  std::cout << "Scores count: " << scores.size() << std::endl;
            for(const auto &s : scores)
            {
                cumulative += s.second;
                if(randomVal <= cumulative)
                {
                    e->getPath().push_back(s.first);

                    bool containsTarget = std::any_of(
                        s.first->data.entities.begin(),
                        s.first->data.entities.end(),
                        [&](const std::unique_ptr<Entity>& ent) {
                            return ent.get() == e->getTarget();
                        });
                    if(containsTarget)
                    {
                        for(auto &eg : s.first->data.entities)
                        {
                            if(e->getTarget() != base)
                            {
                                eg.get()->getHitbox()->setFillColor(sf::Color::Magenta);
                                e->setTarget(base);
                            }
                            else{
                                e->setTarget(target);
                            }

                            break;
                        }
                    }

                    // Update pheromone after choosing the cell
                    double updatedP = Q / s.first->data.p.strength;
                    s.first->data.p.strength += updatedP;
        
                    moveToCell(curCell, s.first, e);



                    
                    break;

                }
            } 

}
void ACO::returnHome(Cell* cell, Entity * e)
{
 //   std::cout << "Returning home!" << std::endl;
}
void ACO::getAdjCells(int x, int y)
{
    const int dx[] = {0, 0, -1, 1, -1, -1, 1, 1};
    const int dy[] = {1, -1, 0, 0, -1, 1, -1, 1};


    adjCells.clear();


    for(int i =0; i < 8; i++)
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
double ACO::pheromoneCalc(Cell* cell, Entity* target)
{
   // std::cout << cell->x << ", " << cell->y << " c:t " << target->getX() << ", " << target->getY() << std::endl;
    double Nij = calculateHeuristic(cell, target);
    double Tij = cell->data.p.strength;

    double numerator = std::pow(Tij, pF) * std::pow(Nij, hF);
    double denominator = sumOfFeasiblePheremoneProb(target);

   // std::cout << numerator / denominator << std::endl;
    return numerator / denominator;
}

double ACO::sumOfFeasiblePheremoneProb(Entity *target)
{
    double sum = 0.0;
    for(auto &ac : adjCells)
    {
        double heuristic = calculateHeuristic(ac, target);
        double pheromone = ac->data.p.strength;
     //   std::cout << "H: " << heuristic << " :hF: " << hF<<std::endl;
    //    std::cout << "P: " << pheromone <<" :pF: "<< pF<< std::endl;
        sum += std::pow(heuristic, hF) * std::pow(pheromone, pF);
    }
    //std::cout << "Sum: " << sum << std::endl;
    return sum + 1e-10;
}


double ACO::calculateHeuristic(Cell* next, Entity* target)
{
    double distance = std::abs(next->x - target->getX()) + std::abs(next->y - target->getY()); // Manhattan distance
    double difficulty = next->data.difficulty; // [0, 1], where 1 is most difficult

    // Heuristic: prioritize shorter distance, lightly penalize difficulty
    double heuristic = 1.0 / (distance + 1.0)/* * (1.0 - 0.5 * difficulty)*/;

    return heuristic;
}
