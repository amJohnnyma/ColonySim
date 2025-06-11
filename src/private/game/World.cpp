#include "World.h"
#include "GlobalVars.h"
#include "../controller/InputManager.h" 
#include "World.h"

World::World(sf::RenderWindow& window)
{
 
    trackedVars = new TrackedVariables();
    chunkManager = std::make_unique<ChunkManager>();
    createACO(); 


}

Cell* World::at(int x, int y) {
    int cx = x / conf::chunkSize;
    int cy = y / conf::chunkSize;
    if(!chunkManager->hasLoaded(cx,cy))
    {
        return nullptr;
    } 
    int lx = x % conf::chunkSize;
    int ly = y % conf::chunkSize;   
    if(lx < 0 || ly < 0)
    {
        return nullptr;
    }
    return chunkManager->getChunk(cx,cy)->at(lx, ly);
}

Chunk* World::getChunkAt(int chunkX, int chunkY) {
    return chunkManager->getChunk(chunkX, chunkY);
}

void World::update()
{
    int numAnts = 0;
    //this is aco
    for(auto &a : sims)
    {
        //if(in simulated) -> i.e. not manually controlled
        //std::cout << "Update aco" << std::endl;
      //  a->update();
        numAnts += a->getNumberAnts();
        //std::cout << "done" << std::endl;
    }
    trackedVars->setNumAnts(numAnts);

    //temp
     sf::Vector2f viewCenter = trackedVars->getViewCenter();
     int chunkX = static_cast<int>(viewCenter.x / (conf::chunkSize * conf::cellSize));
     int chunkY = static_cast<int>(viewCenter.y / (conf::chunkSize * conf::cellSize));

    // std::cout << "View Center: " << viewCenter.x << ", " << viewCenter.y << std::endl;
   //  std::cout << "Chunk Center: " << chunkX << ", " << chunkY << std::endl;
    // // Load necessary chunks and unload distant ones
    chunkManager->ensureChunksAround(chunkX, chunkY, 5);  // load chunks in a 5x5 area
    chunkManager->unloadDistantChunks(chunkX, chunkY, 5); // unload chunks beyond 9x9 area

   

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
   endX   = std::min(conf::worldSize.x, endX);
    endY   = std::min(conf::worldSize.y, endY);

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
    //std::cout << dt << std::endl;

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

    for(int x = 0; x < conf::worldSize.x/conf::chunkSize; x++)
    {
        for(int y = 0; y < conf::worldSize.y/conf::chunkSize;y++)
        {
            Chunk* chunk = getChunkAt(x,y);
            if(!chunk) continue;
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
    }


    for(auto & base : trackedVars->getBases())
    {
        ACO* aco = new ACO(raw_goals, this, base);
        sims.push_back(aco);
    }
    

}

    
    

//helper render
void World::drawTerrain(sf::RenderWindow& window)
{
    sf::VertexArray combinedVA(sf::TriangleFan); 
    combinedVA.resize(0); // Clear just in case

    const sf::View& view = window.getView();

    int startX = static_cast<int>((view.getCenter().x - view.getSize().x / 2) / conf::cellSize);// - 1;
    int endX   = static_cast<int>((view.getCenter().x + view.getSize().x / 2) / conf::cellSize);// + 1;
    int startY = static_cast<int>((view.getCenter().y - view.getSize().y / 2) / conf::cellSize);// - 1;
    int endY   = static_cast<int>((view.getCenter().y + view.getSize().y / 2) / conf::cellSize);// + 1;

  
    startX = std::max(0, startX);
    startY = std::max(0, startY);
  //  endX   = std::min(conf::worldSize.x, endX);
  //  endY   = std::min(conf::worldSize.y, endY);

    // Using sf::Quads is more efficient for rectangles
    combinedVA.setPrimitiveType(sf::Quads);
//std::cout << "Boom" << std::endl;
    for (int x = startX; x < endX; x++) {
        for (int y = startY; y < endY; y++) {
          //  std::cout << "Bam" << std::endl;
            Cell* dc = this->at(x, y);
          //  std::cout << "Crash" << std::endl;
            if (dc != nullptr && dc->cellShape) {
               // std::cout << "Craaaaa" << std::endl;
                Rectangle* shape = static_cast<Rectangle*>(dc->cellShape.get());
                if (shape) {
                    const sf::VertexArray& va = shape->getVA();

                    // We assume shape->getVA() uses sf::TriangleFan, so we have 6 vertices (center + 4 corners + close)

                    // Convert the TriangleFan to quads here, or just append the four corners for quads:

                    // Extract corners (vertices 1 to 4) to add to combined VA as a quad:
                    if (va.getVertexCount() >= 5) {
                        size_t baseIndex = combinedVA.getVertexCount();
                        combinedVA.resize(baseIndex + 4);

                        // Copy the 4 corners (skip center and last duplicate vertex)
                        combinedVA[baseIndex + 0] = va[1];
                        combinedVA[baseIndex + 1] = va[2];
                        combinedVA[baseIndex + 2] = va[3];
                        combinedVA[baseIndex + 3] = va[4];
                    }
                }
            }
        }
    }

    window.draw(combinedVA);
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
    for (int x = 0; x < conf::worldSize.x; x++) {
        for (int y = 0; y < conf::worldSize.y; y++) {
            int index = (x+y*conf::worldSize.x) *4;
            
            float px = x * conf::cellSize;
            float py = y * conf::cellSize;

            sf::FloatRect cellBounds(px,py, conf::cellSize,conf::cellSize);
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
    //std::cout << "Draw terrain" << std::endl;
    drawTerrain(window);
    //std::cout << "Fin draw" << std::endl;
    //std::cout << "Draw ent" << std::endl;
    drawEntities(window);
    //std::cout << "Fin draw" << std::endl;

}

// Usage in World::handleInput
void World::handleInput(InputManager& inputManager, sf::RenderWindow& window) {
    speed = baseSpeed * inputManager.getCurrentZoom();
    sf::View& view = inputManager.getView();
    bool inBounds = true;
    sf::Vector2f center = view.getCenter();
    if (inputManager.isKeyHeld(sf::Keyboard::W) && center.y > 0)
    {
        view.move(0, -speed);
    }
    if (inputManager.isKeyHeld(sf::Keyboard::S) && center.y < conf::max_world_size.y * conf::chunkSize * conf::cellSize)
    {
        view.move(0, speed);
    }
    if (inputManager.isKeyHeld(sf::Keyboard::A) && center.x > 0)
    {
        view.move(-speed, 0);
    }
    if (inputManager.isKeyHeld(sf::Keyboard::D)&& center.x < conf::max_world_size.x * conf::chunkSize * conf::cellSize)
    {
        view.move(speed, 0);
    }

    if (inputManager.isKeyHeld(sf::Keyboard::Num1)) running = true;
    if (inputManager.isKeyHeld(sf::Keyboard::Num2)) running = false;

    if (inputManager.isKeyHeld(sf::Keyboard::R)) {
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
    trackedVars->setViewCenter(view.getCenter());
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
