#include "../headers/World.h"
#include "../headers/GlobalVars.h"
#include "../../controller/InputManager.h" 
#include "World.h"

World::World(int w, int h, sf::RenderWindow& window)
{
    width = w;
    height = h;
    view = window.getDefaultView();
    trackedVars = new TrackedVariables();
    WorldGeneration gen(0,w,h,cellSize);
    grid = gen.getResult();    
    createACO();

   // gen.~WorldGeneration();


}

Cell* World::at(int x, int y) {
    int cx = x / conf::chunkSize;
    int cy = y / conf::chunkSize;
    int lx = x % conf::chunkSize;
    int ly = y % conf::chunkSize;
    return grid[{cx, cy}]->at(lx, ly);
}


void World::update()
{
    int numAnts = 0;
    //this is aco
    for(auto &a : sims)
    {
        //if(in simulated) -> i.e. not manually controlled
        //std::cout << "Update aco" << std::endl;
        a->update();
        numAnts += a->getNumberAnts();
        //std::cout << "done" << std::endl;
    }
    trackedVars->setNumAnts(numAnts);
    //this sucks only update when the world has changed
    /*
    std::vector<Cell*> rawPointers;
    rawPointers.reserve(grid.size());
    for (const auto& cellPtr : grid) {
        rawPointers.push_back(cellPtr.get());
        for(const auto& e : cellPtr.get()->data.entities)
        {
            if(e.get()->getName() == "Base")
            {
                antBase = {cellPtr.get()->x, cellPtr.get()->y};
            }
        }
    }
    trackedVars.setWorld(rawPointers);
    */
    
    for (auto& [key, chunk] : grid) {
        //chunk->update();
        //chunk->render();
    }
  



}

void World::drawEntities(sf::RenderWindow& window)
{
    std::vector<Entity*> waitList;
    std::vector<sf::VertexArray> pathTraces;  // Each ants path

for (auto& [chunkCoords, chunkPtr] : grid)
{
    if (!chunkPtr) continue;

    for (auto& cellPtr : chunkPtr->getCells())
    {
        if (!cellPtr) continue;

        for (auto& entityPtr : cellPtr->data.entities)
        {
            if (!entityPtr) continue;

            if (Ant* ant = dynamic_cast<Ant*>(entityPtr.get()))
            {
                waitList.push_back(ant);

                // Example: setting up path trace (optional)
                sf::VertexArray pathTrace(sf::LinesStrip);

                // // Adding path vertices to the VertexArray (no duplicates)
                // std::vector<Cell*> path = j->getPath();

                
                // // Loop through the path and add segments to pathTrace
                // for (size_t i = 1; i < path.size(); ++i)
                // {
                //     float x1 = path[i - 1]->x * cellSize + cellSize / 2;
                //     float y1 = path[i - 1]->y * cellSize + cellSize / 2;

                //     float x2 = path[i]->x * cellSize + cellSize / 2;
                //     float y2 = path[i]->y * cellSize + cellSize / 2;

                //     sf::Vector2f start(x1, y1);
                //     sf::Vector2f end(x2, y2);

                //     sf::Vector2f direction = end - start;
                //     float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                //     float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
                //     float segmentThickness = 0.0f;
                //     // Assuming 'segmentThickness' is a float and 'path[i]->data.p.strength' is also a float
                //     float thick = segmentThickness + path[i]->data.p[0].strength; //use find food pheremone for now

                //     if (thick > 1)
                //     {
                //         // Add the vertices to the pathTrace array
                //         pathTrace.append(sf::Vertex(start, sf::Color::White));  // Start point
                //         pathTrace.append(sf::Vertex(end, sf::Color::White));    // End point
                //     }
                //     else
                //     {
                //         // Remove the duplicate path if thickness is too small
                //         path.erase(path.begin() + i);
                //     }

                //         pathTraces.push_back(pathTrace);
               // }
            }
            else
            {
                window.draw(*entityPtr->getHitbox());
            }
        }
    }
    }

    // Now draw the path trace using the accumulated vertex array
  //  for (auto& pathTrace : pathTraces)
  //  {
   //     window.draw(pathTrace);
  //  }

    float dt = antClock.restart().asSeconds();
    // Draw the waitList (RectangleShape for entities)
    for (auto& e : waitList)
    {
        if(Ant* ant = dynamic_cast<Ant*>(e))
        {
            if(ant->stillAnimating())
                ant->updateMovement(dt);
        }
        window.draw(*e);
    }
}

void World::createACO()
{    
    std::vector<Cell*> raw_goals;

    for (auto& [coord, chunkPtr] : grid) {
    Chunk* chunk = chunkPtr.get();
       for (auto& cellPtr : chunk->getCells()) { // assuming Chunk is a container of Cells
            for (auto& eg : cellPtr->data.entities) {
                if (eg->getName().find("location") != std::string::npos) {
                    raw_goals.push_back(cellPtr.get());
                    std::cout << "Pushed back location" << std::endl;
                }
                if (eg->getName().find("Base") != std::string::npos) {
                    trackedVars->setBase(eg.get());
                }
            }
        } 
    }

    for(auto & base : trackedVars->getBases())
    {
        ACO* aco = new ACO(raw_goals, this, width, height, base);
        sims.push_back(aco);
    }
    

}

    
    

//helper render
void World::drawTerrain(sf::RenderWindow & window)
{
    int drawCount = 0;

    std::vector<sf::VertexArray> allgridshapes;    
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

            Cell* dc = this->at(x,y);
            //draw pheromone on dc at some point
            if(dc->cellShape)
            {
                Rectangle* shape = dynamic_cast<Rectangle*>(dc->cellShape.get());
                if(shape)
                {          
                 //   float normalized = dc->data.p[0].strength; //find food pheromone for now          

                    allgridshapes.push_back(shape->getVA());

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

    for (auto& shape : allgridshapes) 
    {
        window.draw(shape);
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

// Usage in World::handleInput
void World::handleInput(InputManager& inputManager, sf::RenderWindow& window) {
    window.setView(view);
    speed = baseSpeed * currentZoom;

    if (inputManager.isKeyHeld(sf::Keyboard::W)) view.move(0, -speed);
    if (inputManager.isKeyHeld(sf::Keyboard::S)) view.move(0, speed);
    if (inputManager.isKeyHeld(sf::Keyboard::A)) view.move(-speed * 0.5f, 0);
    if (inputManager.isKeyHeld(sf::Keyboard::D)) view.move(speed * 0.5f, 0);

    if (inputManager.isKeyHeld(sf::Keyboard::Q)) {
        view.zoom(1.001f);
        currentZoom *= 1.001f;
    }
    if (inputManager.isKeyHeld(sf::Keyboard::E)) {
        view.zoom(0.999f);
        currentZoom *= 0.999f;
    }

    if (inputManager.isKeyHeld(sf::Keyboard::Num1)) running = true;
    if (inputManager.isKeyHeld(sf::Keyboard::Num2)) running = false;

    if (inputManager.isKeyHeld(sf::Keyboard::R)) {
        sims.clear();
        grid.clear();
        WorldGeneration gen(0, width, height, cellSize);
        grid = gen.getResult();
        createACO();
    }

    if (inputManager.isKeyHeld(sf::Keyboard::Escape)) window.close();

    if (inputManager.isKeyPressedOnce(sf::Keyboard::O)) {
        std::cout << "Old pf: " << conf::pF << std::endl;
        conf::pF += 0.1;
        std::cout << "New pf: " << conf::pF << std::endl;
    }

    if (inputManager.isKeyPressedOnce(sf::Keyboard::P)) {
        std::cout << "Old pf: " << conf::pF << std::endl;
        conf::pF -= 0.1;
        std::cout << "New pf: " << conf::pF << std::endl;
    }

    if (inputManager.isKeyPressedOnce(sf::Keyboard::K)) {
        std::cout << "Old hf: " << conf::hF << std::endl;
        conf::hF += 0.1;
        std::cout << "New hf: " << conf::hF << std::endl;
    }

    if (inputManager.isKeyPressedOnce(sf::Keyboard::L)) {
        std::cout << "Old hf: " << conf::hF << std::endl;
        conf::hF -= 0.1;
        std::cout << "New hf: " << conf::hF << std::endl;
    }

    if (inputManager.isKeyPressedOnce(sf::Keyboard::Up)) {
        std::cout << "Old timestep: " << conf::timestep << std::endl;
        conf::timestep += 1;
        std::cout << "New timestep: " << conf::timestep << std::endl;
    }

    if (inputManager.isKeyPressedOnce(sf::Keyboard::Down)) {
        std::cout << "Old timestep: " << conf::timestep << std::endl;
        conf::timestep -= 1;
        std::cout << "New timestep: " << conf::timestep << std::endl;
    }
}
void World::toggleSimState()
{
    running = !running;
}

void World::buildBuilding(std::string type)
{
    if(trackedVars->isInBuildingMode())
    {
        for(auto & sc : trackedVars->getSelectedCells())
        {
            //std::cout << "Building in cell: " + std::to_string(sc->x) + ", " + std::to_string(sc->y) << std::endl;
            auto building = std::make_unique<BuildingLocation>(sc->x, sc->y, "Building");
            Cell* cell = at(sc->x, sc->y);

            cell->data.entities.push_back(std::unique_ptr<Entity>(building.release()));
        }
    }    

}

void World::destroyBuilding(std::string type)
{
    if(trackedVars->isInBuildingMode())
    {
        for(auto & sc : trackedVars->getSelectedCells())
        {
            //std::cout << "Building in cell: " + std::to_string(sc->x) + ", " + std::to_string(sc->y) << std::endl;
            Cell* cell = at(sc->x, sc->y);
            auto& entities = cell->data.entities;
            entities.erase(std::remove_if(entities.begin(), entities.end(),
                [](const std::unique_ptr<Entity>& e) {
                    return dynamic_cast<BuildingLocation*>(e.get()) != nullptr;
                }),
                entities.end());

        }
    }    
}
World::~World()
{
    delete trackedVars;
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
