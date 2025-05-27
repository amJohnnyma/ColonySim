#include "Controller.h"
Controller::Controller(World *world)
: world(world)
{
}


void Controller::clearSelectedCells()
{
       for(auto cell : selectedCells)
       {
              cell->restoreColor();
       }
       selectedCells.clear();
}

/*
        std::ostringstream fromto;
    fromto << "From(" << from.x << ", " << from.y << ") "
           << "To(" << to.x << ", " << to.y << ")";

    std::ostringstream bounds;
    bounds << "TopLeft(" << topLeft.x << ", " << topLeft.y << ") "
           << "BottomRight(" << bottomRight.x << ", " << bottomRight.y << ")";

    std::cout << fromto.str() << std::endl;
    std::cout << bounds.str() << std::endl;
    */

/*
    std::ostringstream fromto;
    fromto << "From(" << from.x << ", " << from.y << ") "
           << "To(" << to.x << ", " << to.y << ")";

    std::ostringstream bounds;
    bounds << "TopLeft(" << left << ", " << top << ") "
           << "BottomRight(" << right << ", " << bottom << ")";

    std::cout << fromto.str() << std::endl;
    std::cout << bounds.str() << std::endl;
    */