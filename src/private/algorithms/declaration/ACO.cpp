#include "../includes/ACO.h"

ACO::ACO(Cell* startCell, std::vector<Cell*>& goals)
{
    std::cout<< "Aco construction" << std::endl;
    std::string conns = " ";
    for(auto &i: startCell->data.entities)
    {
        if(i->name == "ant")
        {
            conns += "Ant: (" + std::to_string(i->x) + ", " + std::to_string(i->y) + ")\n";
        }
    }
    for(auto &g:goals)
    {
        for(auto &i: g->data.entities)
        {
            if(i->name == "location")
            {
                conns += "->location: (" + std::to_string(i->x) + ", " + std::to_string(i->y) + ")\n";
            }
        }
    }

    std::cout << conns << std::endl;

}