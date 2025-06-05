#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <iostream>
#include "Chunk.h"

//This is the only place where chunks can be modified
//Every system must comply and work through chunk manager
// -> For thread safety later on
// -> For serializing data to load in and out of RAM

struct ChunkWrapper
{
    std::unique_ptr<Chunk> chunk;
    state chunkState;
};

enum state //Manager stores chunks in a hash map and assigns a status to allow access
{
    IN_USE, //can probably be read with no issues (touchwood)
    AVAILABLE, //can have anything done
    QUEUED, //for save
    SERIALIZING // currently getting serialized


};

class ChunkManager
{
    private:
        std::unordered_map<std::pair<int, int>, ChunkWrapper, pair_hash> grid;
    private:
        bool canWrite(/*Hash value?*/); //edit a chunk
        bool canRead(); //use a chunk
        bool canSerialize(); //serialize chunk
    public:
        ChunkManager();
        ~ChunkManager();
        bool isAvailable(std::string type);
        void addChunk(std::pair<int, int> key, ChunkWrapper);
        Chunk* getChunk(int x, int y);
        void saveInRam(int x, int y);

};

#endif