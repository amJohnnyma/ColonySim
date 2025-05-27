#ifndef LOCATION_H
#define LOCATION_H

#include "Entity.h"

class Location : public Entity
{


    private:
    private:

    public:
        Location(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::RectangleShape> hitbox, Cell* currentCell);
        Location() {}
    public:

    std::vector<Cell*>& getPath() override {        static std::vector<Cell*> dummy; // Safe fallback
        return dummy;};
};

#endif