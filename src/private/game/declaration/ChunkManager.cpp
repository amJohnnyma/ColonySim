#include "../headers/ChunkManager.h"


ChunkManager::ChunkManager()
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