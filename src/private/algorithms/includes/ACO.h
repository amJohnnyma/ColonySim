#ifndef ACO_H
#define ACO_H

#include "../../structs/worldObjects.h"
#include <iostream>

class ACO
{

    private:
        Cell* startCell;
        std::vector<Cell*> goals;
        float theta = 0.0f;



        




        
    public:
        ACO(Cell* startCell, std::vector<Cell*> & goals);
        void update();
};

#endif