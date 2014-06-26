/*
 * Author: Rodrigo Castro Azevedo
 * Date: 26/05/2014
 * 
 * Description: This is the main class of the City Rendering Generator. It represents a city environment, storing all
 * its data that are necessary to represent and render it.
 */

#pragma once

#include <vector>
#include <sstream>
#include "Road.h"
#include "../engine/Entity.h"
#include "../engine/Naquadah.h"
#include "../engine/math/Vector3.h"

class Road;

class Intersection : public Entity {
public:

    Intersection(void);
    Intersection(Vector3 position);
    virtual ~Intersection(void);

    /* Connects this intersction to another, creating a connection and a road between them. */
    Road *connectTo(Intersection *other);

    std::vector<Intersection*> *getConnections() { return connections; }

    /* Returns the number of Chunks that are currently using this Intersection. */
    int getNumChunksSharing() { return (int) numChunksSharing; }

    /* Tells the Intersection that another Chunks is now using it. */
    void addChunkSharing() { numChunksSharing++; }

    /* Tells the Intersection that a Chunk that was previously using it is not using it anymore. */
    void removeChunkSharing() { numChunksSharing--; }

    /*
     * Calculates and returns the distance between two Intersection. If one or both Intersections are invalid, it will
     * return -1.
     */
    static float distanceBetween(Intersection *a, Intersection *b);

    /*
     * Iterates through the list and returns the Intersection that is closest, in position, to the origin Intersection.
     * If the list is empty it will return nullptr. It does not return the distance between the two Intersections.
     */
    static Intersection *getClosestIntersectionTo(Intersection *origin, std::vector<Intersection*> *list);

protected:

    std::vector<Intersection*> *connections;
    std::vector<Road*> *roads;

    /* Indicates the number of Chunks that currently have this Intersection within. Defaults to zero. */
    char numChunksSharing;
};
