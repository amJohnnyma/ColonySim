#include "GlobalVars.h"
//put in wrapper to modify vars at runtime
namespace conf
{
    const int cellSize = 50;
    const int antSize = 50;
    double maxPheromone = 20;
    const sf::Vector2f window_size = {1000, 1000};
    const sf::Vector2f window_size_f = static_cast<sf::Vector2f>(window_size);
    sf::Vector2i max_world_size = {5, 5}; //in chunks    
    int chunkSize = 16;
    sf::Vector2i worldSize = {16*1, 16*1};//the initial size 
    /*
    16,16 = 1x1
    32,32 = 2x2
    ...
    n,n = (n/chunksize)^2
    
    */
    const uint32_t max_framerate = 60;
    const float dt = 1.0f / static_cast<float>(max_framerate);
    float calcDT = 0;
    const int numAnts = 100; //per base
    const int numBases = 4; //one base per team
    const int numLocations =3;
    const int antPathDrawLength = 10;
    int Q = 1;
    double pF = 0.5; //pheremone influnce
    double hF = 20.8; // heuristic influence //higher because terrain difficulty lowers it heavily
    double pheremoneEvap = 0.005;
    unsigned int timestep = 500;
    double locationFoodRegenerationRate = 1000;
    float antSpeed = 250; //250 / log(simSpeed + 0.1) + 1 -> decent scaling
    float terrainDifficultyScale = 0.05f;
    int baseSeparationDistance = 5;
    float perlinSmoothness = 0.1f;
    unsigned int seed = 0;
    int perlinLayers = 5;
    double perlinFlatness = 0.95f;
    const BiomeData biomeInfo[biomeSize] = {
    {0.35f, false, sf::Color(0, 0, 255), WATER},     // WATER
    {0.4f,  true,  sf::Color(255, 255, 0), BEACH},   // BEACH
    {0.54f, true,  sf::Color(0, 255, 0), FOREST},     // FOREST
    {0.68f, true,  sf::Color(100, 255, 100), JUNGLE},   // JUNGLE
    {1.0f,  false,  sf::Color(150, 75, 0), WOODS}     // WOODS
    };
    int simulationSpeed = 10;
    double baseSpawnChance = 0.001; //0 - 1
    int defaultNumAntsPerBase = 200;
    double numAntsToSpawn = 0.5; //0.5 - 1.5
    double locationSpawnChance = 0.001; //0 - 1
    int numberOfTeams = 16; // >0
    double chosenTeam; //0 - 1 (chosen * num teams = team)
    
}