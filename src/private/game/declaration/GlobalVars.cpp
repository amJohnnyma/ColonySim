#include "../headers/GlobalVars.h"
//put in wrapper to modify vars at runtime
namespace conf
{
    const int cellSize = 50;
    const int antSize = 50;
    double maxPheromone = 0;
    const sf::Vector2f window_size = {1000, 1000};
    const sf::Vector2f window_size_f = static_cast<sf::Vector2f>(window_size);
    const sf::Vector2i worldSize = {16, 16}; // factor of 16 for now (should be how many chunks)
    const uint32_t max_framerate = 60;
    const float dt = 1.0f / static_cast<float>(max_framerate);
    const int numAnts = 10; //per base
    const int numBases = 1; //one base per team
    const int numLocations =1;
    const int antPathDrawLength = 10;
    int Q = 1;
    double pF = 0.5; //pheremone influnce
    double hF = 0.8; // heuristic influence //higher because terrain difficulty lowers it heavily
    double pheremoneEvap = 0.005;
    unsigned int timestep = 500;
    double locationFoodRegenerationRate = 10000;
    float antSpeed = 500.0f;
    float terrainDifficultyScale = 0.05f;
    int baseSeparationDistance = 5;
    int chunkSize = 16;
}