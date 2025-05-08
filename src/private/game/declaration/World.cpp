#include "../headers/World.h"
#include "World.h"

World::World(int w, int h, sf::RenderWindow& window)
{
    width = w;
    height = h;
    view = window.getDefaultView();
    WorldGeneration gen(0,w,h,cellSize);
    grid = gen.getResult();
 //   gen.~WorldGeneration();


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
void World::drawEntities(sf::RenderWindow& window)
{  

    for(auto &i : grid)
    {
        //if(i.get()->data.entities.size() > 0)
        for(auto &j : i.get()->data.entities)
        {
          //  std::cout << "Drawing entities" << std::endl;
          //  std::cout << std::to_string(j.get()->hitbox.get()->getTexture()->getSize().x) << ", "<< std::to_string(j.get()->hitbox.get()->getTexture()->getSize().y) << std::endl;
         // std::cout << "Has texture: " << (j->hitbox->getTexture() ? "yes" : "no") << std::endl;
        //  std::cout << "Texture pointer: " << static_cast<const void*>(j->hitbox->getTexture()) << std::endl;

          window.draw(*j->hitbox);

        }

        

    }
    
}

//helper render
void World::drawTerrain(sf::RenderWindow & window)
{
    int drawCount = 0;
    
   
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
           

            
            //for debuggin
            drawCount++;       

                
               
    
            window.draw(*this->at(x,y).get()->cs);
        }
    }
//std::cout << "terrain drawn: " + std::to_string(drawCount) << std::endl;
//std::cout << "terrain skipped: " + std::to_string(skippedCount) << std::endl;
}


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
            rs.setOutlineThickness(0.5f);
            rs.setOutlineColor(sf::Color::Blue);
            rs.setFillColor(sf::Color::Transparent); // Optional    
            
            //for debuggin
            drawCount++;       

                
               
    
            window.draw(rs);
        }
    }
//std::cout << "Cells drawn: " + std::to_string(drawCount) << std::endl;
//std::cout << "Cells skipped: " + std::to_string(skippedCount) << std::endl;
}

/*
void World::drawGrid(sf::RenderWindow &window)
{
    int drawCount = 0;
    int skippedCount = 0;
    
    // Get current view bounds
    sf::FloatRect viewRect(
        window.getView().getCenter().x - window.getView().getSize().x / 2.f,
        window.getView().getCenter().y - window.getView().getSize().y / 2.f,
        window.getView().getSize().x,
        window.getView().getSize().y
    );

    // Adjust cell size based on the zoom level
    float zoom = view.getSize().x / 800.f; // You can change 800.f to be the base size for your zoom factor
    float zoomedCellSize = cellSize * zoom; // Adjust cell size based on zoom

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            float px = x * zoomedCellSize;
            float py = y * zoomedCellSize;

            // Check if the current cell intersects with the view rectangle
            sf::FloatRect cellBounds(px, py, zoomedCellSize, zoomedCellSize);
            if (!viewRect.intersects(cellBounds)) {
                skippedCount++;
                continue;
            }

            // Create a rectangle shape for the grid cell
            sf::RectangleShape rs;
            rs.setSize(sf::Vector2f(zoomedCellSize, zoomedCellSize));
            rs.setPosition(px, py);
            rs.setOutlineThickness(0.5f);
            rs.setOutlineColor(sf::Color::Blue);
            rs.setFillColor(sf::Color::Transparent); // Optional: you can change this to fill cells with color

            // For debugging purposes: count cells drawn/skipped
            drawCount++;

            // Draw the grid cell
            window.draw(rs);
        }
    }
    
    // For debugging: uncomment to log drawn/skipped counts
    // std::cout << "Cells drawn: " + std::to_string(drawCount) << std::endl;
    // std::cout << "Cells skipped: " + std::to_string(skippedCount) << std::endl;
}
*/
void World::render(sf::RenderWindow &window)
{
    window.clear();
    drawGrid(window);
    drawTerrain(window);
    drawEntities(window);

    window.display();
}

void World::handleInput(sf::RenderWindow &window)
{
    window.setView(view);

    speed = baseSpeed * currentZoom; // Scale speed based on zoom

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        view.move(0, -speed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        view.move(0, speed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        view.move(-speed * 0.5f, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        view.move(speed * 0.5f, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        view.zoom(1.001f);
        currentZoom *= 1.001f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        view.zoom(0.999f);
        currentZoom *= 0.999f;
    }
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
