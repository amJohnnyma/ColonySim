#include "../headers/GlobalVars.h"

namespace conf
{
    const int cellSize = 50;
    const int antSize = 50;
    double maxPheromone = 0;
    const sf::Vector2f window_size = {1000, 1000};
    const sf::Vector2f window_size_f = static_cast<sf::Vector2f>(window_size);
    const sf::Vector2i worldSize = {100, 100};
    const uint32_t max_framerate = 60;
    const float dt = 1.0f / static_cast<float>(max_framerate);
    const int numAnts = 1000;
    const int numBases = 1;
    const int numLocations =3;
    const int antPathDrawLength = 20;
    int Q = 1;
    double pF = 1.5; //pheremone influnce
    double hF = 5; // heuristic influence
    double pheremoneEvap = 0.1;
    unsigned int timestep = 500;
}