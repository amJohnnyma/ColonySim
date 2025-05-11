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
}


#endif // GLOBALVARS_H
