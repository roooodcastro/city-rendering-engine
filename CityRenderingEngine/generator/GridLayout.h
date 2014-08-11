/*
 * Author: Rodrigo Castro Azevedo
 * Date: 13/06/2014
 *
 * Description: The GridLayout is a static interface that may be used to define grid layouts for road network creation.
 * The city may have many grid layouts at the same time, and each one will generate different kinds of road layouts.
 * The most common one, the ManhattanGridLayout, will create rectangular CityBlocks, with semi-random sizes, 
 */

#pragma once

#include "City.h"
#include "math/Perlin.h"

class Chunk;
class Intersection;

class GridLayout {
public:

    Vector2 posMin;
    Vector2 posMax;

    GridLayout(const int layoutId, const Vector2 &posMin, const Vector2 &posMax);
    virtual ~GridLayout(void) {}

    /*
     * Calculates and returns a Vector2 position indicating the position of the intersection in the bounding space.
     * This function will return (-1, -1) if there's no Intersection inside the bounding space provided. 
     */
    virtual Vector2 getIntersectionPosition() = 0;

    /*
     * This function should generate Roads from the newest Intersection created for the chunk. It must ensure that all
     * Roads generated must be valid, and don't overlap other Roads or Intersections.
     */
    virtual void generateRoads(Chunk *chunk, Intersection *newIntersection) = 0;

    /*
     * Generates a new CityBlock using a variation of the Gift-wrapping algorithm. The start Intersection is the start
     * point of the algorithm. It will use the cityBlockAngle to determine the best next Intersection to add to the
     * CityBlock. Once the algorithm reaches the start point again, the City Block is created and added to the Chunk.
     */
    virtual CityBlock *generateCityBlock(Chunk *chunk, Intersection *start);

    /* The unique identifier of this type of GridLayout. To be defined by inherited classes. */
    const int GRID_LAYOUT_ID;
};