#ifndef CONTROLLER_H
#define CONTROLLER_H    

#include "../game/World.h"
#include "../structs/worldObjects.h"
#include <iostream>
#include <sstream>

class Controller
{
    protected:   
        World* world;         
        std::vector<Cell*> selectedCells;
        std::vector<std::string> selectFilter = {"Null"};
        sf::Color selectOverlay;
        void clearSelectedCells();
    public:
        Controller(World* world);
        virtual void update() = 0; //Whatever a controller needs to update
        virtual ~Controller() = default;
        //select cells from to
        virtual void selectCells(sf::Vector2i from, sf::Vector2i to) = 0;
    private: //helper functions    
};

#endif