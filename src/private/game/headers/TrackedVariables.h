#ifndef TRACKED_VARIABLES_H
#define TRACKED_VARIABLES_H

#include "../../structs/worldObjects.h"


class TrackedVariables
{
    private:
        std::vector<Entity*> bases; //grid coordinates of base
        const std::vector<Cell*>* world = nullptr; //The entire world -> might be useful later
        int numAnts;
        std::vector<Cell*> selectedCells;
        bool inBuildingMode = false;
    public:
        TrackedVariables() {};
        ~TrackedVariables() {};
        void setBase(Entity* loc) 
        {
            if(find(bases.begin(), bases.end(), loc) == bases.end())
            {
                bases.push_back(loc);
            }
        }
        void setWorld(const std::vector<Cell*>& w) {world = &w;}
        void setNumAnts(int na) {numAnts = na;}
        int getNumAnts() {return numAnts;}
        int getBaseFood(Entity* tb) {
    return tb->getResource();
}
    void setSelectedCells(std::vector<Cell*> cells)
    {
        selectedCells = cells;
    }
    std::vector<Entity*> getBases() 
    {
        std::cout << "Num bases: " + std::to_string(this->bases.size()) << std::endl;
        return bases;}
    std::vector<Cell*> getSelectedCells() { return selectedCells; }
    double getPF() {return conf::pF;}
    double getHF() { return conf::hF;}
    double isInBuildingMode() {return inBuildingMode;}
    void setBuildingMode(bool val) {inBuildingMode=val;}
    std::string getBasesInfo()
    {
        std::string out = "";
        for(auto& b : bases)
        {
            //this is lazy, just check if it is last then dont add a /n
            out += std::to_string(b->getTeam()) + ": " + std::to_string(b->getResource()) + "\n";
        }
        return out;
    }

};

#endif
