#include "../headers/World.h"
#include "../headers/GlobalVars.h"
#include "../../controller/InputManager.h" 
#include "World.h"

World::World(int w, int h, sf::RenderWindow& window)
{
    width = w;
    height = h;        
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
    std::vector<Ant*> ants;
    std::vector<BuildingLocation*> buildings;

    // Get visible area
    const sf::View view = window.getView();

    int startX = static_cast<int>((view.getCenter().x - view.getSize().x / 2) / conf::cellSize) - 1;
    int endX   = static_cast<int>((view.getCenter().x + view.getSize().x / 2) / conf::cellSize) + 1;

    int startY = static_cast<int>((view.getCenter().y - view.getSize().y / 2) / conf::cellSize) - 1;
    int endY   = static_cast<int>((view.getCenter().y + view.getSize().y / 2) / conf::cellSize) + 1;

    startX = std::max(0, startX);
    startY = std::max(0, startY);
    endX   = std::min(height, endX);
    endY   = std::min(width, endY);

    // Iterate over relevant cells
    for (int x = startX; x < endX; x++)
    {
        for (int y = startY; y < endY; y++)
        {
            Cell* cell = this->at(x, y);
            if (!cell) continue;

            for (auto& entityPtr : cell->data.entities)
            {
                if (!entityPtr) continue;

                if (Ant* ant = dynamic_cast<Ant*>(entityPtr.get()))
                {
                    ants.push_back(ant);
                }
                else if (BuildingLocation* bl = dynamic_cast<BuildingLocation*>(entityPtr.get()))
                {
                    buildings.push_back(bl);
                }
                else
                {
                    window.draw(*entityPtr->getHitbox());
                }
            }
        }
    }

    float dt = antClock.restart().asSeconds();

    for (Ant* e : ants)
    {
        if (e->stillAnimating())
            e->updateMovement(dt);

        window.draw(*e);
    }

    for (BuildingLocation* bl : buildings)
    {
        window.draw(*bl);
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

    std::vector<const sf::VertexArray*> allgridshapes;    
    const sf::View view = window.getView();
     int startX = static_cast<int>((view.getCenter().y - view.getSize().y / 2) / conf::cellSize) - 1;
        int endX   = static_cast<int>((view.getCenter().y + view.getSize().y / 2) / conf::cellSize) + 1;

        int startY = static_cast<int>((view.getCenter().x - view.getSize().x / 2) / conf::cellSize) - 1;
        int endY   = static_cast<int>((view.getCenter().x + view.getSize().x / 2) / conf::cellSize) + 1;

        // Clamp to map bounds
        startX = std::max(0, startX);
        startY = std::max(0, startY);
        endX   = std::min(width, endX);
        endY   = std::min(height, endY);

        // Draw loop
        for (int x = startX; x < endX; x++) {
            for (int y = startY; y < endY; y++) {   

            
            //for debuggin
            drawCount++;       

            Cell* dc = this->at(x,y);
            //draw pheromone on dc at some point
            if(dc->cellShape)
            {
                Rectangle* shape = static_cast<Rectangle*>(dc->cellShape.get());
                if(shape)
                {          

                    allgridshapes.push_back(&shape->getVA());

                
                }
            }

            
        }
    }

    for (auto& shape : allgridshapes) 
    {
        window.draw(*shape);
    }
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
    //drawGrid(window);
    drawTerrain(window);
    drawEntities(window);

  //  window.display();
}

// Usage in World::handleInput
void World::handleInput(InputManager& inputManager, sf::RenderWindow& window) {
    speed = baseSpeed * inputManager.getCurrentZoom();
    sf::View& view = inputManager.getView();
    if (inputManager.isKeyHeld(sf::Keyboard::W)) view.move(0, -speed);
    if (inputManager.isKeyHeld(sf::Keyboard::S)) view.move(0, speed);
    if (inputManager.isKeyHeld(sf::Keyboard::A)) view.move(-speed * 0.5f, 0);
    if (inputManager.isKeyHeld(sf::Keyboard::D)) view.move(speed * 0.5f, 0);


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
    window.setView(view);
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
