#include "../headers/World.h"
#include "World.h"
#include "../headers/GlobalVars.h"

World::World(int w, int h, sf::RenderWindow& window)
{
    width = w;
    height = h;
    view = window.getDefaultView();
    WorldGeneration gen(0,w,h,cellSize);
    grid = gen.getResult();
 //   gen.~WorldGeneration();
    createACO();


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
    //this is aco
    for(auto &a : sims)
    {
        //if(in simulated) -> i.e. not manually controlled
        a.update();
    }

}

void World::drawEntities(sf::RenderWindow& window)
{  
    std::vector<sf::RectangleShape*> waitList;
    std::vector<std::vector<Cell*>> pathTraces;

    for(auto &i : grid)
    {
        //if(i.get()->data.entities.size() > 0)
        for(auto &j : i.get()->data.entities)
        {
        //  std::cout << "Drawing entities" << std::endl;
        //  std::cout << std::to_string(j.get()->hitbox.get()->getTexture()->getSize().x) << ", "<< std::to_string(j.get()->hitbox.get()->getTexture()->getSize().y) << std::endl;
        //  std::cout << "Has texture: " << (j->hitbox->getTexture() ? "yes" : "no") << std::endl;
        //  std::cout << "Texture pointer: " << static_cast<const void*>(j->hitbox->getTexture()) << std::endl;

            if(j->getName() == "ant")
            {
                waitList.push_back(j->getHitbox());     
                std::vector<Cell*> path;
                for(auto &p:j.get()->getPath())           
                {
                    path.push_back(p);
                }
                pathTraces.push_back(path);
            }
            else{
                window.draw(*j.get()->getHitbox());

            }
          

        }

        

    }

    for(auto &e : waitList)
    {
        window.draw(*e);
    }

          //  std::cout<<"Path trace check: " << pathTraces.size() <<std::endl;

            float segmentThickness = 1.0f;

            for (auto& path : pathTraces)
            {
               // std::cout << "Path check: " << path.size() << std::endl;
            
                for (size_t i = 1; i < path.size(); ++i) // start at 1 to access previous point
                {
                  //  std::cout << "Segment: " << i - 1 << " -> " << i << std::endl;
            
                    float x1 = path[i - 1]->x * cellSize + cellSize / 2;
                    float y1 = path[i - 1]->y * cellSize + cellSize / 2;
            
                    float x2 = path[i]->x * cellSize + cellSize / 2;
                    float y2 = path[i]->y * cellSize + cellSize / 2;
            
                    sf::Vector2f start(x1, y1);
                    sf::Vector2f end(x2, y2);
            
                    sf::Vector2f direction = end - start;
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
            
                    sf::RectangleShape segment(sf::Vector2f(length, segmentThickness + path[i]->data.p.strength));
                    segment.setPosition(start);
                    segment.setRotation(angle);
                    segment.setFillColor(sf::Color::White); // You can also color based on index or path
            
                    window.draw(segment);
                }
            }
    
}

void World::createACO()
{    
    std::vector<Cell*> raw_goals;
    std::vector<Cell*> raw_grid;
    raw_grid.reserve(grid.size());
    for (const auto& cell_ptr : grid) {
        raw_grid.push_back(cell_ptr.get());  // raw pointer to the same object
    }
    for(auto &s : grid)
    {
        for(auto &e : s.get()->data.entities)
        {
            if(e->getName() == "ant")
            {
                
                for(auto &g : grid)
                {
                    for(auto &eg : g.get()->data.entities)
                    {
                        if(eg->getName() == "location")
                        {
                            raw_goals.push_back(g.get());
                        }
                    }
                }
                ACO aco(s.get(), raw_goals, raw_grid, width, height);
                sims.push_back(aco);
            }
        }
    }
    

}

//temp draw function which shows lines between all entities
/*
   void World::drawEntities(sf::RenderWindow& window)
   {
       // Iterate through grid
       for (auto &i : grid)
       {
           // Iterate through entities in the current grid cell
           for (auto it1 = i.get()->data.entities.begin(); it1 != i.get()->data.entities.end(); ++it1)
           {
               // Get position of the first entity in current cell
               sf::Vector2f pos1 = it1->get()->hitbox->getPosition() + sf::Vector2f(cellSize / 2.f, cellSize / 2.f);
   
               // Iterate through all other grid cells to compare with other entities
               for (auto &j : grid)
               {
                   // Skip the same cell, we don't need to draw lines within the same cell
                   if (i == j) continue;
   
                   // Iterate through entities in the other grid cell
                   for (auto it2 = j.get()->data.entities.begin(); it2 != j.get()->data.entities.end(); ++it2)
                   {
                       // Get position of the second entity in the other cell
                       sf::Vector2f pos2 = it2->get()->hitbox->getPosition()+ sf::Vector2f(cellSize / 2.f, cellSize / 2.f);
   
                       // Create a line using sf::VertexArray
                       sf::VertexArray line(sf::Lines, 2);
                       line[0].position = pos1;
                       line[1].position = pos2;
   
                       // Optionally set the color for the line
                       line[0].color = sf::Color::White;
                       line[1].color = sf::Color::White;
   
                       // Draw the line between the two entities
                       window.draw(line);
                   }
               }
   
               // Draw the hitbox of the current entity
               window.draw(*it1->get()->hitbox);
           }
       }
   }
*/

   
    
    

//helper render
void World::drawTerrain(sf::RenderWindow & window)
{
    int drawCount = 0;
    
   
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
           

            
            //for debuggin
            drawCount++;       

            Cell* dc = this->at(x,y).get();
            float scaled = std::clamp(dc->data.p.strength / maxPheromone, 0.0, 1.0);
            dc->cs.get()->setScale(scaled, scaled);
            window.draw(*dc->cs.get());
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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
    {
        running = true;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
    {
        running = false;
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
