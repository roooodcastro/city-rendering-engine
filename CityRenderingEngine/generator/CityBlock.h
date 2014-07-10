/*
 * Author: Rodrigo Castro Azevedo
 * Date: 26/05/2014
 * 
 * Description: This class represents a City Block, which is basically a primitive polygon that can be formed by three
 * or more vertices and define one city block. CityBlocks can have any size, and may be of different types, depending
 * on where on the city it's located and its size. A CityBlock may have one or more buildings, that are atores as the
 * child entities of the CityBlock, which is also an Entity. It may have one building acting as a small square, or many
 * large buildings forming a city block in the middle of the business and commercial centre.
 */

#pragma once

#include <vector>
#include "Intersection.h"
#include "Building.h"
#include "../engine/Entity.h"
#include "../engine/math/Geom.h"

class Building;

class CityBlock : public Entity {
public:

    CityBlock(void);
    virtual ~CityBlock(void);

    /* Overload of Entity's methods. */
    virtual void update(float millisElapsed);
    //virtual void draw(float millisElapsed);

    /* Adds an intersection to this CityBlock. */
    void addVertice(Intersection *intersection);

    std::vector<Intersection*> *getVertices() { return vertices; }

    /* Returns the number of Chunks that are currently using this CityBlock. */
    int getNumChunksSharing() { return (int) numChunksSharing; }

    /* Tells the CityBlock that another Chunks is now using it. */
    void addChunkSharing() { numChunksSharing++; }

    /* Tells the CityBlock that a Chunk that was previously using it is not using it anymore. */
    void removeChunkSharing() { numChunksSharing--; }

    /*
     * This function should be called after all vertices are set. This will calculate the space of the block and decide
     * the number, size and position of buildings inside the block. This will erase all previous buildings, if any, and
     * recreate the buildings vector.
     */
    void generateBuildings();

    /* Unloads OpenGL resources and references. This function MUST ONLY be called from the render thread. */
    void unloadOpenGL();

    /* Returns the center point of this CityBlock, calculated averaging the position of all its Intersections. */
    Vector3 getCentralPosition();

    /* Calculates and returns the world position of this entity. */
    virtual Vector3 getWorldPosition() {
        return Vector3(position);
    }

protected:

    /*
     * This function will take a polygon made of Vector2s and split it recursively until it reaches a perimeter smaller
     * than the set value for the type of this CityBlock. When it reaches this size, it'll create an empty Building and
     * add it to the final building list, only if the final lot has direct contato with the sides of the CityBlock, ie.
     * with the street. To split a lot, it takes the longest side of the delimitating polygon and traces a line that is
     * perpendicular to it. The lot is divided using this line and the process continues recursively, until the correct
     * size is reached.
     */
    std::vector<Building*> splitLots(std::vector<Vector2> &lotPolygon);

    /* The Intersections that are "vertices" to this CityBlock. A CityBlock must have at least 3 vertices. */
    std::vector<Intersection*> *vertices;

    /* The maximum perimiter that a single Building lot can occupy. This depends on the type of this CityBlock. */
    float maximumPerimeterPerBuilding;

    /* Indicates the number of Chunks that currently have this CityBlock within. Defaults to zero. */
    char numChunksSharing;
};
