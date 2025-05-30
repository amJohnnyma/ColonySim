#include "../includes/ACO.h"
#include "../../game/headers/GlobalVars.h"

ACO::ACO(Cell *startCell, std::vector<Cell *> &goals, std::vector<Cell *> &world, int width, int height, Entity* base)
{
std::cout << "ACO const" << std::endl;

    std::cout << "ACO for team: " + std::to_string(base->getTeam()) << std::endl;
    team = base->getTeam();
    this->base = base;
    std::cout << this->base->getTeam() << std::endl;
    for (auto &g : goals)
    {
        this->goals.insert(g);
    }

    this->world = world;
    worldWidth = width;
    worldHeight = height;
    curCell = startCell;

    assignRandomTarget(goals);

    std::cout << "Fin" << std::endl;
}

ACO::~ACO()
{
}
void ACO::assignRandomTarget(std::vector<Cell *> &raw_goals)
{
    for (auto &cell : world)
    {
        for(auto &e : cell->data.entities)
        {
            if (FoodLocation* loc = dynamic_cast<FoodLocation*>(e.get()))
            {
                // Check if e.get() is already in tl
                bool alreadyAdded = std::any_of(tl.begin(), tl.end(), [&](Entity *existing)
                    { return existing == loc; });

                if (!alreadyAdded)
                {
                    tl.push_back(loc);
                    std::cout << "Added possible location: " << loc->getName()
                              << " ( " << loc->getX() << ", " << loc->getY() << " )" << std::endl;
                }
            }
        }

    }

    for (auto &cell : world)
    {
        for (auto &entity : cell->data.entities)
        {
               // std::cout << "e" << std::endl;
            if (Ant* ant = dynamic_cast<Ant*>(entity.get()))
            {
               // std::cout << "Ant target get" << std::endl;
                getNewTarget(ant);
                //ant->getPath().push_back(cell);
            }
        }
    }
    //std::cout << "Construct fin" << std::endl;
}

void ACO::getNewTarget(Ant *ant)
{

    //when assigning a target dont change it if not needed (still has food)

    std::random_device rd;
    std::mt19937 gen(rd());
    
     //  std::cout << "tl.size(): " << tl.size() << std::endl;
    if (tl.empty())
        return;
    std::uniform_int_distribution<> dis(0, tl.size() - 1);
    int randomIndex = dis(gen);
    int counter = 0;
    while(tl[randomIndex]->getResource() <= 0 && counter < tl.size())
    {
        if(randomIndex == tl.size() - 1)
        {
            randomIndex =0;
        }
        else{
            randomIndex++;
        }

        counter++;

    }
      // std::cout << "Assigned index: " << randomIndex << std::endl;
    
    if(counter < tl.size())
    {
        //std::cout << "Setting" << std::endl;
        ant->setTarget(tl[randomIndex]);
        possibleLocations = true;
       // std::cout << "done set" << std::endl;
    }
    else{
       // std::cout << "No more locations with food" << std::endl;
        possibleLocations = false;
        ant->setTarget(base);
    }
    
   // target = tl[randomIndex];
}

void ACO::update()
{
    /////////////////testing
    /*
    int baseAnts = 0;
    int foodAnts = 0;
    for (auto &cell : world)
    {
        for (auto &e : cell->data.entities)
        {
            if (e)
            {
                if (Ant *ant = dynamic_cast<Ant *>(e.get()))
                {
                    if(ant->getTarget()->getName() == "Base")
                    {
                        baseAnts++;
                    }
                    else{
                        foodAnts++;
                    }
                }
            }
        }
    }
    std::cout << "Base: " << std::to_string(baseAnts) << "\t Food: " << std::to_string(foodAnts) << std::endl;
    */
/////////////////////

    //  std::cout << base->getResource() << std::endl;
    bool noBetterF = true;
    double runBest = 0;
     //  std::cout << "Updating aco" << std::endl;
    // from start cell, look at each entity (ant)
    for (auto &cell : world)
    {
        for (auto &e : cell->data.entities)
        {
            //  std::cout << "Checking entity" << std::endl;
            if (e)
            {
                if (Ant* ant = dynamic_cast<Ant*>(e.get()))
                {    
                   // std::cout << "Ant team: " << ant->getTeam() << ", Target: " << ant->getTarget()->getName() << "\n";

                    if(!ant->sameTeam(ant->getTeam(), team))   
                    {
                        break;
                    }             
                    if(ant->stillAnimating())
                    {
                        break;
                    }
                    //   std::cout << "found ant" << std::endl;
                     //  std::cout << "Target: " << ant->getTarget()->getName() << std::endl;
                    if(ant->getTarget()->getResource() <= 0 && ant->getTarget() != base)
                    {
                        getNewTarget(ant);
                    }                    
                    if (ant->getTarget() && ant->getTarget()->getName() == "Base:" + std::to_string(ant->getTeam()))
                    {                        
                        if(!ant->sameTeam(ant->getTeam(), ant->getTarget()->getTeam()))
                        {
                            getNewTarget(ant);
                            break;
                        }
                        if((!possibleLocations) && (e.get()->getX() == ant->getTarget()->getX())&& (e.get()->getY() == ant->getTarget()->getY()))
                        {
                            getNewTarget(ant);
                            break;
                        }
                       // std::cout <<std::to_string(ant->getTeam()) +  "Returning home" << std::endl;
                        returnHome(cell, ant);
                    }
                    else
                    {
                       // std::cout << std::to_string(ant->getTeam()) + " Finding food" << std::endl;
                        findFood(cell, ant);
                    }
                } 
                else if(FoodLocation* fl = dynamic_cast<FoodLocation*>(e.get()))
                {
                   fl->regenerate();
                 // e.get()->giveResource(1);
                 //   std::cout << e.get()->getName() << " : " << e.get()->getResource() << std::endl;
                }
            }

        }
        for (int i = 0; i < 2; ++i) {
            for (auto& [key, strength] : cell->data.p[i].pheromoneMap) {
                double bonus = (i == 0) ? 0.02 : 0.1;  // home vs base bonus
                strength = (1 - conf::pheremoneEvap) * strength + bonus;
            }
        }

    }
}

void ACO::moveToCell(Cell *from, Cell *to, Entity *e)
{
    for (auto it = from->data.entities.begin(); it != from->data.entities.end(); ++it)
    {
        if (it->get() == e)
        {
            // Move ownership of entity to 'to' cell
            to->data.entities.push_back(std::move(*it));
            from->data.entities.erase(it);

            if (Ant* ant = dynamic_cast<Ant*>(e)) {
                float toX = to->x * conf::cellSize;  // center of the cell
                float toY = to->y * conf::cellSize;

                ant->startMovingTo(toX, toY);

             //   float fromX = ant->getHitbox()->getPosition().x;
             //   float fromY = ant->getHitbox()->getPosition().y;
            //    float angleDeg = std::atan2(toY - fromY, toX - fromX) * 180.f / M_PI;
             //   ant->setRotation(angleDeg);
            }
            break;
        }
    }
}
// not used currently
void ACO::depositPheremones(Cell *c)
{
    double pheremones = 0.0;
}

void ACO::findFood(Cell *cell, Ant *e)
{

    curCell = cell;
    //   std::cout<<"Getting adj cells"<<std::endl;
    getAdjCells(cell->y, cell->x, e);
    if (adjCells.size() == 0)
    {
        //     std::cout << "No adj cells" << std::endl;
     //   e->getPath().clear();
        return; // very temporary
    }
    // compare all adjacent squares
    std::vector<std::pair<Cell *, double>> scores;
    for (const auto &ac : adjCells)
    {
        double score = pheromoneCalc(ac, e->getTarget(), false);
        scores.push_back({ac, score});
    }

    // Normalize scores
    double totalScore = 0.0;
    for (auto &score : scores)
        totalScore += score.second;

    // Choose a random cell based on the weighted scores
    double randomVal = rand() / (double)RAND_MAX * totalScore;
    double cumulative = 0.0;
    //  std::cout << "Scores count: " << scores.size() << std::endl;
    for (const auto &s : scores)
    {
        cumulative += s.second;
        if (randomVal <= cumulative)
        {
            // e->getPath().push_back(s.first);
            e->addPath(s.first);

            bool containsTarget = std::any_of(
                s.first->data.entities.begin(),
                s.first->data.entities.end(),
                [&](const std::unique_ptr<Entity> &ent)
                {
                    bool hasFound = ent.get() == e->getTarget() ||
                    std::find(tl.begin(), tl.end(), ent.get()) != tl.end();

                    if(hasFound)
                    {
                    transferResource(ent.get(),e,10);
                 //   std::cout << ent->getName() << " -> " << ent->getResource() << std::endl;
                    }

                    return hasFound;
                });
            if (containsTarget)
            {
               // std::cout << "Target base " << base->getTeam() << std::endl;
                e->setTarget(base);
            }

            int team = getTeam(e->getTeam());
            // Update pheromone after choosing the cell
            double updatedP = conf::Q / s.first->data.p[0].pheromoneMap[team];
            s.first->data.p[0].pheromoneMap[team] += updatedP;

            moveToCell(curCell, s.first, e);

            break;
        }
    }
}
void ACO::returnHome(Cell *cell, Ant *e)
{
    //   std::cout << "Returning home!" << std::endl;
    curCell = cell;
    //   std::cout<<"Getting adj cells"<<std::endl;
    getAdjCells(cell->y, cell->x, e);
    if (adjCells.size() == 0)
    {
        //     std::cout << "No adj cells" << std::endl;
     //   e->getPath().clear();
        return; // very temporary
    }
    // compare all adjacent squares
    std::vector<std::pair<Cell *, double>> scores;
    for (const auto &ac : adjCells)
    {
        double score = pheromoneCalc(ac, e->getTarget(), true);
        scores.push_back({ac, score});
    }

    // Normalize scores
    double totalScore = 0.0;
    for (auto &score : scores)
        totalScore += score.second;

    // Choose a random cell based on the weighted scores
    double randomVal = rand() / (double)RAND_MAX * totalScore;
    double cumulative = 0.0;
    //  std::cout << "Scores count: " << scores.size() << std::endl;
    for (const auto &s : scores)
    {
        cumulative += s.second;
        if (randomVal <= cumulative)
        {
            // e->getPath().push_back(s.first);
            e->addPath(s.first);

            bool containsTarget = std::any_of(
                s.first->data.entities.begin(),
                s.first->data.entities.end(),
                [&](const std::unique_ptr<Entity> &ent)
                {
                    bool hasFound = ent.get() == e->getTarget();
                    if(hasFound)
                    {
                    transferResource(e,ent.get(),e->getResource());
                    //std::cout << ent->getName() << " -> " << ent->getResource() << std::endl;
                    }

                    return hasFound;
                });
            if (containsTarget)
            {
                getNewTarget(e);
            }

            //for now...
            int team = getTeam(e->getTeam());
            // Update pheromone after choosing the cell
            double updatedP = conf::Q / s.first->data.p[0].pheromoneMap[team];
            s.first->data.p[0].pheromoneMap[team] += updatedP;


            moveToCell(curCell, s.first, e);

            break;
        }
    }
}
void ACO::getAdjCells(int x, int y, Entity *e)
{
    const int dx[] = {0, 0, -1, 1, -1, -1, 1, 1};
    const int dy[] = {1, -1, 0, 0, -1, 1, -1, 1};

    adjCells.clear();

    for (int i = 0; i < 8; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        // Check bounds
        if (nx >= 0 && nx < worldWidth && ny >= 0 && ny < worldHeight)
        {
            int index = ny * worldWidth + nx;
            auto& tile = world[index];

            // Check if already in path
            bool inPath = std::any_of(e->getPath().begin(), e->getPath().end(), [&](const auto& p) {
                return p->x == nx && p->y == ny;
            });

            if (inPath) continue;
            
            // Check for blocking buildings
            bool notBlocked = std::none_of(world[nx * worldWidth + ny]->data.entities.begin(), world[nx * worldWidth + ny]->data.entities.end(),
                [](const auto& b) {
                    return dynamic_cast<BuildingLocation*>(b.get()) != nullptr;
                });

            if (notBlocked)
            {
                adjCells.push_back(tile);
            }
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
double ACO::pheromoneCalc(Cell *cell, Entity *target, bool returningHome)
{
    int team = getTeam(target->getTeam());
    // std::cout << cell->x << ", " << cell->y << " c:t " << target->getX() << ", " << target->getY() << std::endl;
    double Nij = calculateHeuristic(cell, target);
    double Tij;
    double homeFac = 1;
    
    if(returningHome)
    {
    Tij = cell->data.p[1].pheromoneMap[team];
    homeFac = 2;

    }
    else{

    Tij = cell->data.p[0].pheromoneMap[team];
    }

    double numerator = std::pow(Tij, conf::pF) * std::pow(Nij * homeFac, conf::hF);
    double denominator = sumOfFeasiblePheremoneProb(target, returningHome);

    // std::cout << numerator / denominator << std::endl;
    return numerator / denominator;
}

double ACO::sumOfFeasiblePheremoneProb(Entity *target, bool returningHome)
{
    int team = getTeam(target->getTeam());

    double sum = 0.0;
    for (auto &ac : adjCells)
    {
        double heuristic = calculateHeuristic(ac, target);
        double Tij;
        double homeFac = 1;

        if (returningHome)
        {
            Tij = ac->data.p[1].pheromoneMap[team];
            homeFac = 2;
        }
        else
        {
            Tij = ac->data.p[0].pheromoneMap[team];
        }
        //   std::cout << "H: " << heuristic << " :hF: " << hF<<std::endl;
        //    std::cout << "P: " << pheromone <<" :pF: "<< pF<< std::endl;
        sum += std::pow(heuristic * homeFac, conf::hF) * std::pow(Tij, conf::pF);
    }
    // std::cout << "Sum: " << sum << std::endl;
    return sum + 1e-10;
}

double ACO::calculateHeuristic(Cell *next, Entity *target)
{
    double distance = std::abs(next->x - target->getX()) + std::abs(next->y - target->getY()); // Manhattan distance
    double difficulty = next->data.difficulty;                                                 // [0, 1], where 1 is most difficult

    // Heuristic: prioritize shorter distance, lightly penalize difficulty
    double heuristic = 1.0 / (distance + 1.0 + conf::terrainDifficultyScale * difficulty);

    return heuristic;
}

void ACO::transferResource(Entity* from, Entity* to, int amount)
{
    int taken = from->takeResource(amount);
    int excess = to->giveResource(taken);
    from->giveResource(excess); // return unused portion
}
