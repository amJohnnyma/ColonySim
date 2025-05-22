#ifndef TRACKED_VARIABLES_H
#define TRACKED_VARIABLES_H

#include "../../structs/worldObjects.h"


class TrackedVariables
{
    private:
        Entity* base; //grid coordinates of base
        const std::vector<Cell*>* world = nullptr; //The entire world -> might be useful later
        int numAnts;
    public:
        TrackedVariables() {};
        ~TrackedVariables() {};
        void setBase(Entity* loc) {base = loc;}
        void setWorld(const std::vector<Cell*>& w) {world = &w;}
        void setNumAnts(int na) {numAnts = na;}
        int getNumAnts() {return numAnts;}
        int getBaseFood() {
    return base->getResource();
}
    double getPF() {return conf::pF;}
    double getHF() { return conf::hF;}

};

#endif
