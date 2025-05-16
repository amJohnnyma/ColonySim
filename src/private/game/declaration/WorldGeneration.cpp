#include "../headers/WorldGeneration.h"

WorldGeneration::WorldGeneration(unsigned int seed, int xWidth, int yWidth, int cellSize)
{
    seed = seed;
    width = xWidth;
    height = yWidth;
    this->cellSize = cellSize;
    grid.clear();

    generateTerrain();
    generateEntities(conf::numAnts,conf::numBases);
    assignTextures();
    generateLocations(conf::numLocations);



}

WorldGeneration::~WorldGeneration()
{
}

std::vector<std::unique_ptr<Cell>> WorldGeneration::getResult() {
    return std::move(grid); // Transfer ownership safely
}

void WorldGeneration::generateTerrain()
{
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
          //  std::unique_ptr<Shape> shape = std::make_unique<Circle>(x, y, cellSize/4, 4);
          std::unique_ptr<Circle> shape = std::make_unique<Circle>(x,y,cellSize/4, 8);

            std::unique_ptr<Cell> newC = std::make_unique<Cell>();     
            CellData cd;
            pheromone p,p2;
            p.x = x;
            p.y = y;
            p.type = "findFood";

            p2.x = x;
            p2.y = y;
            p.type = "returnHome";


            cd.type = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
            //random between 0-1. 0 easy, 1 hard
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.001,1.0);
            double randomVal = dis(gen);
            cd.difficulty = randomVal;

            p.strength = 0.1; //arbitrary init
            p2.strength = 0.1;


            shape->setFillColor(sf::Color(0,255,0, (randomVal*255)));    

            cd.p[0] = p;
            cd.p[1] = p2;
            newC->cellShape = std::move(shape);   
            newC->x = x;
            newC->y = y;
            newC->data = std::move(cd);
            grid.push_back(std::move(newC));
        }
    }


}

void WorldGeneration::generateEntities(int num, int col)
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
    xVal = conf::worldSize.x/2;
    yVal = conf::worldSize.y/2;
    std::pair<int, int> point = {xVal, yVal};
    for(int i = 0; i < col; i ++)
    {
    //    std::random_device rd;
     //   std::mt19937 gen(rd());
     //   std::uniform_int_distribution<> xdis(0.0,width-1);
     //   std::uniform_int_distribution<> ydis(0.0, height-1);

        for(int k = 0; k < num; k ++)
        {
            sf::RectangleShape* rs = new sf::RectangleShape();       



                rs->setSize(sf::Vector2f(cellSize,cellSize));
                rs->setPosition(xVal * cellSize, yVal * cellSize);  
                rs->setOutlineThickness(1.f); 
                rs->setOutlineColor(sf::Color::Red); 
                rs->setFillColor(sf::Color::White);   
                Cell* cell = grid[xVal*width+yVal].get();
                auto et = std::make_unique<Ant>(yVal, xVal, "ant", 10, std::make_unique<sf::RectangleShape>(*rs),cell); 
        
                vis.push_back({yVal, xVal});         


                


                grid[xVal*width+yVal].get()->data.entities.push_back(std::move(et));

        

        }   
        sf::RectangleShape* rs2 = new sf::RectangleShape();      
    
        rs2->setSize(sf::Vector2f(cellSize,cellSize));
        rs2->setPosition(xVal * cellSize, yVal * cellSize);  
        rs2->setOutlineThickness(1.f); 
        rs2->setOutlineColor(sf::Color::Red);
        rs2->setFillColor(sf::Color::Yellow);   
        Cell* cell = grid[xVal*width+yVal].get();
    
        auto et2 = std::make_unique<Location>(xVal, yVal, "Base", 100000, std::make_unique<sf::RectangleShape>(*rs2),cell); //make ant entity
        grid[xVal*width+yVal].get()->data.entities.push_back(std::move(et2));
    
    
        for(auto &i : grid)
        {
            for(auto &k : i.get()->data.entities)
            {
                std::cout << k.get()->getName() << ", " << k.get()->getX() << ", " << k.get()->getX() << std::endl;
            }
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

        std::pair<int, int> point = {xdis(gen)/4, ydis(gen)/4};

        if (std::find(vis.begin(), vis.end(), point) == vis.end()) {
    
            vis.push_back({point.first, point.second});


            double dif = grid[point.second*width+point.first].get()->data.difficulty;

            rs->setSize(sf::Vector2f(cellSize,cellSize));
            rs->setPosition(point.first * cellSize, point.second * cellSize);  
            rs->setOutlineThickness(1.f); 
            rs->setOutlineColor(sf::Color::White);
            rs->setFillColor(sf::Color(255,255,255,dif*100));   


            Cell* cell = grid[point.first*width+point.second].get();

            std::string name = "location" + std::to_string(k);
            auto et = std::make_unique<FoodLocation>(point.first, point.second, name, 100, std::make_unique<sf::RectangleShape>(*rs),cell);
            et.get()->giveResource(100.0);

            grid[point.first*width+point.second].get()->data.entities.push_back(std::move(et));
            
        }
        else{
            k --; //retry this step
        }       

    }  
}