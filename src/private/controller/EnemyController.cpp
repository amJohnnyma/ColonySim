#include "EnemyController.h"

EnemyController::EnemyController(World *world,sf::Color overlay)
: Controller(world)
{
    this->selectOverlay = overlay;
}
void EnemyController::update() {
}
void EnemyController::selectCells(sf::Vector2i from, sf::Vector2i to, std::vector<std::string> filter)
{
       // simplify edge cases by creating new float with bottom left and top right edges
       int left = std::min(from.x, to.x);
       int right = std::max(from.x, to.x);
       int top = std::min(from.y, to.y);    // top = smaller Y
       int bottom = std::max(from.y, to.y); // bottom = larger Y

       //  sf::Vector2i topLeft(left, top);
       //  sf::Vector2i bottomRight(right, bottom);

       int cellsize = conf::cellSize;

       // grid position (get closest grid according to screen space and then go to grid space)
       sf::Vector2i topLeft;
       topLeft.x = std::max((left / cellsize) * cellsize, 0) / cellsize; 
       topLeft.y = std::max((top / cellsize) * cellsize, 0) / cellsize;

       sf::Vector2i bottomRight;
       bottomRight.x = std::min(((right + cellsize - 1) / cellsize) * cellsize, conf::worldSize.x * cellsize) / cellsize;
       bottomRight.y = std::min(((bottom + cellsize - 1) / cellsize) * cellsize, conf::worldSize.y * cellsize) / cellsize;
    //     std::ostringstream fromto;
    // fromto << "From(" << from.x << ", " << from.y << ") "
    //        << "To(" << to.x << ", " << to.y << ")";

    // std::ostringstream bounds;
    // bounds << "TopLeft(" << topLeft.x << ", " << topLeft.y << ") "
    //        << "BottomRight(" << bottomRight.x << ", " << bottomRight.y << ")";

    // std::cout << fromto.str() << std::endl;
    // std::cout << bounds.str() << std::endl;
    clearSelectedCells();
       for(int x = topLeft.x; x < bottomRight.x; x++)
       {
              for(int y = topLeft.y; y < bottomRight.y; y++)
              {
                     Cell* curC = world->at(y,x);
                     curC->setFilterOverlay(selectOverlay);
                     selectedCells.push_back(curC);
              }
       }
              
}