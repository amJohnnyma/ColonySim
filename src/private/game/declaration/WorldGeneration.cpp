#include "../headers/WorldGeneration.h"

WorldGeneration::WorldGeneration(unsigned int seed, int xWidth, int yWidth, int cellSize)
{
    seed = seed;
    width = xWidth;
    height = yWidth;
    this->cellSize = cellSize;
    generateTerrain();
    generateEntities(2);
    assignTextures();
    generateLocations(5);


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

            p.strength = randomVal * 2; //arbitrary init


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
    //test entities to see grid indexes
    /*
    for(int i = 0; i < 5; i ++)
    {
        std::unique_ptr<entity> et = std::make_unique<entity>();
        et->name = "ant";
        std::unique_ptr<sf::RectangleShape> rs = std::make_unique<sf::RectangleShape>();
    
        et->x = i;
        et->y = 0;

        vis.push_back({et->x, et->y});

        rs.get()->setSize(sf::Vector2f(cellSize,cellSize));
        rs.get()->setPosition(et->x * cellSize, et->y * cellSize);  
        rs.get()->setOutlineThickness(1.f); 
        rs.get()->setOutlineColor(sf::Color::Red);
        rs.get()->setFillColor(sf::Color::White);    

        et->hitbox = std::move(rs);
        grid[et->y*width+et->x].get()->data.entities.push_back(std::move(et));
    
    }
        */
    //////
    
    int xVal;
    int yVal;
    for(int k = 0; k < num; k ++)
    {
        sf::RectangleShape* rs = new sf::RectangleShape();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> xdis(0.0,width-1);
        std::uniform_int_distribution<> ydis(0.0, height-1);

        std::pair<int, int> point = {xdis(gen), ydis(gen)};

        xVal = xdis(gen);
        yVal = ydis(gen);

        if (std::find(vis.begin(), vis.end(), point) == vis.end()) {
            rs->setSize(sf::Vector2f(cellSize,cellSize));
            rs->setPosition(xdis(gen) * cellSize, ydis(gen) * cellSize);  
            rs->setOutlineThickness(1.f); 
            rs->setOutlineColor(sf::Color::Red); 
            rs->setFillColor(sf::Color::White);   
            Cell* cell = grid[yVal*width+xVal].get();
            auto et = std::make_unique<Entity>(xVal, yVal, "ant", 10, std::make_unique<sf::RectangleShape>(*rs),cell); 
    
            vis.push_back({et->getY(), et->getY()});            

            


            grid[yVal*width+xVal].get()->data.entities.push_back(std::move(et));


            
            

        }
        else{
            k--;
        }
      

    }   
    
    sf::RectangleShape* rs2 = new sf::RectangleShape();      
    
    rs2->setSize(sf::Vector2f(cellSize,cellSize));
    rs2->setPosition(xVal * cellSize, yVal * cellSize);  
    rs2->setOutlineThickness(1.f); 
    rs2->setOutlineColor(sf::Color::Red);
    rs2->setFillColor(sf::Color::Yellow);   
    Cell* cell = grid[yVal*width+xVal].get();

    auto et2 = std::make_unique<Entity>(xVal, yVal, "Base", 100, std::make_unique<sf::RectangleShape>(*rs2),cell); //make ant entity
    grid[yVal*width+xVal].get()->data.entities.push_back(std::move(et2));


    for(auto &i : grid)
    {
        for(auto &k : i.get()->data.entities)
        {
            std::cout << k.get()->getName() << ", " << k.get()->getX() << ", " << k.get()->getX() << std::endl;
        }
    }

}

void WorldGeneration::assignTextures()
{
    auto& manager = TextureManager::getInstance();
    sf::Texture* antTexture = manager.loadTexture("ant", "src/textures/entities/ants/ant.png");

    if (!antTexture) return;

    for (auto& cell : grid)
    {
        for (auto& ent : cell->data.entities)
        {
            if (ent->getName() == "ant")
            {
                ent->getHitbox()->setTexture(antTexture);

            }
        }
    }
}

void WorldGeneration::generateLocations(int num)
{
    std::vector<std::pair<int,int>> vis;
    for(int k = 0; k < num; k ++)
    {
        sf::RectangleShape* rs = new sf::RectangleShape();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> xdis(0.0,width-1);
        std::uniform_int_distribution<> ydis(0.0, height-1);

        std::pair<int, int> point = {xdis(gen), ydis(gen)};

        if (std::find(vis.begin(), vis.end(), point) == vis.end()) {
    
            vis.push_back({point.first, point.second});


            double dif = grid[point.second*width+point.first].get()->data.difficulty;

            rs->setSize(sf::Vector2f(cellSize,cellSize));
            rs->setPosition(point.first * cellSize, point.second * cellSize);  
            rs->setOutlineThickness(1.f); 
            rs->setOutlineColor(sf::Color::White);
            rs->setFillColor(sf::Color(255,255,255,dif*100));   



            Cell* cell = grid[point.first*width+point.second].get();

            auto et = std::make_unique<Entity>(point.first, point.second, "location", 1000, std::make_unique<sf::RectangleShape>(*rs),cell);

            grid[point.first*width+point.second].get()->data.entities.push_back(std::move(et));
            
        }
        else{
            k --; //retry this step
        }       

    }  
}