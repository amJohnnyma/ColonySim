#include "Controller.h"
Controller::Controller(World *world)
: world(world)
{
}

void Controller::selectCells(sf::Vector2i from, sf::Vector2i to, std::vector<std::string> filter)
{
    //simplify edge cases by creating new float with bottom left and top right edges
    int left   = std::min(from.x, to.x);
    int right  = std::max(from.x, to.x);
    int top    = std::min(from.y, to.y); // top = smaller Y
    int bottom = std::max(from.y, to.y); // bottom = larger Y

    sf::Vector2i topLeft(left, top);
    sf::Vector2i bottomRight(right, bottom);
     

    std::ostringstream fromto;
    fromto << "From(" << from.x << ", " << from.y << ") "
           << "To(" << to.x << ", " << to.y << ")";

    std::ostringstream bounds;
    bounds << "TopLeft(" << left << ", " << top << ") "
           << "BottomRight(" << right << ", " << bottom << ")";

    std::cout << fromto.str() << std::endl;
    std::cout << bounds.str() << std::endl;

}
