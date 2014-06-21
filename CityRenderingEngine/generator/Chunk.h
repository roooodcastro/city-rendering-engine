/*
 * Author: Rodrigo Castro Azevedo
 * Date: 11/06/2014
 * 
 * Description: A Chunk is a region of the city that has the fixed size of 1 km squared (1km x 1km). It's basically a
 * subdivision of the city, that is used to split the (infinite) city into manageable chunks that can be saved as files
 * and loaded when needed. Only the chunks close to the "player" will be loaded, as as the "player" moves around, the
 * chunks will load and unload as necessary, and if the chunk to be loaded doesn't exist, it will be generated and
 * loaded.
 *
 * Chunks are located using a Vector2 that stores its position in world space. This position is the absolute XZ
 * position of the bottom left corner of the chunk. The oposite corner of the chunk will be in the position (X + 1000,
 * Z * 1000). This means that the center of the chunk will not be (X, Z), but (X + 500, Z + 500) instead.
 *
 * A chunk contains a vector with all the intersections that are inside it and also all intersections that are outside
 * it but are necessary to define all CityBlocks that are inside the Chunl. So if there's a CityBlock on the edge of
 * the chunk, the Chunk will have all of its Intersections, and the chunk next to it will also have the same
 * Intersections. This duplicates some Intersections when saving to file, using up more space in disk, but ensures that
 * when loading a chunk, all CityBlocks will be correctly loaded.
 */

#pragma once

#include <vector>
#include "CityBlock.h"
#include "Intersection.h"
#include "../engine/Entity.h"
#include "../engine/Resource.h"
#include "../engine/math/Vector2.h"

class Intersection;

class Chunk : public Entity, Resource {
public:

    /* Chunk size, in meters. Each Chunk will be CHUNK_SIZE X CHUNK_SIZE m². */
    static const int CHUNK_SIZE = 1000;
    /* Size of the subchunks. This MUST be a number that CHUKN_SIZE is divisible by. */
    static const int SUBCHUNK_SIZE = 10;

    Chunk(void);
    Chunk(const Vector2 &position);
    virtual ~Chunk(void);

    virtual void update(float millisElapsed);
    virtual void draw(float millisElapsed);

    /*
     * Loads this chunk from file to memory. The chunk to be loaded is the one at position. If the position is invlaid
     * or still doesn't exist, nothing will be loaded.
     */
    virtual void load();

    /* Unloads this chunk from memory. */
    virtual void unload();

    std::vector<Intersection*> *getIntersections() const { return intersections; }
    std::vector<CityBlock*> *getCityBlocks() const { return cityBlocks; }
    std::vector<Road*> *getRoads() const { return roads; }

    /* Returns the position of this Chunk as a Vector2, for Chunk calculations. */
    Vector2 getChunkPos() { return Vector2(position.x, position.z); }

    /* Adds a new Intersection to this Chunk. */
    void addIntersection(Intersection *intersection);
    
    /* Removes an Intersection from this Chunk. */
    void removeIntersection(Intersection *intersection);

    /* Adds a new Road to this Chunk. */
    void addRoad(Road *road);

    /* Adds a new CityBlock to this Chunk. */
    void addCityBlock(CityBlock *cityBlock);
    
    /* Returns the closest Intersection to the intersection provided. */
    Intersection *getClosestIntersectionTo(Intersection *intersection);

    /* Returns the closest Intersections to the intersection provided. */
    std::vector<Intersection*> getClosestIntersectionsTo(Intersection *intersection, int number);

    /*
     * Saves this Chunk to its file. If the file already exists, this will override its contents. This function should
     * only be called when there's an update to the Chunk's data, such as a new building, or a modification to an
     * existing one. This will automatically locate the chunk's file, using the path that's set on City.
     */
    void saveToFile();

    /* Generates and returns the filename for this chunk. */
    std::string getFileName();

    Vector3 getWorldPosition() {
        float offset = Chunk::CHUNK_SIZE / 2.0f;
        return this->position + Vector3(offset, 0, offset);
    }

    /* Checks if the Chunk exists as a file. Returns true if ti exists, and false if it needs to be generated. */
    static bool chunkExists(const Vector2 &position);

    /*
     * Loads the Chunk at position from the disk. If the Chunk doesn't exist, doesn't load anything and returns null.
     * If the Chunk is successfully loaded, it's added to the ResourcesManager and it's returned.
     */
    static Chunk *loadChunk(const Vector2 &position);

    static std::string getFileName(const Vector2 &position) {
        std::stringstream fileName;
        fileName << position.x << "-" << position.y << ".cnk";
        return fileName.str();
    }

    std::string getEntityName() {
        std::stringstream name;
        name << "CHUNK_" << position.x << "-" << position.z;
        return name.str();
    }

protected:

    /* The Intersections that are in this chunk. */
    std::vector<Intersection*> *intersections;

    /* The Roads that are in this chunk. */
    std::vector<Road*> *roads;

    /* The CityBlocks that are inside this chunk. */
    std::vector<CityBlock*> *cityBlocks;
};
