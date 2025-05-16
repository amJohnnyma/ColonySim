#ifndef FOODLOCATION_H
#define FOODLOCATION_H

#include "Location.h"

class FoodLocation : public Location{

    private:

    public:
        FoodLocation(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::RectangleShape> hitbox, Cell* currentCell);
    public:
        void regenerate() { giveResource(conf::locationFoodRegenerationRate * this->maxResource);}

};

#endif  