#include "World.h"

World::World(int w, int h)
{
    width = w;
    height = h;
   // grid = std::vector<std::unique_ptr<Cell>>((w/cellSize)*(h/cellSize));
    for(int x = 0; x < w; x++)
    {
        for(int y = 0; y < h; y++)
        {
            std::unique_ptr<sf::CircleShape> shape = std::make_unique<sf::CircleShape>(cellSize/4);
            shape->setOrigin(cellSize/4,cellSize/4);
            shape->setPosition(
                x * cellSize + cellSize / 2.f,
                y * cellSize + cellSize / 2.f
            );
            shape->setFillColor(sf::Color::Green);    

            std::unique_ptr<Cell> newC = std::make_unique<Cell>();     
            CellData cd;

            newC->cs = std::move(shape);   

            cd.type = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
            newC->x = x;
            newC->y = y;
            newC->data = cd;
            grid.push_back(std::move(newC));
        }
    }
}

std::unique_ptr<Cell> &World::at(int x, int y)
{
    return grid[y * width + x];
}

const std::unique_ptr<Cell> &World::at(int x, int y) const
{
    return grid[y*width+x];
}

void World::update()
{

}


void World::render(sf::RenderWindow &window)
{
   // std::cout << "Rendering" << std::endl;
    window.clear();
    for(auto &i : grid)
    {
        window.draw(*i->cs);
    }
    window.display();
}

World::~World()
{
}

int World::getWidth()
{
    return 0;
}

int World::getHeight()
{
    return 0;
}

std::vector<std::unique_ptr<Cell>> World::getGrid()
{
    return std::vector<std::unique_ptr<Cell>>();
}

std::vector<Cell> World::getBase()
{
    return std::vector<Cell>();
}
