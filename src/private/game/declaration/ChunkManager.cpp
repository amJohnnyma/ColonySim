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
    std::cout << "Chunk at: " << key.first << ", " << key.second << std::endl;
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

void ChunkManager::ensureChunksAround(int centerChunkX, int centerChunkY, int radius) {
    for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy) {
            int cx = std::max(centerChunkX + dx,0);
            int cy = std::max(centerChunkY + dy,0);
            if (grid.find({cy, cx}) == grid.end()) {
                if(grid.size() < 100) //hard limit
                {
                std::cout << grid.size() << std::endl;
                conf::worldSize.x += conf::chunkSize;
                conf::worldSize.y += conf::chunkSize;
                worldGen->createChunk(cx, cy); // returns unique_ptr<Chunk>
                std::cout << "Created chunk: " << cy << ", " << cx << std::endl;

                }
            }
        }
    }
}


void ChunkManager::unloadDistantChunks(int playerChunkX, int playerChunkY, int radius)
{
    for (auto it = grid.begin(); it != grid.end(); ) {
        int cx = it->first.second;
        int cy = it->first.first;

        if (abs(cx - playerChunkY) > radius || abs(cy - playerChunkX) > radius) {
            /*
            // Save chunk data before deletion
            Chunk* chunk = it->second.get(); //this will require serialization
            saveChunkToFile(cx, cy, chunk); // cache in RAM
            */
            it = grid.erase(it); // Automatically deletes chunk memory
            conf::worldSize.x -= conf::chunkSize;
            conf::worldSize.y -= conf::chunkSize;            
            std::cout << "deleted chunk" << std::endl;
        } else {
            ++it;
        }
    }
}