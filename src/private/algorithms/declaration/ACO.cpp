#include "../includes/ACO.h"
#include "../../game/headers/GlobalVars.h"

ACO::ACO(Cell *startCell, std::vector<Cell *> &goals, std::vector<Cell *> &world, int width, int height)
{

    for (auto &cell : world)
    {
        for (auto &e : cell->data.entities)
        {
            if (e)
            {
                if (e.get()->getName() != "default")
                {
                    std::cout << cell->x << ", " << cell->y << ": " << e.get()->getName() << std::endl;
                }
                if (e.get()->getName() == "Base")
                {
                    std::cout << "Ants have base" << std::endl;
                    base = e.get();
                    break;
                }
            }
        }
    }

    for (auto &g : goals)
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
    for (auto &cell : world)
    {
        for(auto &e : cell->data.entities)
        {
                        if (e.get()->getName().find("location") != std::string::npos)
            {
                // Check if e.get() is already in tl
                bool alreadyAdded = std::any_of(tl.begin(), tl.end(), [&](Entity *existing)
                                                { return existing == e.get(); });

                if (!alreadyAdded)
                {
                    tl.push_back(e.get());
                    std::cout << "Added possible location: " << e->getName()
                              << " ( " << e->getX() << ", " << e->getY() << " )" << std::endl;
                }
            }
        }

    }

    for (auto &cell : world)
    {
        for (auto &entity : cell->data.entities)
        {
            //    std::cout << "e" << std::endl;
            if (entity.get()->getName() == "ant")
            {
                getNewTarget(entity.get());
                entity.get()->getPath().push_back(cell);
            }
        }
    }
}

void ACO::getNewTarget(Entity *e)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    //   std::cout << "tl.size(): " << tl.size() << std::endl;
    if (tl.empty())
        return;
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
    // from start cell, look at each entity (ant)
    for (auto &cell : world)
    {
        for (auto &e : cell->data.entities)
        {
            //  std::cout << "Checking entity" << std::endl;
            if (e)
                if (e->getName() == "ant")
                {
                    //   std::cout << "found ant" << std::endl;
                    //   std::cout << "Target: " << e.get()->getTarget()->getName() << std::endl;

                    if (e.get()->getTarget() && e.get()->getTarget()->getName() == "Base")
                    {
                        returnHome(cell, e.get());
                    }
                    else
                    {
                        findFood(cell, e.get());
                    }
                }
        }
        // update pheremones of all cells
        double pLevel = cell->data.p[0].strength;
        double updatedP = (1 - conf::pheremoneEvap) * pLevel + 0.01;
        double pLevel2 = cell->data.p[1].strength;
        double updatedP2 = (1 - conf::pheremoneEvap) * pLevel2 + 0.01;
        cell->data.p[0].strength = updatedP;
        cell->data.p[1].strength = updatedP2;
        /*
            if (cell->data.p.strength > conf::maxPheromone)
            {
                conf::maxPheromone = cell->data.p.strength;
            }
                */
    }
}

void ACO::moveToCell(Cell *from, Cell *to, Entity *e)
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

            // curCell = to;
            break; // Exit the loop after moving the entity
        }
    }
}
// not used currently
void ACO::depositPheremones(Cell *c)
{
    double pheremones = 0.0;
}

void ACO::findFood(Cell *cell, Entity *e)
{

    curCell = cell;
    //   std::cout<<"Getting adj cells"<<std::endl;
    getAdjCells(cell->y, cell->x, e);
    if (adjCells.size() == 0)
    {
        //     std::cout << "No adj cells" << std::endl;
        e->getPath().clear();
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
                    std::cout << ent->getName() << " -> " << ent->getResource() << std::endl;
                    }

                    return hasFound;
                });
            if (containsTarget)
            {
                if (e->getTarget() != base)
                {
                    e->setTarget(base);
                }
                else
                {
                    e->setTarget(target);
                }
            }

            // Update pheromone after choosing the cell
            double updatedP = conf::Q / s.first->data.p[0].strength;
            s.first->data.p[0].strength += updatedP;

            moveToCell(curCell, s.first, e);

            break;
        }
    }
}
void ACO::returnHome(Cell *cell, Entity *e)
{
    //   std::cout << "Returning home!" << std::endl;
    curCell = cell;
    //   std::cout<<"Getting adj cells"<<std::endl;
    getAdjCells(cell->y, cell->x, e);
    if (adjCells.size() == 0)
    {
        //     std::cout << "No adj cells" << std::endl;
        e->getPath().clear();
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
                    bool hasFound = ent.get() == e->getTarget() ||
                    std::find(tl.begin(), tl.end(), ent.get()) != tl.end();
                    if(hasFound)
                    {
                    transferResource(e,ent.get(),10);
                    std::cout << ent->getName() << " -> " << ent->getResource() << std::endl;
                    }

                    return hasFound;
                });
            if (containsTarget)
            {
                if (e->getTarget() != base)
                {
                    e->setTarget(base);
                }
                else
                {
                    e->setTarget(target);
                }
            }

            // Update pheromone after choosing the cell
            double updatedP = conf::Q / s.first->data.p[1].strength;
            s.first->data.p[1].strength += updatedP;

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
            bool inPath = false;
            for (auto &p : e->getPath())
            {
                if (p->x == nx && p->y == ny)
                {
                    inPath = true;
                    break;
                }
            }
            if (!inPath)
            {
                adjCells.push_back(world[index]);
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
    // std::cout << cell->x << ", " << cell->y << " c:t " << target->getX() << ", " << target->getY() << std::endl;
    double Nij = calculateHeuristic(cell, target);
    double Tij;
    
    if(returningHome)
    {
    Tij = cell->data.p[1].strength;

    }
    else{

    Tij = cell->data.p[0].strength;
    }

    double numerator = std::pow(Tij, conf::pF) * std::pow(Nij, conf::hF);
    double denominator = sumOfFeasiblePheremoneProb(target, returningHome);

    // std::cout << numerator / denominator << std::endl;
    return numerator / denominator;
}

double ACO::sumOfFeasiblePheremoneProb(Entity *target, bool returningHome)
{
    double sum = 0.0;
    for (auto &ac : adjCells)
    {
        double heuristic = calculateHeuristic(ac, target);
        double Tij;

        if (returningHome)
        {
            Tij = ac->data.p[1].strength;
        }
        else
        {

            Tij = ac->data.p[0].strength;
        }
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
    double heuristic = 1.0 / (distance + 1.0) /* * (1.0 - 0.5 * difficulty)*/;

    return heuristic;
}

void ACO::transferResource(Entity* from, Entity* to, int amount)
{
    int taken = from->takeResource(amount);
    int excess = to->giveResource(taken);
    from->giveResource(excess); // return unused portion
}
