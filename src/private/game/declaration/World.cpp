#include "../headers/World.h"

World::World(int w, int h, sf::RenderWindow& window)
{
    width = w;
    height = h;
    view = window.getDefaultView();

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

//helper render
void World::drawGrid(sf::RenderWindow & window)
{
    int drawCount = 0;
    int skippedCount = 0;
    
    sf::FloatRect viewRect(
        window.getView().getCenter().x - window.getView().getSize().x / 2.f,
        window.getView().getCenter().y - window.getView().getSize().y / 2.f,
        window.getView().getSize().x,
        window.getView().getSize().y
    );
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            float px = x * cellSize;
            float py = y * cellSize;

            sf::FloatRect cellBounds(px,py, cellSize,cellSize);
            if(!viewRect.intersects(cellBounds))
            {
                skippedCount++;
                continue;
            }
                

            sf::RectangleShape rs;
            rs.setSize(sf::Vector2f(cellSize, cellSize));
            rs.setPosition(x * cellSize, y * cellSize);
            rs.setOutlineThickness(1.f);
            rs.setOutlineColor(sf::Color::Blue);
            rs.setFillColor(sf::Color::Transparent); // Optional    
            
            //for debuggin
            drawCount++;       

                
               
    
            window.draw(rs);
        }
    }
std::cout << "Cells drawn: " + std::to_string(drawCount) << std::endl;
std::cout << "Cells skipped: " + std::to_string(skippedCount) << std::endl;
}

void World::render(sf::RenderWindow &window)
{
    window.clear();
    drawGrid(window);
    /*
        for(int i =0; i < height; i ++)
    {
        window.draw(*this->at(i,i).get()->cs); //eyawouigwe8uigybwr
    }
    */

    //window.draw(*this->at(5,5).get()->cs);
    window.display();
}

void World::handleInput(sf::RenderWindow &window)
{
        //Temp input
            window.setView(view);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                view.move(0, -0.5); // up
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                view.move(0, 0.5);  // down
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                view.move(-0.5, 0); // left
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                view.move(0.5, 0);  // right    
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                view.zoom(1.0001f); // zoom out
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                view.zoom(0.9999f); // zoom in
            
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
