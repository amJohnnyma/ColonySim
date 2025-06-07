#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H


#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "../entities/Entity.h"
#include "../shapes/Shape.h"
#include "biomDef.h"

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>()(p.first);
        auto h2 = std::hash<T2>()(p.second);
        return h1 ^ (h2 << 1);
    }
};
struct GridObject
{
    virtual ~GridObject() = default;
};

struct perTeamPheromone
{

};

//make a class
struct pheromone
{

    std::map<int, double> pheromoneMap;

};


struct CellData
{
    std::string type = "Default";
    std::string terrain ="flat";
    double difficulty = 0;
    pheromone p; 
    std::vector<std::unique_ptr<Entity>> entities;
    BiomeData biomeinfo;

        // Move-only
    CellData() = default;
    CellData(CellData&&) = default;
    CellData& operator=(CellData&&) = default;

    // Disable copy
    CellData(const CellData&) = delete;
    CellData& operator=(const CellData&) = delete;
};






#endif