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
    createACO();

   // gen.~WorldGeneration();


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
    std::vector<sf::VertexArray> pathTraces;  // Each ants path

    for (auto& i : grid)
    {
        for (auto& j : i.get()->data.entities)
        {
            if (j->getName() == "ant")
            {
                waitList.push_back(j->getHitbox());
                sf::VertexArray pathTrace(sf::LinesStrip);

                // Adding path vertices to the VertexArray (no duplicates)
                std::vector<Cell*> path = j.get()->getPath();

                
                // Loop through the path and add segments to pathTrace
                for (size_t i = 1; i < path.size(); ++i)
                {
                    float x1 = path[i - 1]->x * cellSize + cellSize / 2;
                    float y1 = path[i - 1]->y * cellSize + cellSize / 2;

                    float x2 = path[i]->x * cellSize + cellSize / 2;
                    float y2 = path[i]->y * cellSize + cellSize / 2;

                    sf::Vector2f start(x1, y1);
                    sf::Vector2f end(x2, y2);

                    sf::Vector2f direction = end - start;
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
                    float segmentThickness = 0.0f;
                    // Assuming 'segmentThickness' is a float and 'path[i]->data.p.strength' is also a float
                    float thick = segmentThickness + path[i]->data.p[0].strength; //use find food pheremone for now

                    if (thick > 1)
                    {
                        // Add the vertices to the pathTrace array
                        pathTrace.append(sf::Vertex(start, sf::Color::White));  // Start point
                        pathTrace.append(sf::Vertex(end, sf::Color::White));    // End point
                    }
                    else
                    {
                        // Remove the duplicate path if thickness is too small
                        path.erase(path.begin() + i);
                    }

                        pathTraces.push_back(pathTrace);
                }
            }
            else
            {
                window.draw(*j.get()->getHitbox());
            }
        }
    }

    // Now draw the path trace using the accumulated vertex array
    for (auto& pathTrace : pathTraces)
    {
        window.draw(pathTrace);
    }

    // Draw the waitList (RectangleShape for entities)
    for (auto& e : waitList)
    {
        window.draw(*e);
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

            }
        }
    }
    ACO aco(raw_grid[0], raw_goals, raw_grid, width, height);
    sims.push_back(aco);
    

}

    
    

//helper render
void World::drawTerrain(sf::RenderWindow & window)
{
    int drawCount = 0;

    std::vector<sf::VertexArray> allCircles;    
    std::vector<sf::Text> allTexts;

    sf::Font font;
    if (!font.loadFromFile("src/fonts/pixel.ttf")) {
        // Make sure to include a fallback or log this
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }
   
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
           

            
            //for debuggin
            drawCount++;       

            Cell* dc = this->at(x,y).get();
            if(dc->cellShape)
            {
                Circle* circleShape = dynamic_cast<Circle*>(dc->cellShape.get());
                if(circleShape)
                {          
                    float normalized = dc->data.p[0].strength; //find food pheromone for now

                    sf::Uint8 redIntensity = static_cast<sf::Uint8>(normalized * 255);

                    circleShape->setFillColor(sf::Color(redIntensity, 255,0,255*dc->data.difficulty));

                    allCircles.push_back(circleShape->getVA());

                    /*
                    
                    sf::Vector2f center = circleShape->getVA()[0].position;

                    // Prepare text
                    sf::Text text;
                    text.setFont(font);
                    text.setString(std::to_string(dc->x) + "," + std::to_string(dc->y));
                    text.setCharacterSize(12); // Adjust as needed
                    text.setFillColor(sf::Color::Red);
                    
                    // Center the text
                    sf::FloatRect bounds = text.getLocalBounds();
                    text.setOrigin(bounds.width / 2, bounds.height / 2);
                    text.setPosition(center);

                    allTexts.push_back(text);
                    */
                
                }
            }

            
        }
    }

    for (auto& circle : allCircles) 
    {
        window.draw(circle);
    }

        for (auto& text : allTexts) {
        window.draw(text);
    }
//std::cout << "terrain drawn: " + std::to_string(drawCount) << std::endl;
//std::cout << "terrain skipped: " + std::to_string(skippedCount) << std::endl;
}


void World::drawGrid(sf::RenderWindow & window)
{

    sf::VertexArray outlines(sf::Lines);
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
            int index = (x+y*width) *4;
            
            float px = x * cellSize;
            float py = y * cellSize;

            sf::FloatRect cellBounds(px,py, cellSize,cellSize);
            if(!viewRect.intersects(cellBounds))
            {
                skippedCount++;
                continue;
            }
                



            outlines.append(sf::Vertex(sf::Vector2f(px,py), sf::Color::Blue));
            outlines.append(sf::Vertex(sf::Vector2f(px + conf::cellSize, py), sf::Color::Blue));

            outlines.append(sf::Vertex(sf::Vector2f(px + conf::cellSize, py), sf::Color::Blue));
            outlines.append(sf::Vertex(sf::Vector2f(px + conf::cellSize, py + conf::cellSize), sf::Color::Blue));

            outlines.append(sf::Vertex(sf::Vector2f(px + conf::cellSize, py + conf::cellSize), sf::Color::Blue));
            outlines.append(sf::Vertex(sf::Vector2f(px, py + conf::cellSize), sf::Color::Blue));

            outlines.append(sf::Vertex(sf::Vector2f(px, py + conf::cellSize), sf::Color::Blue));
            outlines.append(sf::Vertex(sf::Vector2f(px, py), sf::Color::Blue));
            //convert to vertex
      //      sf::RectangleShape rs;
       //     rs.setSize(sf::Vector2f(cellSize, cellSize));
       //     rs.setPosition(x * cellSize, y * cellSize);
       //     rs.setOutlineThickness(0.5f);
        //    rs.setOutlineColor(sf::Color::Blue);
       //     rs.setFillColor(sf::Color::Transparent); // Optional    
            
            //for debuggin
            drawCount++;       

                
               
    
        }
    }

 //   window.draw(grid);
    window.draw(outlines);
//std::cout << "Cells drawn: " + std::to_string(drawCount) << std::endl;
//std::cout << "Cells skipped: " + std::to_string(skippedCount) << std::endl;
}

void World::render(sf::RenderWindow &window)
{
    window.clear();
    drawGrid(window);
    drawTerrain(window);
    drawEntities(window);

  //  window.display();
}

void World::handleInput(sf::RenderWindow &window, sf::Event &event)
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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
    {
        sims.clear();
        grid.clear();
        WorldGeneration gen(0,width,height,cellSize);
        grid = gen.getResult();
        createACO();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        window.close();
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O)
    {
        std::cout << "Old pf: " << conf::pF << std::endl;
        conf::pF += 0.1;
        std::cout << "New pf: " << conf::pF << std::endl;
    }
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
    {
        std::cout << "Old pf: " << conf::pF << std::endl;
        conf::pF -= 0.1;
        std::cout << "New pf: " << conf::pF << std::endl;
    }
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::K)
    {
        std::cout << "Old hf: " << conf::hF << std::endl;
        conf::hF += 0.1;
        std::cout << "New hf: " << conf::hF << std::endl;
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L)
    {
        std::cout << "Old hf: " << conf::hF << std::endl;
        conf::hF -= 0.1;
        std::cout << "New hf: " << conf::hF << std::endl;
    }
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
    {
        std::cout << "Old timestep: " << conf::timestep << std::endl;
        conf::timestep += 1;
        std::cout << "New timestep: " << conf::timestep << std::endl;
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
    {
        std::cout << "Old timestep: " << conf::timestep << std::endl;
        conf::timestep -= 1;
        std::cout << "New timestep: " << conf::timestep << std::endl;
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
