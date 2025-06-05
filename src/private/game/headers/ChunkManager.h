#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <iostream>

//This is the only place where chunks can be modified
//Every system must comply and work through chunk manager
// -> For thread safety later on
// -> For serializing data to load in and out of RAM

enum state //Manager stores chunks in a hash map and assigns a status to allow access
{
    IN_USE,
    AVAILABLE,
    QUEUED, //for save
    SERIALIZING // currently getting serialized


};

class ChunkManager
{
    private:
        bool canWrite(); //edit a chunk
        bool canRead(); //use a chunk
        bool canSerialize(); //serialize chunk
    public:
        ChunkManager();
        ~ChunkManager();
        bool isAvailable(std::string type);

};

#endif