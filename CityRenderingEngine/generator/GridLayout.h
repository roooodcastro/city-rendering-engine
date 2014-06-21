/*
 * Author: Rodrigo Castro Azevedo
 * Date: 13/06/2014
 *
 * Description: The GridLayout is a static interface that may be used to define grid layouts for road network creation.
 * The city may have many grid layouts at the same time, and each one will generate different kinds of road layouts.
 * The most common one, the ManhattanGridLayout, will create rectangular CityBlocks, with semi-random sizes, 
 */

#pragma once

#include <vector>
#include "Chunk.h"
#include "Intersection.h"
#include "Road.h"
#include "../engine/math/Vector2.h"

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

    /* The unique identifier of this type of GridLayout. To be defined by inherited classes. */
    const int GRID_LAYOUT_ID;
};