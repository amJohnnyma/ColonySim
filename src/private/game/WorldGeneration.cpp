#include "WorldGeneration.h"
#include <unordered_set>

#include "ChunkManager.h"
WorldGeneration::WorldGeneration(unsigned int seed, ChunkManager* cm, int cellSize)
{

    if (seed == 0)
    {
        seed = std::random_device{}(); // non-deterministic seed
        std::cout << seed << std::endl;
        conf::seed = seed;
    }

    this->seed = conf::seed;

    perlinNoise = new PerlinNoise(conf::seed);

    this->cellSize = cellSize;
    grid.clear();
    this->cm = cm;
    //std::cout << "Gen t s" << std::endl;
    generateTerrain(); 
    //std::cout << "Gen t d" << std::endl;
    // std::cout << "Gen e s" << std::endl;
    // generateEntities(conf::numAnts,conf::numBases);
    // std::cout << "Gen e d" << std::endl;
    // std::cout << "Gen l s" << std::endl;
    // generateLocations(conf::numLocations);
    // std::cout << "Gen l d" << std::endl;
    // std::cout << "Gen te s" << std::endl;
    // assignTextures();
    // std::cout << "Gen te d" << std::endl;
    logAllEntities();

}

WorldGeneration::~WorldGeneration()
{
    delete perlinNoise;
}

std::unordered_map<std::pair<int, int>, Chunk *, pair_hash> WorldGeneration::getResult() {

    //ask chunk manager for results (i.e. world must ask chunk manager not worldgen)
    std::unordered_map<std::pair<int, int>, Chunk*, pair_hash> result;


    return result;
}

pheromone WorldGeneration::createPheromones(int x, int y)
{
    pheromone p1;
    TeamInfo p = 0;

    for(int i = 1; i <= conf::numBases; i++)
    {
        p = setTeam(p, i);
        p1.pheromoneMap[p] = 1;
    }
    return p1;
}

double WorldGeneration::generateDifficulty()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.001, 1.0);
    return dis(gen);
}

//these will probably end up using some complex texturing else there will be thousands of draw calls
//just using a rectangle with color for now
std::unique_ptr<Rectangle> WorldGeneration::createCellShape(int x, int y, float size)
{
    auto shape = std::make_unique<Rectangle>(x, y, size, size);
    return shape;
}

std::unique_ptr<Cell> WorldGeneration::createCell(int x, int y, float cellSize, float noise)
{
    auto cell = std::make_unique<Cell>();
    BiomeData cellBiome = getBiome(noise);

    CellData cd;
    cd.type = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    //cd.difficulty = std::clamp(generateDifficulty() * 2, 0.5, 1.0);
    cd.difficulty = noise;
    cd.biomeinfo = cellBiome;

    auto pheromones = createPheromones(x, y);
    
    cd.p = pheromones;

    cell->cellShape = createCellShape(x, y, cellSize);
    

    sf::Color biomec = colorFromBiome(cellBiome.biome);
    double val = std::max(0.2, cd.difficulty);
    biomec.a = static_cast<sf::Uint8>(255 * val);
    cell->setColor(biomec);
    cell->x = x;
    cell->y = y;
    cell->data = std::move(cd);

    return cell;
}
void WorldGeneration::createChunk(int chunkX, int chunkY)
{
    std::pair<int, int> key = {chunkX, chunkY};

    auto chunk = std::make_unique<Chunk>(chunkX, chunkY, conf::chunkSize);

        std::mt19937 rng(conf::seed + chunkX * 73856093 + chunkY * 19349663); 
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int x = 0; x < conf::chunkSize; ++x) {
        for (int y = 0; y < conf::chunkSize; ++y) {
            int worldX = chunkX * conf::chunkSize + x;
            int worldY = chunkY * conf::chunkSize + y;

            float nx = worldX * conf::perlinSmoothness;
            float ny = worldY * conf::perlinSmoothness;
            float val = perlinNoise->val(nx, ny);

            auto cell = createCell(worldX, worldY, conf::cellSize, val);

            //colonies
            float baserng = dist(rng);
            int team = static_cast<int>(dist(rng)*conf::numberOfTeams);
            TeamInfo p = 0;
            p = setTeam(p, team);
            if(baserng < conf::baseSpawnChance) 
            {
                //spawn base and the ants in this cell
                auto base = createBase(worldX, worldY, p);
                base->setTeam(p);
                cell.get()->data.entities.push_back(std::move(base));
                for (int k = 0; k < conf::defaultNumAntsPerBase; k++)
                {
                    auto ant = createAnt(worldX, worldY);
                    ant->setTeam(p);
                    cell.get()->data.entities.push_back(std::move(ant));               
                }

                
            }
            float locationrng = dist(rng);
            if(locationrng < conf::locationSpawnChance)
            {
                //spawn a location here
                if (!cell->data.entities.empty() || !cell->data.biomeinfo.passable)
                {
                    continue;
                }
                double difficulty = cell->data.difficulty; // consistent indexing: row major
                auto location = createLocation(worldX,worldY,difficulty);
                cell.get()->data.entities.push_back(std::move(location));
            }
            //spawn buildings
            //spawn whatever else
            chunk->push_back(std::move(cell));
        }
    }
    cm->addChunk(key, {std::move(chunk), state::AVAILABLE}); 
    if (grid.find(key) != grid.end()) return; // Already generated
    grid[key] = chunkCount;    // --> Load these into RAM aswell and when they are in a certain radius load back into this key
    std::cout << "Num chunks: " << chunkCount << " -> Map size: " << grid.size() << std::endl;
    chunkCount++;

}


void WorldGeneration::generateTerrain()
{
    for (int cx = 0; cx < conf::worldSize.x / conf::chunkSize; cx++) {
        for (int cy = 0; cy < conf::worldSize.y / conf::chunkSize; cy++) {
            createChunk(cx,cy);
        }
    }
}


std::unique_ptr<sf::Sprite> WorldGeneration::createAntShape(sf::Color fillColor, int x, int y, float cellSize)
{
    auto& manager = TextureManager::getInstance();
    sf::Texture* antTexture = manager.loadTexture("ant", "src/textures/entities/ants/ant-top-down.png");
    auto shape = std::make_unique<sf::Sprite>(*antTexture);
    shape->setColor(fillColor);
   // std::cout << "Size: " << antTexture->getSize().x << ", " << antTexture->getSize().y << std::endl;
    float scaleX = static_cast<float>(conf::cellSize) / antTexture->getSize().x;
    float scaleY =static_cast<float>(conf::cellSize)/ antTexture->getSize().y;
   // std::cout << "Scale: " << scaleX << ", " << scaleY << std::endl;
    shape->setScale(scaleX, scaleY);

    shape->setPosition(
        (x) * conf::cellSize,
        (y) * conf::cellSize
    );

    return shape;
}

std::unique_ptr<sf::Sprite> WorldGeneration::createBaseShape(sf::Color fillColor, int x, int y, float cellSize)
{
    auto& manager = TextureManager::getInstance();
    sf::Texture* antTexture = manager.loadTexture("base", "src/textures/entities/buildings/base.jpg");
    auto shape = std::make_unique<sf::Sprite>(*antTexture);
    shape->setColor(fillColor);
  //  std::cout << "Size: " << antTexture->getSize().x << ", " << antTexture->getSize().y << std::endl;
    float scaleX = static_cast<float>(conf::cellSize) / antTexture->getSize().x;
    float scaleY =static_cast<float>(conf::cellSize)/ antTexture->getSize().y;
   // std::cout << "Scale: " << scaleX << ", " << scaleY << std::endl;
    shape->setScale(scaleX, scaleY);

    // shape->setOrigin(
    //     antTexture->getSize().x / 2.f,
    //     antTexture->getSize().y / 2.f
    // );

    // Now position it based on grid
    shape->setPosition(
        (x) * conf::cellSize,
        (y) * conf::cellSize
    );

    return shape;
}

//why must world gen make the ants hitbox??
std::unique_ptr<Ant> WorldGeneration::createAnt(int x, int y)
{
    auto shape = createAntShape(sf::Color::White, x, y, cellSize);
    // int chunkX = x / conf::chunkSize;
    // int chunkY = y / conf::chunkSize;

    // Chunk* chunk = nullptr;
    Cell* cell = nullptr;
//     try {
//        // std::cout << "try" << std::endl;
//         chunk = grid[{chunkX, chunkY}].get();
//     } catch (const std::out_of_range& e) {
//         // handle missing chunk
//        // std::cout << "Catch" << std::endl;
//         grid[{chunkX,chunkY}] = std::make_unique<Chunk>(chunkX,chunkY,conf::chunkSize);
//     }

//     if (chunk) {
//       //  std::cout << "Chunk exists" << std::endl;
//         int localX = x % conf::chunkSize;
//         int localY = y % conf::chunkSize;
//         cell = chunk->at(localX, localY);
//         // now you can use the cell pointer
//     }
//   //  std::cout << "Making ant" << std::endl;
    return std::make_unique<Ant>(x, y, "ant", 10, std::move(shape), cell);
}

std::unique_ptr<Location> WorldGeneration::createBase(int x, int y, TeamInfo p)
{
    auto shape = createBaseShape(sf::Color::Yellow, x, y, cellSize);
    int chunkX = x / conf::chunkSize;
    int chunkY = y / conf::chunkSize;

    Chunk* chunk = nullptr;
    Cell* cell = nullptr;
    try {
        //chunk = grid[{chunkX, chunkY}].get();
    } catch (const std::out_of_range& e) {
        // handle missing chunk
    }

    if (chunk) {
        int localX = x % conf::chunkSize;
        int localY = y % conf::chunkSize;
        cell = chunk->at(localX, localY);
        // now you can use the cell pointer
    }
    return std::make_unique<Location>(x, y, "Base:"+std::to_string(p), 100000, std::move(shape), cell);
}

std::unique_ptr<Location> WorldGeneration::createLocation(int x, int y, double difficulty)
{




    auto rs = createLocationShape(x, y, cellSize, difficulty);
    std::string name = "location";

    auto locationEntity = std::make_unique<FoodLocation>(x, y, name, 10000, std::move(rs), nullptr);
    locationEntity->giveResource(100000.0);

    return locationEntity;

}

void WorldGeneration::logAllEntities()
{
    for (int chunkY = 0; chunkY < conf::worldSize.y; ++chunkY)
    {
        for (int chunkX = 0; chunkX < conf::worldSize.x; ++chunkX)
        {
            Chunk* chunk = cm->getChunk(chunkX, chunkY);
            if (!chunk) continue;

            for (int cy = 0; cy < conf::chunkSize; ++cy)
            {
                for (int cx = 0; cx < conf::chunkSize; ++cx)
                {                   

                    auto cell = chunk->at(cx,cy);
                    if (!cell) continue;

                    for (const auto& entityPtr : cell->data.entities)
                    {
                        if (!entityPtr) continue;
                        std::cout << entityPtr->getName() << ", "
                                  << entityPtr->getX() << ", "
                                  << entityPtr->getY() << ", "
                                  << entityPtr->getTeam()
                                  << std::endl;
                    }
                }
            }
        }
    }
}







const std::unordered_map<std::pair<int, int>, std::unique_ptr<Chunk>, pair_hash>& WorldGeneration::getGridRef() const {
    //return grid;
}



int getEdgeBiased(int max) {
    static std::mt19937 rng(conf::seed);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float r = dist(rng);

    // Skew toward edges
    float edgeBias = std::pow(std::abs(r - 0.5f) * 2, 1.5f); // 1.5 = strength of bias
    bool leftSide = (dist(rng) < 0.5f); // flip a coin

    if (leftSide) {
        return static_cast<int>(edgeBias * (max));
    } else {
        return static_cast<int>((1.0f - edgeBias) * (max));
    }
}
void WorldGeneration::generateEntities(int num, int col)
{
    std::vector<std::pair<int, int>> previousLocations;

    for (int b = 1; b <= conf::numBases; b++)
    {
        //std::cout << std::to_string(b) << std::endl;
        int xVal = 0;
        int yVal = 0;

        bool tooClose;
        bool building = false;
        do
        {
            xVal = getEdgeBiased(conf::worldSize.x);
            yVal = getEdgeBiased(conf::worldSize.y);

            //hard lock on where bases can spawn for now
            float difficulty = getDifficulty(xVal, yVal);
            if(difficulty > 0.75f)
            {
                createBuilding(xVal,yVal, "");
                building = true;
                continue;
            } 

            // Check if this (xVal, yVal) is within 10 units of any previous location
            tooClose = std::any_of(previousLocations.begin(), previousLocations.end(),
                                   [&](const std::pair<int, int> &loc)
                                   {
                                       double dist = std::hypot(loc.first - xVal, loc.second - yVal);
                                       return dist < conf::baseSeparationDistance;
                                   });

        } while (tooClose);
        if(building) //building in this spot
        {
            b--;
            continue;
        }
        int localX = xVal % conf::chunkSize;
        int localY = yVal % conf::chunkSize;   
        int chunkX = xVal / conf::chunkSize;
        int chunkY = yVal / conf::chunkSize; 
        // if(grid[{chunkX, chunkY}].get()->at(localX,localY)->data.biomeinfo.biome == WATER)
        // {
        //     b--;
        //     continue;
        // }
        previousLocations.emplace_back(xVal, yVal);
        TeamInfo p = 0;
        p = setTeam(p, b);

            for (int k = 0; k < num; k++)
            {
                std::cout << "Make ant" << std::endl;
                auto ant = createAnt(xVal, yVal);
                ant->setTeam(p);   

             //   grid[{chunkX, chunkY}].get()->at(localX,localY)->data.entities.push_back(std::move(ant));
            }
            std::cout << "Creating base in team: " + std::to_string(p) << std::endl;
            auto base = createBase(xVal, yVal, p);
            base->setTeam(p);
            std::cout << "Made base" << std::endl;
            //grid[{chunkX, chunkY}].get()->at(localX,localY)->data.entities.push_back(std::move(base));

        

            // Logging moved to separate function
    }
            logAllEntities();
}

void WorldGeneration::assignTextures()
{
}

// Helper to create a random engine once
std::mt19937& getRandomEngine() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

// Hash function for pairs so we can use unordered_set
struct pair_hash_vis {
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1,T2>& p) const {
        return std::hash<T1>()(p.first) ^ (std::hash<T2>()(p.second) << 1);
    }
};

// Create rectangle shape for a location
std::unique_ptr<sf::Sprite> WorldGeneration::createLocationShape(int x, int y, float cellSize, double difficulty)
{
    auto& manager = TextureManager::getInstance();
    sf::Texture* antTexture = manager.loadTexture("foodlocation", "src/textures/entities/buildings/foodlocation.png");
    auto shape = std::make_unique<sf::Sprite>(*antTexture);
    shape->setColor(sf::Color::Blue);
 //   std::cout << "Size: " << antTexture->getSize().x << ", " << antTexture->getSize().y << std::endl;
    float scaleX = static_cast<float>(conf::cellSize) / antTexture->getSize().x;
    float scaleY =static_cast<float>(conf::cellSize)/ antTexture->getSize().y;
 //   std::cout << "Scale: " << scaleX << ", " << scaleY << std::endl;
    shape->setScale(scaleX, scaleY);

    // shape->setOrigin(
    //     antTexture->getSize().x / 2.f,
    //     antTexture->getSize().y / 2.f
    // );

    // Now position it based on grid
    shape->setPosition(
        (x) * conf::cellSize,
        (y) * conf::cellSize
    );

    return shape;
}

void WorldGeneration::generateLocations(int num)
{
    std::unordered_set<std::pair<int,int>, pair_hash_vis> visited;

    auto& gen = getRandomEngine();
    std::uniform_int_distribution<> xdist(0, 1);
    std::uniform_int_distribution<> ydist(0, 1);
    int created = 0;
    while (created < num)
    {
        int x = xdist(gen);
        int y = ydist(gen);
     // int x = 5;
     // int y =5;


        // Avoid duplicates
        if (visited.count({x, y}) == 0)
        {

            int chunkX = x / conf::chunkSize;
            int chunkY = y / conf::chunkSize;

            Chunk* chunk = nullptr;
            Cell* cell = nullptr;
            try {
              //  chunk = grid.at({chunkX, chunkY}).get();
            } catch (const std::out_of_range& e) {
                // handle missing chunk
                continue;
            }

            if (chunk) {
                int localX = x % conf::chunkSize;
                int localY = y % conf::chunkSize;
                cell = chunk->at(localX, localY);
                // now you can use the cell pointer
            }
            else{
                continue;
            }

            if(!cell->data.entities.empty() || !cell->data.biomeinfo.passable)
            {
                continue;
            }

            visited.insert({x, y});

            double difficulty = cell->data.difficulty;  // consistent indexing: row major

            auto rs = createLocationShape(x, y, cellSize, difficulty);
            std::string name = "location" + std::to_string(created);

            auto locationEntity = std::make_unique<FoodLocation>(x, y, name, 10000, std::move(rs), cell);
            locationEntity->giveResource(100000.0);

            cell->data.entities.push_back(std::move(locationEntity));

            created++;
        }
        // else retry without incrementing created
    }
}

float WorldGeneration::getDifficulty(int x, int y)
{    
    int localX = x % conf::chunkSize;
    int localY = y % conf::chunkSize;   
    int chunkX = x / conf::chunkSize;
    int chunkY = y / conf::chunkSize; 
  //  double val = static_cast<double>(grid[{chunkX, chunkY}]->at(localX,localY)->data.difficulty);
   // return val;
   return 0.f;
}

void WorldGeneration::createBuilding(int x, int y, std::string type)
{
        int localX = x % conf::chunkSize;
        int localY = y % conf::chunkSize;   
        int chunkX = x / conf::chunkSize;
        int chunkY = y / conf::chunkSize; 

        //if type == building
      //  Cell* cell = cell = grid[{chunkX, chunkY}].get()->at(localX,localY);
      //  if(cell != nullptr && cell->data.entities.empty())  
        // {
        // std::cout << "Building building at " << x << ", " << y << std::endl;
        // auto building = std::make_unique<BuildingLocation>(x, y, "Building");
        // cell->data.entities.push_back(std::unique_ptr<Entity>(building.release()));
        // }     

}

BiomeData WorldGeneration::getBiome(float e)
{
    for (int i = 0; i < conf::biomeSize; ++i)
    {
        if (e < conf::biomeInfo[i].threshold)
            return conf::biomeInfo[i];
    }
    return conf::biomeInfo[conf::biomeSize-1]; // Fallback (should not reach here if thresholds are valid)
}

sf::Color WorldGeneration::colorFromBiome(Biome b)
{
    if (b >= 0 && b < conf::biomeSize)
        return conf::biomeInfo[b].color;

    return sf::Color(10, 10, 10); // Default/fallback
}
