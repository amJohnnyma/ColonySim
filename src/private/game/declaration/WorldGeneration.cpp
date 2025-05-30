#include "../headers/WorldGeneration.h"
#include "WorldGeneration.h"
#include <unordered_set>

WorldGeneration::WorldGeneration(unsigned int seed, int xWidth, int yWidth, int cellSize)
{
    seed = seed;
    width = xWidth;
    height = yWidth;
    this->cellSize = cellSize;
    grid.clear();

    generateTerrain();
    generateEntities(conf::numAnts,conf::numBases);
    assignTextures();
    generateLocations(conf::numLocations);



}

WorldGeneration::~WorldGeneration()
{
}

std::vector<std::unique_ptr<Cell>> WorldGeneration::getResult() {
    return std::move(grid); // Transfer ownership safely
}

std::array<pheromone,2> WorldGeneration::createPheromones(int x, int y)
{
    pheromone p1;

    pheromone p2;

    return {p1, p2};
}

double WorldGeneration::generateDifficulty()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.001, 1.0);
    return dis(gen);
}

std::unique_ptr<Rectangle> WorldGeneration::createCellShape(int x, int y, float size)
{
    auto shape = std::make_unique<Rectangle>(x, y, size, size);
    // Set the fill color with difficulty affecting alpha channel
    return shape;
}

std::unique_ptr<Cell> WorldGeneration::createCell(int x, int y, float cellSize)
{
    auto cell = std::make_unique<Cell>();

    CellData cd;
    cd.type = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
   // cd.difficulty = std::clamp(generateDifficulty() * 2, 0.5, 1.0);
    cd.difficulty = 1;

    auto pheromones = createPheromones(x, y);
    cd.p[0] = pheromones[0];
    cd.p[1] = pheromones[1];

    cell->cellShape = createCellShape(x, y, cellSize);
    cell->setColor(sf::Color(0, 255, 0, static_cast<sf::Uint8>(cd.difficulty * 255)));
    cell->x = x;
    cell->y = y;
    cell->data = std::move(cd);

    return cell;
}
//refactor this so i can just say auto ant = std::unique<ant> instead of the "constructor" being here
std::unique_ptr<sf::Sprite> WorldGeneration::createAntShape(sf::Color fillColor, int x, int y, float cellSize)
{
    auto& manager = TextureManager::getInstance();
    sf::Texture* antTexture = manager.loadTexture("ant", "src/textures/entities/ants/ant-top-down.png");
    auto shape = std::make_unique<sf::Sprite>(*antTexture);
    shape->setColor(fillColor);
    std::cout << "Size: " << antTexture->getSize().x << ", " << antTexture->getSize().y << std::endl;
    float scaleX = static_cast<float>(conf::cellSize) / antTexture->getSize().x;
    float scaleY =static_cast<float>(conf::cellSize)/ antTexture->getSize().y;
    std::cout << "Scale: " << scaleX << ", " << scaleY << std::endl;
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

std::unique_ptr<sf::Sprite> WorldGeneration::createBaseShape(sf::Color fillColor, int x, int y, float cellSize)
{
    auto& manager = TextureManager::getInstance();
    sf::Texture* antTexture = manager.loadTexture("base", "src/textures/entities/buildings/base.jpg");
    auto shape = std::make_unique<sf::Sprite>(*antTexture);
    shape->setColor(fillColor);
    std::cout << "Size: " << antTexture->getSize().x << ", " << antTexture->getSize().y << std::endl;
    float scaleX = static_cast<float>(conf::cellSize) / antTexture->getSize().x;
    float scaleY =static_cast<float>(conf::cellSize)/ antTexture->getSize().y;
    std::cout << "Scale: " << scaleX << ", " << scaleY << std::endl;
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
    Cell* cell = grid[x * width + y].get();
    return std::make_unique<Ant>(y, x, "ant", 10, std::move(shape), cell);
}

std::unique_ptr<Location> WorldGeneration::createBase(int x, int y)
{
    auto shape = createBaseShape(sf::Color::Yellow, x, y, cellSize);
    Cell* cell = grid[x * width + y].get();
    return std::make_unique<Location>(x, y, "Base", 100000, std::move(shape), cell);
}

void WorldGeneration::logAllEntities()
{
    for (auto& cellPtr : grid)
    {
        for (auto& entityPtr : cellPtr->data.entities)
        {
            std::cout << entityPtr->getName() << ", "
                      << entityPtr->getX() << ", "
                      << entityPtr->getY() << ", "
                      << entityPtr->getTeam()
                      << std::endl;
        }
    }
}



void WorldGeneration::generateTerrain()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            grid.push_back(createCell(x, y, cellSize));
        }
    }
}

int getEdgeBiased(int max) {
    static std::mt19937 rng(std::random_device{}());
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
        int xVal = 0;
        int yVal = 0;

        bool tooClose;
        do
        {
            xVal = getEdgeBiased(conf::worldSize.x);
            yVal = getEdgeBiased(conf::worldSize.y);

            // Check if this (xVal, yVal) is within 10 units of any previous location
            tooClose = std::any_of(previousLocations.begin(), previousLocations.end(),
                                   [&](const std::pair<int, int> &loc)
                                   {
                                       double dist = std::hypot(loc.first - xVal, loc.second - yVal);
                                       return dist < conf::baseSeparationDistance;
                                   });

        } while (tooClose);

        previousLocations.emplace_back(xVal, yVal);
        TeamInfo p = 0;
        p = setTeam(p, b);

        for (int i = 0; i < col; i++)
        {
            for (int k = 0; k < num; k++)
            {
                auto ant = createAnt(xVal, yVal);
                ant->setTeam(p);
                grid[xVal * width + yVal]->data.entities.push_back(std::move(ant));
            }

        }
            auto base = createBase(xVal, yVal);
            base->setTeam(p);
            grid[xVal * width + yVal]->data.entities.push_back(std::move(base));

            // Logging moved to separate function
    }
           // logAllEntities();
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
struct pair_hash {
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
    std::cout << "Size: " << antTexture->getSize().x << ", " << antTexture->getSize().y << std::endl;
    float scaleX = static_cast<float>(conf::cellSize) / antTexture->getSize().x;
    float scaleY =static_cast<float>(conf::cellSize)/ antTexture->getSize().y;
    std::cout << "Scale: " << scaleX << ", " << scaleY << std::endl;
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
    std::unordered_set<std::pair<int,int>, pair_hash> visited;

    auto& gen = getRandomEngine();
    std::uniform_int_distribution<> xdist(0, width - 1);
    std::uniform_int_distribution<> ydist(0, height - 1);

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
            visited.insert({x, y});

            double difficulty = grid[x * width + y]->data.difficulty;  // consistent indexing: row major

            auto rs = createLocationShape(x, y, cellSize, difficulty);

            Cell* cell = grid[x * width + y].get();

            std::string name = "location" + std::to_string(created);

            auto locationEntity = std::make_unique<FoodLocation>(x, y, name, 100, std::move(rs), cell);
            locationEntity->giveResource(100.0);

            cell->data.entities.push_back(std::move(locationEntity));

            created++;
        }
        // else retry without incrementing created
    }
}