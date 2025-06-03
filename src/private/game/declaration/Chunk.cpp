#include "Chunk.h"

Chunk::Chunk(int cx, int cy, int size) : chunkX(cx), chunkY(cy), chunkSize(size) {
    cells.reserve(size * size);
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            int worldX = cx * size + x;
            int worldY = cy * size + y;
            auto cell = std::make_unique<Cell>();
            cell->x = worldX;
            cell->y = worldY;
            cells.push_back(std::move(cell));
        }
    }
}

Cell* Chunk::at(int localX, int localY) {
    return cells[localY * chunkSize + localX].get();
}

void Chunk::update() {
    // TODO: implement entity updates per chunk
}



void Chunk::render(sf::RenderWindow& window) {

}

std::vector<std::unique_ptr<Cell>>& Chunk::getCells() {
    return cells;
}
