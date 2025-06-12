#ifndef CELL_H
#define CELL_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../structs/worldObjects.h"


class Cell {
    public:
        std::unique_ptr<Shape> cellShape;
        int x, y;
        CellData data;
        sf::Color originalColor;

        sf::Color lerpColor(sf::Color base, sf::Color overlay, float weight);

    public:
        Cell() {}
        Cell(int x, int y, CellData &&data, sf::Color color);
        // Cell(const Cell& other);
        // Cell& operator=(const Cell& other);
        ~Cell();

        void setColor(sf::Color col);
        void setFilterOverlay(sf::Color overlay, float weight = 0.5f);
        void restoreColor();

        // Getters
        int getX() const { return x; }
        int getY() const { return y; }
        const sf::Color& getOriginalColor() const { return originalColor; }
        const CellData& getData() const { return data; }
        Shape* getShape() const { return cellShape.get(); }

        void setShape(std::unique_ptr<Shape> shape) { cellShape = std::move(shape); }


};

#endif