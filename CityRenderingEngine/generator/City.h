/*
 * Author: Rodrigo Castro Azevedo
 * Date: 26/05/2014
 * 
 * Description: This is the main class of the City Rendering Generator. It represents a city environment, storing all
 * its data that are necessary to represent and render it.
 */

#pragma once

#include <SDL.h>
#include <vector>
#include "Chunk.h"
#include "ChunkGenerator.h"
#include "../engine/math/Vector2.h"
#include "../engine/math/Vector3.h"

class Road;
class Chunk;
class CityBlock;
class Intersection;
class ChunkGenerator;

class City {
public:

    /* The maximum number of Chunks that are allowed to be loading at the same time. */
    static const int MAX_PARALEL_LOADING_CHUNKS = 1;

    City(void);
    ~City(void);

    /* Returns an intersection at the specified position. If no intersections are found, this will return null. */
    Intersection *getIntersectionAt(Vector3 position);

    /* Returns the vector containint the Intersections. */
    std::vector<Intersection*> *getIntersections() { return intersections; }

    /* Returns the vector containing the CityBlocks. */
    std::vector<CityBlock*> *getCityBlocks() { return cityBlocks; }

    /* Returns the vector containing the Chunks. */
    std::vector<Chunk*> *getChunks() { return chunks; }

    /*
     * Adds a Chunk to the City. The Chunk should be fully loaded before it's added. The Chunk will also be added to
     * the current Scene.
     */
    void addChunk(Chunk *chunk);

    /*
     * Unloads and removes the chunk from memory. This will remove the Chunk from the list of loaded Chunks of the
     * city, and also unload it from memory, requiring it to be loaded again.
     */
    void unloadChunk(Chunk *chunk);

    /*
     * Requests that the Chunk at position chunkPos be loaded OR generated. This will delegate the task to a worker
     * thread, which will add the new Chunk to the list, and the function isChunkLoaded() will be able to determine
     * when the Chunk finished loading. chunkPos is a world space position, and MUST be in multiples of 1000.
     */
    void loadChunk(const Vector2 &chunkPos);

    /*
     * Checks if the Chunk at chunkPos is currently loaded. It doesn't check if the Chunk already exists or needs to be
     * generated.
     */
    bool isChunkLoaded(const Vector2 &chunkPos);

    /* Tells the City that the chunk has finished loading. This tells the City that the next Chunk may be loaded. */
    void flagChunkAsLoaded(Chunk *chunk);

    /*
     * Returns the Chunk at chunkPos. If the Chunk is not already loaded, it'll load from disk if the flag is set. If
     * the Chunk also doesn't exist on disk, or if it's not loaded and the flag is set to false, null is returned.
     */
    // TODO: do the loading from disk part
    Chunk *getChunkAt(const Vector2 &chunkPos, bool loadFromDisk);

    /*
     * Returns a list of the eight neighbour Chunks to chunk. If aneighbour Chunk is not loaded, it will be loaded if
     * and only if loadFromDisk is set to true. If the Chunk is not loaded or does not exist, the list will not contain
     * it, so the final list may have a size between 0 and 8 Chunks. This will not generate new Chunks.
     */
    // TODO: do the loading from disk part
    std::vector<Chunk*> getNeighbourChunks(Chunk *chunk, bool loadFromDisk);

    /*
     * Generates a City that is composed of square blocks, like NYC. The parameters width and height relate to the size
     * of the city, in blocks. All roads and intersections are parallel.
     */
    static City *generateManhattanGrid(int witdh, int height);

    /* Locks and unlocks the mutex, to prevent errors while accessing and editting the entities map */
    void lockMutex();
    void unlockMutex();

protected:

    /* A vector to store the position of all the Chunks that are currently being loaded. */
    std::vector<Vector2> chunksLoading;

    // TODO: turn this into an ordered map, with the position as key, and implement binary search or similar to find.
    std::vector<Intersection*> *intersections;

    /* The CityBlocks that exist in this city. */
    std::vector<CityBlock*> *cityBlocks;

    /* The loaded chunks of the city. */
    std::vector<Chunk*> *chunks;

    /* Mutex to prevent errors caused by racing conditions, especially when loading Chunks on worlker threads. */
    SDL_mutex *mutex;
};