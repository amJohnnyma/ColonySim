#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "../game/headers/GlobalVars.h"

#include <cstdint>
#include <sstream>
#include <iostream>

using TeamInfo = uint16_t; //then we can upgrade to uint32 if needed

constexpr uint16_t TEAM_MASK     = 0x000F; // 0000 0000 0000 1111 //what team
constexpr uint16_t TYPE_MASK     = 0x0070; // 0000 0000 0111 0000 //what type is it enemy-controller, player-controlled, enemy, player -> unused for now
constexpr uint16_t RESERVED_MASK = 0x0080; // 0000 0000 1000 0000 //reserved bit -> unused for now
constexpr uint16_t PERM_MASK     = 0x0F00; // 0000 1111 0000 0000 //what can it do -> unused for now
constexpr uint16_t STATUS_MASK   = 0xF000; // 1111 0000 0000 0000 //status of object -> unused for now


constexpr uint8_t getTeam(TeamInfo v) {
    return v & TEAM_MASK;
}

constexpr TeamInfo setTeam(TeamInfo v, uint8_t team) {
    return (v & ~TEAM_MASK) | (team & 0x0F);
}

constexpr uint8_t getType(TeamInfo v) {
    return (v & TYPE_MASK) >> 4;
}

constexpr TeamInfo setType(TeamInfo v, uint8_t type) {
    return (v & ~TYPE_MASK) | ((type & 0x07) << 4);
}

constexpr bool getReserved(TeamInfo v) {
    return (v & RESERVED_MASK) != 0;
}

constexpr TeamInfo setReserved(TeamInfo v, bool flag) {
    return flag ? (v | RESERVED_MASK) : (v & ~RESERVED_MASK);
}

constexpr uint8_t getPerm(TeamInfo v) {
    return (v & PERM_MASK) >> 8;
}

constexpr TeamInfo setPerm(TeamInfo v, uint8_t perm) {
    return (v & ~PERM_MASK) | ((perm & 0x0F) << 8);
}

constexpr uint8_t getStatus(TeamInfo v) {
    return (v & STATUS_MASK) >> 12;
}

constexpr TeamInfo setStatus(TeamInfo v, uint8_t status) {
    return (v & ~STATUS_MASK) | ((status & 0x0F) << 12);
}


class Cell;

class Entity
{
protected:
    int x, y;
    std::string name;
    double resource = 0;
    double maxResource =0;
    std::unique_ptr<sf::RectangleShape> hitbox;
    //bits for team//
    TeamInfo teamInfo;

public:
    // Constructor
    Entity(int x, int y, std::string name, double maxResource);
    Entity(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::RectangleShape> hitbox, Cell* currentCell);
    Entity() {}
    virtual ~Entity() {}

    // Getter and setter for position
    void setPos(int x, int y);
    // Function to take resources
    int takeResource(int amount);
    // Function to give resources
    int giveResource(int amount);
    // Additional getter methods for private members
    int getX() const { return x; }
    int getY() const { return y; }
    double getResource() const { return resource; }
    double getMaxResource() const { return maxResource; }
    const std::string& getName() const { return name; }
    sf::RectangleShape* getHitbox() { return hitbox.get(); }
    virtual std::vector<Cell*>& getPath() = 0;
    TeamInfo getTeam() {return teamInfo;}
    bool sameTeam(TeamInfo a, TeamInfo b) {
    return (a & TEAM_MASK) == (b & TEAM_MASK);
}

    // Setters
    
    void setHitbox(std::unique_ptr<sf::RectangleShape> newHitbox) {
        hitbox = std::move(newHitbox);
    }

    
void setX(int newX) { 
    x = newX; 
    hitbox->setPosition(x * conf::cellSize, y * conf::cellSize);
}

void setY(int newY) { 
    y = newY; 
    hitbox->setPosition(x * conf::cellSize, y * conf::cellSize);
}

void setName(const std::string& newName) { name = newName; }

void setResource(double newResource) {
    resource = std::min(newResource, maxResource);
}

void setMaxResource(double newMax) {
    maxResource = newMax;
    if (resource > maxResource) resource = maxResource;
}

void setTeam(TeamInfo ti)
{
    this->teamInfo = ti;
}






};
#endif