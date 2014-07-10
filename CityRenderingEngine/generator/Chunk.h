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

#include "City.h"
#include "Road.h"
#include "CityBlock.h"
#include "Intersection.h"
#include "../engine/Entity.h"
#include "../engine/Resource.h"

class City;

class Chunk : public Entity, Resource {
public:

    /* Chunk size, in meters. Each Chunk will be CHUNK_SIZE X CHUNK_SIZE m². */
    static const int CHUNK_SIZE = 1000;
    /* Size of the subchunks. This MUST be a number that CHUKN_SIZE is divisible by. */
    static const int SUBCHUNK_SIZE = 10;

    Chunk(void);
    Chunk(const Vector2 &position, City *city);
    virtual ~Chunk(void);

    /*
     * Here we get the entity's attributes and calculate the final model matrix. If the entity has children, it will
     * update its child's matrices as well. The three parameter Vector3 are the offset of the Transform, and should be
     * set to make children position, rotate and scale relative to their parents and grandparents. If the Entity don't
     * have any parent, these vectors should be passed at their neutral values. The bool parameters indicate if their
     * correspondent offset Vector3 has changed since the last frame. If they changed, the modelMatrix will have to be
     * updated even if this Entity's Transform didn't change.
     */
    virtual void calculateModelMatrix(Vector3 addPos, Vector3 addRot, Vector3 addSiz,
        bool pDiff, bool rDiff, bool sDiff);

    virtual void update(float millisElapsed);
    virtual void draw(float millisElapsed);

    /*
     * Loads this chunk from file to memory. The chunk to be loaded is the one at position. If the position is invlaid
     * or still doesn't exist, nothing will be loaded.
     */
    virtual void load();

    /* Unloads this chunk from memory. */
    virtual void unload();

    /* Unloads OpenGL resources and references. This function MUST ONLY be called from the render thread. */
    virtual void unloadOpenGL();

    std::vector<Intersection*> *getIntersections() const { return intersections; }
    std::vector<CityBlock*> *getCityBlocks() const { return cityBlocks; }
    std::vector<Road*> *getRoads() const { return roads; }

    /* Adds a new Intersection to this Chunk. */
    void addIntersection(Intersection *intersection);
    
    /* Removes an Intersection from this Chunk. */
    void removeIntersection(Intersection *intersection);

    /* Adds a new Road to this Chunk. */
    void addRoad(Road *road);

    /* Adds a new CityBlock to this Chunk. */
    void addCityBlock(CityBlock *cityBlock);
    
    /* Removes a CityBlock from this Chunk. */
    void removeCityBlock(CityBlock *cityBlock);

    /* Returns the closest Intersection to the intersection provided. */
    Intersection *getClosestIntersectionTo(const Vector3 &position);

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

    /*
     * Returns the position of the centre of this Chunk as a Vector2. This is the same as getPosition(), without the
     * "y" element of the Vector3.
     */
    Vector2 getCentrePos() { return Vector2(position.x, position.z); }

    /* Returns the chunkPosition, the position at the bottom left corner of the Chunk, as a Vector2. */
    Vector2 getChunkPos() {
        float offset = Chunk::CHUNK_SIZE / 2.0f;
        return this->getCentrePos() - Vector2(offset, offset);
    }

    /* Indicates if OpenGL resources have already been safely released by the render main thread. */
    bool isSafeToDelete() { return safeToDelete; }

    /* Sets the bool that indicates if the OpenGL resources were safely released. */
    void setSafeToDelete(bool safeToDelete) { this->safeToDelete = safeToDelete; }

    /* Checks if the Chunk exists as a file. Returns true if ti exists, and false if it needs to be generated. */
    static bool chunkExists(const Vector2 &position);

    /*
     * Loads the Chunk at position from the disk. If the Chunk doesn't exist, doesn't load anything and returns null.
     * If the Chunk is successfully loaded, it's added to the ResourcesManager and it's returned.
     */
    static Chunk *loadChunk(const Vector2 &position, City *city);

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

    /* Calculates and returns the world position of this entity. */
    //virtual Vector3 getWorldPosition() {
        //return Vector3();
    //}

protected:

    /* The Intersections that are in this chunk. */
    std::vector<Intersection*> *intersections;

    /* The Roads that are in this chunk. */
    std::vector<Road*> *roads;

    /* The CityBlocks that are inside this chunk. */
    std::vector<CityBlock*> *cityBlocks;

    /* The City in which this Chunk is loaded. */
    City *city;

    /* Indicates if OpenGL resources have already been safely released by the render main thread. Defaults to false. */
    bool safeToDelete;
};