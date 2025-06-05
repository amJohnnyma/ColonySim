#include "../headers/GlobalVars.h"
//put in wrapper to modify vars at runtime
namespace conf
{
    const int cellSize = 50;
    const int antSize = 50;
    double maxPheromone = 0;
    const sf::Vector2f window_size = {1000, 1000};
    const sf::Vector2f window_size_f = static_cast<sf::Vector2f>(window_size);
    const sf::Vector2i worldSize = {64, 64}; // factor of 16 for now (should be how many chunks) 16,32,48,64,128,256
    /*
    16,16 = 1x1
    32,32 = 2x2
    ...
    n,n = (n/chunksize)^2
    
    */
    const uint32_t max_framerate = 60;
    const float dt = 1.0f / static_cast<float>(max_framerate);
    const int numAnts = 200; //per base
    const int numBases = 10; //one base per team
    const int numLocations =5;
    const int antPathDrawLength = 10;
    int Q = 1;
    double pF = 0.5; //pheremone influnce
    double hF = 0.8; // heuristic influence //higher because terrain difficulty lowers it heavily
    double pheremoneEvap = 0.005;
    unsigned int timestep = 500;
    double locationFoodRegenerationRate = 100;
    float antSpeed = 500.0f;
    float terrainDifficultyScale = 0.05f;
    int baseSeparationDistance = 5;
    int chunkSize = 16;
    float perlinSmoothness = 0.1f;
    unsigned int seed = 12345;
    int perlinLayers = 5;
    double perlinFlatness = 0.95f;
    const BiomeData biomeInfo[biomeSize] = {
    {0.35f, false, sf::Color(0, 0, 255), WATER},     // WATER
    {0.4f,  true,  sf::Color(255, 255, 0), BEACH},   // BEACH
    {0.54f, true,  sf::Color(0, 255, 0), FOREST},     // FOREST
    {0.68f, true,  sf::Color(100, 255, 100), JUNGLE},   // JUNGLE
    {1.0f,  false,  sf::Color(150, 75, 0), WOODS}     // WOODS
};
}