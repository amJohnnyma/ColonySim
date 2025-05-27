#ifndef BUILDING_LOCATION_H
#define BUILDING_LOCATION_H

#include "Location.h"

class BuildingLocation : public Location
{
    public:
        BuildingLocation(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::RectangleShape> hitbox, Cell* currentCell);
        BuildingLocation(int x, int y, std::string name);
};

#endif