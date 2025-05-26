#ifndef ANT_H
#define ANT_H
#include "Entity.h"


class Ant : public Entity
{
    private:
        Entity* target;
        Entity* base;
        std::vector<Cell*> path;
        bool returningHome = false;
    public:
        Ant(int x, int y, std::string name, double maxResource, std::unique_ptr<sf::RectangleShape> hitbox, Cell* currentCell);
    public:
        bool getStatus() {return returningHome;}
        void setStatus(bool stat) {returningHome = stat;}
        Entity* getTarget() const { return target; }
        std::vector<Cell*>& getPath() override {return path;}
        void setTarget(Entity* newTarget) { target = newTarget; }
        void addPath(Cell *newC);
        void setRotation(double angle);
};

#endif