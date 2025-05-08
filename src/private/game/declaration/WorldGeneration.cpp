#include "../headers/WorldGeneration.h"

WorldGeneration::WorldGeneration(unsigned int seed, int xWidth, int yWidth, int cellSize)
{
    seed = seed;
    width = xWidth;
    height = yWidth;
    this->cellSize = cellSize;
    generateTerrain();
    generateEntities(4);
    assignTextures();


}

WorldGeneration::~WorldGeneration()
{
}

std::vector<std::unique_ptr<Cell>> WorldGeneration::getResult()
{
    return std::move(grid);
}

void WorldGeneration::generateTerrain()
{
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            std::unique_ptr<sf::CircleShape> shape = std::make_unique<sf::CircleShape>(cellSize/4);
            shape->setOrigin(cellSize/4,cellSize/4);
            shape->setPosition(
                x * cellSize + cellSize / 2.f,
                y * cellSize + cellSize / 2.f
            );

            std::unique_ptr<Cell> newC = std::make_unique<Cell>();     
            CellData cd;
            pheromone p;
            p.strength = 0;
            p.x = x;
            p.y = y;
            p.type = "default";


            cd.type = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
            //random between 0-1. 0 easy, 1 hard
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.0,1.0);
            double randomVal = dis(gen);
            cd.difficulty = randomVal;

            shape->setFillColor(sf::Color(0,255,0, (randomVal*255)));    

            cd.p = p;
            newC->cs = std::move(shape);   
            newC->x = x;
            newC->y = y;
            newC->data = std::move(cd);
            grid.push_back(std::move(newC));
        }
    }
}

void WorldGeneration::generateEntities(int num)
{

    std::vector<std::pair<int,int>> vis;
    for(int k = 0; k < num; k ++)
    {
        std::unique_ptr<entity> et = std::make_unique<entity>();
        et->name = "ant";
        std::unique_ptr<sf::RectangleShape> rs = std::make_unique<sf::RectangleShape>();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> xdis(0.0,width);
        std::uniform_int_distribution<> ydis(0.0, height);

        std::pair<int, int> point = {xdis(gen), ydis(gen)};

        if (std::find(vis.begin(), vis.end(), point) == vis.end()) {
            et->x = xdis(gen);
            et->y = ydis(gen);
    
            vis.push_back({et->x, et->y});

            rs.get()->setSize(sf::Vector2f(cellSize,cellSize));
            rs.get()->setPosition(et->x * cellSize, et->y * cellSize);  
            rs.get()->setOutlineThickness(1.f); 
            rs.get()->setOutlineColor(sf::Color::Red);
            rs.get()->setFillColor(sf::Color::White);    

            et->hitbox = std::move(rs);
            grid[et->y*width+et->x].get()->data.entities.push_back(std::move(et));
            
        }
        else{
            k --; //retry this step
        }       

    }    

}
#include <filesystem>

void WorldGeneration::assignTextures()
{
    auto& manager = TextureManager::getInstance();
    sf::Texture* antTexture = manager.loadTexture("ant", "src/textures/entities/ants/ant.png");

    if (!antTexture) return;

    for (auto& cell : grid)
    {
        for (auto& ent : cell->data.entities)
        {
            if (ent->name == "ant")
            {
                ent->hitbox->setTexture(antTexture);

            }
        }
    }
}

