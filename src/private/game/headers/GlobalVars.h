#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <SFML/Graphics.hpp>

namespace conf
{
    extern const int cellSize;
    extern const int antSize;
    extern double maxPheromone;
    extern const sf::Vector2f window_size;
    extern const sf::Vector2f window_size_f;
    extern const sf::Vector2i worldSize;
    extern const uint32_t max_framerate;
    extern const float dt;
    extern const int numAnts;
    extern const int numBases;
    extern const int numLocations;
    extern const int antPathDrawLength;
    extern int Q ;
    extern double pF ; //pheremone influnce
    extern double hF ; // heuristic influence
    extern double pheremoneEvap ;
    extern unsigned int timestep;
}


#endif // GLOBALVARS_H
