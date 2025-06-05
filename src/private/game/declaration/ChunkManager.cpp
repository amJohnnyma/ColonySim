#include "../headers/ChunkManager.h"
#include "../headers/WorldGeneration.h"


ChunkManager::ChunkManager()
: worldGen(std::make_unique<WorldGeneration>(conf::seed, this, conf::cellSize)) 
{
    
}


ChunkManager::~ChunkManager()
{
}

bool ChunkManager::isAvailable(std::string type)
{
    return false;
}

void ChunkManager::addChunk(std::pair<int, int> key, ChunkWrapper newChunk)
{
    grid[key] = std::move(newChunk);
}

Chunk *ChunkManager::getChunk(int x, int y)
{
    if(grid[{x,y}].chunkState == state::AVAILABLE)
    {
        return grid[{x,y}].chunk.get();
    }
    return nullptr;
}

bool ChunkManager::canWrite()
{
    return false;
}

bool ChunkManager::canRead()
{
    return false;
}

bool ChunkManager::canSerialize()
{
    return false;
}