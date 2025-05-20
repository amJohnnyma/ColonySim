#ifndef TRACKED_VARIABLES_H
#define TRACKED_VARIABLES_H

#include "../../structs/worldObjects.h"


class TrackedVariables
{
    private:
        std::pair<int, int> base; //grid coordinates of base
        std::vector<Cell*> world; //The entire world -> might be useful later
        int numAnts;
    public:
        TrackedVariables() {};
        ~TrackedVariables() {};
        void setBase(std::pair<int, int> loc) {base = loc;}
        void setWorld(std::vector<Cell*> w) {world = w;}
        void setNumAnts(int na) {numAnts = na;}
        int getNumAnts() {return numAnts;}
};

#endif
