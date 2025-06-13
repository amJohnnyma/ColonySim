#include "ACO.h"
#include "../game/GlobalVars.h"

ACO::ACO(std::vector<Cell*> &goals, World* world,  Entity* base)
{
 //   std::cout << "ACO const" << std::endl;
  //  std::cout << "ACO for team: " + std::to_string(base->getTeam()) << std::endl;
    team = base->getTeam();
    this->base = base;
 //   std::cout << this->base->getTeam() << std::endl;
//    std::cout << "Num goals: " << goals.size() << std::endl;
    for (auto& g : goals) {
    if (!g) {
   //     std::cerr << "[WARN] Null goal pointer encountered, skipping.\n";
        continue;
    }

    this->goals.push_back({g->data.type, {g->x, g->y}});
}

    this->world = world;

    assignRandomTarget(goals);

 //   std::cout << "Fin" << std::endl;
}

ACO::~ACO()
{
}
void ACO::assignRandomTarget(std::vector<Cell *> &raw_goals)
{
  //  std::cout << "Raw goal size: " << std::to_string(raw_goals.size()) << std::endl;

    for (auto& cell : raw_goals)
    {
        if (!cell) continue;

        int x = cell->getX();
        int y = cell->getY();

        Cell* worldCell = world->at(x, y);
        if (!worldCell) continue;

        for (auto& e : worldCell->data.entities)
        {
            if (FoodLocation* loc = dynamic_cast<FoodLocation*>(e.get()))
            {
                // Check if already added based on (x, y)
                bool alreadyAdded = std::any_of(tl.begin(), tl.end(), [&](const std::pair<int, int>& existing) {
                    return existing.first == loc->getX() && existing.second == loc->getY();
                });

                if (!alreadyAdded)
                {
                    tl.emplace_back(loc->getX(), loc->getY());
                    std::cout << "Added possible location: " << loc->getName()
                              << " ( " << loc->getX() << ", " << loc->getY() << " )" << std::endl;
                }
            }
        }
    }

    // Now iterate again for Ant entities and assign targets
    for (int x = 0; x < conf::worldSize.x; x++) {
        for (int y = 0; y < conf::worldSize.y; y++) {
            Cell* cell = world->at(x, y);
            if (!cell) continue;

            for (auto& entity : cell->data.entities) {
                if (Ant* ant = dynamic_cast<Ant*>(entity.get())) {
                    getNewTarget(ant);
                    // ant->getPath().push_back(cell);
                }
            }
        }
    }
    //std::cout << "Construct fin" << std::endl;
}

void ACO::addLocation(Entity *loc)
{
    bool alreadyAdded = std::any_of(tl.begin(), tl.end(), [&](const std::pair<int, int> &existing)
                                    { return existing.first == loc->getX() && existing.second == loc->getY(); });

    if (!alreadyAdded)
    {
        std::cout << "Adding new loc" << std::endl;
        tl.push_back({loc->getX(), loc->getY()});
    }
    else{
        std::cout << "Loc alr added" << std::endl;
    }
}

void ACO::getNewTarget(Ant *ant)
{
   // std::cout << tl.size() << std::endl;
    if (tl.empty())
        return;

    std::vector<std::tuple<float, int, int, Entity*>> scoredTargets;

    for (const auto& loc : tl) {
        Cell* cell = world->at(loc.first, loc.second);
        if (!cell)
            continue;

        for (auto& entity : cell->data.entities) {
            auto* food = dynamic_cast<FoodLocation*>(entity.get());
            if (food) {
                float dx = loc.first - ant->getX();
                float dy = loc.second - ant->getY();
                float distSq = dx * dx + dy * dy;

                float resource = static_cast<float>(food->getResource());

                // Define a scoring function: lower score is better                /
                float score = distSq / (resource + 1.0f); // +1 to avoid divide by zero

                scoredTargets.emplace_back(score, loc.first, loc.second, food);
            }
        }
    }

    // Sort by score (closest & richest food first)
    std::sort(scoredTargets.begin(), scoredTargets.end());
    if (scoredTargets.empty()) {
    std::cout << "[ACO] No viable food locations found for ant at ("
              << ant->getX() << "," << ant->getY() << ")\n";
}

    for (const auto& [score, x, y, food] : scoredTargets) {
        if (food) {
          //  std::cout << "Target: " << x << ", " << y << std::endl;
            ant->setTarget(food);
            possibleLocations = true;
            return;
        }
    }

    // No valid target found, fall back to base
    std::cout << "No target" << std::endl;
    possibleLocations = false;
    ant->setTarget(base);
}

bool same(int x, int y, int w, int v)
{
    //std::cout << "Comparing (" << x << "," << y << ") with (" << w << "," << v << ")" << std::endl;
    return (x == w) && (y==v);
}

void ACO::update()
{

    //  std::cout << "Updating aco" << std::endl;
    // from start cell, look at each entity (ant)
    //would be better to store a list of ants
    for (int x = 0; x < conf::worldSize.x; x++)
    {
        for (int y = 0; y < conf::worldSize.y; y++)
        {
          //  std::cout << "Updating:" << x << ", " << y << std::endl;
            Cell *cell = world->at(x, y);
          //  std::cout << "Updating P2:" << x << ", " << y << std::endl;
            if(cell)
            for (auto &e : cell->data.entities)
            {
           // std::cout << "Updating P3:" << x << ", " << y << std::endl;
                //  std::cout << "Checking entity" << std::endl;
                if(!e)
                {
                  //  std::cerr << "No ent" << std::endl;
                    continue;

                }
                    if (Ant *ant = dynamic_cast<Ant *>(e.get()))
                    {
                        if(!ant)
                        {
                          //  std::cerr << "Not ant" << std::endl;
                            continue;

                        }


                        if (!ant->sameTeam(ant->getTeam(), team))
                        {
                            continue;
                        }
                        if(ant->stillAnimating())
                        {
                            continue;
                        }
                        if(!ant->getTarget())
                        {
                          //  std::cerr << "No target" << std::endl;
                            getNewTarget(ant);
                            continue;
                        }
 
                        // std::cout << "found ant" << std::endl;
                        // std::cout << "Target name: " << ant->getTarget()->getName() << std::endl;
                        // std::cout << "target pos: " << ant->getTarget()->getX() <<", " << ant->getTarget()->getY() << std::endl;
                        // std::cout << "Current pos: " << ant->getX() << ", " << ant->getY() << std::endl;

                        if (ant->getTarget()->getResource() <= 0 && ant->getTarget() != base)
                        {
                            getNewTarget(ant);
                        }
                        if (ant->getTarget() && ant->getTarget()->getName() == "Base:" + std::to_string(ant->getTeam()))
                        {
                            //std::cout << "Base found" << std::endl;
                           // std::cout << ant->getTeam() << ", " << ant->getTarget()->getTeam() << std::endl;
                            if (!ant->sameTeam(ant->getTeam(), ant->getTarget()->getTeam()) || ant->getTeam() == ant->getTarget()->getTeam())
                            {
                               // std::cout << "Getting new target" << std::endl;
                                getNewTarget(ant);
                                continue;
                            }
                            if ((!possibleLocations) && (e.get()->getX() == ant->getTarget()->getX()) && (e.get()->getY() == ant->getTarget()->getY()))
                            {
                                getNewTarget(ant);
                                continue;
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
                    else if (FoodLocation *fl = dynamic_cast<FoodLocation *>(e.get()))
                    {
                        fl->regenerate();
                        // e.get()->giveResource(1);
                        //   std::cout << e.get()->getName() << " : " << e.get()->getResource() << std::endl;
                    }
                
            }

            // update pheromone
            cell->data.p.pheromoneMap[team] *= (1.0 - conf::pheremoneEvap);
        }
    }
}

void ACO::moveToCell(std::pair<int, int> from, std::pair<int, int> to, Entity *e)
{
    Cell* fromCell = world->at(from.first, from.second);
    Cell* toCell = world->at(to.first, to.second);

    if (!fromCell || !toCell) {
        std::cout << "Invalid from or to cell coordinates!" << std::endl;
        return;
    }

   // std::cout << "Moving entity '" << e->getName() << "' from (" << from.first << ", " << from.second << ") to (" << to.first << ", " << to.second << ")" << std::endl;

   // std::cout << "Entities in from cell: " << fromCell->data.entities.size() << std::endl;
    for (auto it = fromCell->data.entities.begin(); it != fromCell->data.entities.end(); ++it)
    {
       // std::cout << "Entity name in from cell: " << it->get()->getName() << std::endl;

        if (it->get() == e)
        {
           // std::cout << "Found matching entity in from cell, moving it." << std::endl;

            // Move ownership of entity to 'to' cell
            toCell->data.entities.push_back(std::move(*it));
            fromCell->data.entities.erase(it);


            if (Ant* ant = dynamic_cast<Ant*>(e)) {
                float toX = to.first * conf::cellSize;
                float toY = to.second * conf::cellSize;

                ant->startMovingTo(toX, toY);
            }
            return;
        }
    }

   // std::cout << "Entity to move was NOT found in the from cell's entities!" << std::endl;
}
void ACO::findFood(Cell *cell, Ant *e)
{

   // std::cout << "Finding food" << std::endl;
    curCell = {cell->x, cell->y};
    //   std::cout<<"Getting adj cells"<<std::endl;
    getAdjCells(cell->x, cell->y);
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
        double score = pheromoneCalc(world->at(ac.first, ac.second), e->getTarget());
        scores.push_back({world->at(ac.first,ac.second), score});
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

            bool containsTarget = std::find(tl.begin(), tl.end(), std::make_pair(s.first->x, s.first->y)) != tl.end();
        //    std::cout << "ANT( " << e->getX() << ", " << e->getY() << ")" << std::endl;
         //   std::cout << "TARGET( " << s.first->x << ", " <<s.first->y << ")" << std::endl;

            if (containsTarget)
            {
                Cell* targetLocation = world->at(s.first->x, s.first->y);
                for(auto& ent:targetLocation->data.entities)
                {
                    if(FoodLocation* fl = dynamic_cast<FoodLocation*>(ent.get()))
                    {
                        e->giveResource(fl->takeResource(10));
                    }
                }
            //   std::cout << "Target base -----------------------------------------------------------------------------" << base->getTeam() << std::endl;
                e->setTarget(base);
            }

            // Update pheromone after choosing the cell
            double currentP = s.first->data.p.pheromoneMap[team];
            double updatedP = conf::Q / (currentP + 0.0001);  // or your preferred epsilon
            s.first->data.p.pheromoneMap[team] += updatedP;


            moveToCell({cell->x, cell->y}, {s.first->x, s.first->y}, e);

            break;
        }
    }
}
void ACO::returnHome(Cell *cell, Ant *e)
{
    //   std::cout << "Returning home!" << std::endl;
    curCell = {cell->x,cell->y};
    //   std::cout<<"Getting adj cells"<<std::endl;
    getAdjCells(cell->x, cell->y);
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
        double score = pheromoneCalc(world->at(ac.first, ac.second), e->getTarget());
        scores.push_back({world->at(ac.first,ac.second), score});
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

            bool containsTarget = same(s.first->x, s.first->y, base->getX(), base->getY());
        //    std::cout << "ANT( " << e->getX() << ", " << e->getY() << ")" << std::endl;
          //  std::cout << "TARGET( " << s.first->x << ", " <<s.first->y << ")" << std::endl;
           // std::cout << "BASE( " << base->getX() << ", " << base->getY() << ")" << std::endl;

            if (containsTarget)
            {
             //   std::cout << "Target rand -----------------------------------------------------------------------------" << base->getTeam() << std::endl;
                
                std::cout << "e:"<< e->getTeam() << " b:" << base->getTeam() << " aco:" << team << std::endl;
                if (!(e->getTeam() == base->getTeam() && base->getTeam() == team)) {
                    exit(0);
                }
                base->giveResource(e->takeResource(10));
                getNewTarget(e);
            }
            // Update pheromone after choosing the cell
            double currentP = s.first->data.p.pheromoneMap[team];
            double updatedP = conf::Q / (currentP + 0.0001);  // or your preferred epsilon
            s.first->data.p.pheromoneMap[team] += updatedP;



            moveToCell({cell->x, cell->y}, {s.first->x, s.first->y}, e);
            break;
        }
    }
}
bool ACO::isInBounds(int x, int y) const {
    return x >= 0 && x < conf::worldSize.x && y >= 0 && y < conf::worldSize.y;
}

bool ACO::isCellBlocked(Cell* tile) const {
    return std::any_of(tile->data.entities.begin(), tile->data.entities.end(),
        [](const auto& b) {
            return dynamic_cast<BuildingLocation*>(b.get()) != nullptr;
        });
}

void ACO::handleEnemiesInCell(Cell* tile, Entity* e) {
    for (auto it = tile->data.entities.begin(); it != tile->data.entities.end();)
    {
        if (Ant* ant = dynamic_cast<Ant*>(it->get()))
        {
            if (!e->sameTeam(ant->getTeam(), e->getTeam()))
            {
                Ant* thisAnt = dynamic_cast<Ant*>(e);
                if (!thisAnt) break;

              //  std::cout << "Attack time" << std::endl;
                double result = thisAnt->attack(ant);

                if (result <= 0)
                {                    
                    it = tile->data.entities.erase(it);
                }
                else
                {
                    ++it;
                }
                break; // attack one per turn
            }
            else
            {
                ++it;
            }
        }
        else
        {
            ++it;
        }
    }
}

void ACO::getAdjCells(int x, int y) {
    const int dx[] = {0, 0, -1, 1, -1, -1, 1, 1};
    const int dy[] = {1, -1, 0, 0, -1, 1, -1, 1};

    adjCells.clear();

    for (int i = 0; i < 8; ++i)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (!isInBounds(nx, ny) || !world->at(nx,ny)->data.biomeinfo.passable) {
            //std::cout << "Not in bound" << std::endl;
            continue;
        } 


        //handleEnemiesInCell(world->at(ny,nx), e);

        if (!isCellBlocked(world->at(nx,ny)))
        {        
            adjCells.push_back({nx,ny});
        }
    }

    // for(auto& [x,y]:adjCells)
    // {
    //     std::cout << std::to_string(x) << ", " << std::to_string(y) << std::endl;
    // }
}


/*
• τij (t) is the pheromone value on edge (i, j) at time t.
• ηij is the heuristic value (often the inverse of the distance) for edge (i, j).
• α is a parameter that controls the influence of the pheromone trail.
• β is a parameter that controls the influence of the heuristic value.
• Ni is the set of feasible nodes that can be visited from node i.
*/
double ACO::pheromoneCalc(Cell *cell, Entity *target)
{
    // std::cout << cell->x << ", " << cell->y << " c:t " << target->getX() << ", " << target->getY() << std::endl;
    double Nij = calculateHeuristic(cell, target);
    double Tij;


    Tij = std::clamp(cell->data.p.pheromoneMap[team], 0.01, 100.0);
   // std::cout << "Tij: " + std::to_string(Tij) << std::endl;

    double numerator = std::pow(Tij, conf::pF) * std::pow(Nij , conf::hF);
    double denominator = sumOfFeasiblePheremoneProb(target) + 0.00001;

    // std::cout << numerator / denominator << std::endl;
    return numerator / denominator;
}

double ACO::sumOfFeasiblePheremoneProb(Entity *target)
{

    double sum = 0.0;
    for (auto &ac : adjCells)
    {
        double heuristic = calculateHeuristic(world->at(ac.first,ac.second), target);
        double Tij;

        Tij = std::clamp(world->at(ac.first,ac.second)->data.p.pheromoneMap[team], 0.01, 100.0);

        //   std::cout << "H: " << heuristic << " :hF: " << hF<<std::endl;
        //    std::cout << "P: " << pheromone <<" :pF: "<< pF<< std::endl;
        sum += std::pow(heuristic, conf::hF) * std::pow(Tij, conf::pF);
    }
    // std::cout << "Sum: " << sum << std::endl;
    return sum + 1e-10;
}

double ACO::calculateHeuristic(Cell *next, Entity *target)
{
    double distance = std::abs(next->x - target->getX()) + std::abs(next->y - target->getY()); // Manhattan distance
    double difficulty = next->data.difficulty;                                                 // [0, 1], where 1 is most difficult

    // Heuristic: prioritize shorter distance, lightly penalize difficulty
    //double heuristic = 1.0 / (distance + 1.0 + conf::terrainDifficultyScale * difficulty);
    double heuristic = 1.0 / (distance + 1.0 + conf::terrainDifficultyScale * difficulty);
    heuristic = std::clamp(heuristic, 0.001, 100.0);

    return heuristic;
}

void ACO::transferResource(Entity* from, Entity* to, int amount)
{
    int taken = from->takeResource(amount);
    int excess = to->giveResource(taken);
    from->giveResource(excess); // return unused portion
}
