#include "Cell.h"

Cell::Cell(int x, int y, CellData&& data, sf::Color color)
    : x(x), y(y), data(std::move(data)), originalColor(color) {}


// Cell::Cell(const Cell& other)
//     : x(other.x), y(other.y), data(other.data), originalColor(other.originalColor) {
//     if (other.cellShape)
//         cellShape = other.cellShape->clone();  // Requires `Shape::clone()`
// }

// Cell& Cell::operator=(const Cell& other) {
//     if (this == &other) return *this;
//     x = other.x;
//     y = other.y;
//     data = other.data;
//     originalColor = other.originalColor;
//     cellShape = other.cellShape ? other.cellShape->clone() : nullptr;
//     return *this;
// }

Cell::~Cell() {}

void Cell::setColor(sf::Color col) {
    originalColor = col;
    if (cellShape) cellShape->setFillColor(col);
}

void Cell::setFilterOverlay(sf::Color overlay, float weight) {
    if (!cellShape) return;
    sf::Color base = cellShape->getColor();
    cellShape->setFillColor(lerpColor(base, overlay, weight));
}

sf::Color Cell::lerpColor(sf::Color base, sf::Color overlay, float weight) {
    sf::Uint8 r = static_cast<sf::Uint8>(base.r * (1.0f - weight) + overlay.r * weight);
    sf::Uint8 g = static_cast<sf::Uint8>(base.g * (1.0f - weight) + overlay.g * weight);
    sf::Uint8 b = static_cast<sf::Uint8>(base.b * (1.0f - weight) + overlay.b * weight);
    sf::Uint8 a = static_cast<sf::Uint8>(base.a * (1.0f - weight) + overlay.a * weight);
    return sf::Color(r, g, b, a);
}

void Cell::restoreColor() {
    if (cellShape)
        cellShape->setFillColor(originalColor);
}
