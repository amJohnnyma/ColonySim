#include "ChunkManager.h"
#include "WorldGeneration.h"
#include <climits>


ChunkManager::ChunkManager(World* world)
: worldGen(std::make_unique<WorldGeneration>(conf::seed, this, conf::cellSize, world)), world(world) 
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
   // std::cout << "Chunk at: " << key.first << ", " << key.second << std::endl;
}

Chunk* ChunkManager::getChunk(int x, int y) {
    if (!this) {
    std::cout << "chunkManager is null!" << std::endl;
    return nullptr;
    }
//std::cout << "grid size: " << grid.size() << std::endl;
    //std::cout << "Looking chunk CM" << std::endl;
    auto it = grid.find({x, y});
    if (it != grid.end() && it->second.chunkState == state::AVAILABLE) {
       // std::cout << "Found chunk CM" << std::endl;
        return it->second.chunk.get();
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
  //  std::cout << "Load" << std::endl;
    for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy)
        {
            int cx = centerChunkX + dx;
            int cy = centerChunkY + dy;
            if (cx < 0 || cy < 0 || hasLoaded(cx,cy) || cx > conf::max_world_size.x || cy > conf::max_world_size.y)
                continue;

              //  std::cout << "Mkae new chunk" << std::endl;
                if (grid.size() < 256) // hard limit
                {
                   // std::cout << "Loaded grid " << grid.size() << std::endl;
                     //std::cout << "World size: " <<  conf::worldSize.x << ", " << conf::worldSize.y << std::endl;
                    // if cx && cy
                    if (cx > conf::max_world_size.x && cy > conf::max_world_size.y)
                    {
                        worldGen->createChunk(conf::max_world_size.x, conf::max_world_size.y, world);                        
                    }
                    else if (cy > conf::max_world_size.y)
                    {
                        worldGen->createChunk(cx, conf::max_world_size.y,world);
                    }
                    else if (cx > conf::max_world_size.x)
                    {
                        worldGen->createChunk(conf::max_world_size.x, cy,world);
                    }
                    else
                    {
                        //std::cout<<"else"<<std::endl;
                        worldGen->createChunk(cx, cy,world);
                    }

                    // if cy
                    // if cx
                    updateWorldSize();
                    // std::cout << "Created chunk: " << cy << ", " << cx << std::endl;
                }
                else{
                    continue;
                }

            // else
            /*
            deserialize the chunk from memory
            */
        }
    }
    //std::cout << "Load fin" << std::endl;
}


void ChunkManager::unloadDistantChunks(int playerChunkX, int playerChunkY, int radius)
{
  //  std::cout << "Unload" << std::endl;
    for (auto it = grid.begin(); it != grid.end(); ) {
        int cx = it->first.first;
        int cy = it->first.second;
        
        if (abs(cx - playerChunkX) > radius || abs(cy - playerChunkY) > radius) {
            /*
            // Save chunk data before deletion
            Chunk* chunk = it->second.get(); //this will require serialization
            saveChunkToFile(cx, cy, chunk); // cache in RAM
            */
            it = grid.erase(it); // Automatically deletes chunk memory
            // -> and then serialize into RAM before deleting
            updateWorldSize();
           // std::cout << "deleted chunk" << std::endl;
        } else {
            ++it;
        }
    }
    //std::cout << "Unload fin" << std::endl;
}
bool ChunkManager::hasLoaded(int x, int y) const {
    return grid.find({x, y}) != grid.end();
}


void ChunkManager::updateWorldSize() {
    if (grid.empty()) {
        conf::worldSize = {0, 0};
        return;
    }

    int minX = INT_MAX, maxX = INT_MIN;
    int minY = INT_MAX, maxY = INT_MIN;

    for (const auto& entry : grid) {
        int x = entry.first.first;
        int y = entry.first.second;

        minX = std::min(minX, x);
        maxX = std::max(maxX, x);
        minY = std::min(minY, y);
        maxY = std::max(maxY, y);
    }

    conf::worldSize.x = (maxX - minX + 1) * conf::chunkSize;
    conf::worldSize.y = (maxY - minY + 1) * conf::chunkSize;
}
