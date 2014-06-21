/*
 * Author: Rodrigo Castro Azevedo
 * Date: 13/06/2014
 *
 * Description: The GridLayout is a static interface that may be used to define grid layouts for road network creation.
 * The city may have many grid layouts at the same time, and each one will generate different kinds of road layouts.
 * The most common one, the ManhattanGridLayout, 
 */

#pragma once

#include <vector>
#include "../GridLayout.h"

class ManhattanGridLayout : public GridLayout {
public:

    static const int MANHATTAN_GRID_LAYOUT_ID = 1;

    ManhattanGridLayout(const Vector2 &posMin, const Vector2 &posMax);

    virtual ~ManhattanGridLayout(void) {}

    /*
     * Calculates and returns a Vector2 position indicating the position of the intersection in the bounding space.
     * This function will return (-1, -1) if there's no Intersection inside the bounding space provided. 
     */
    virtual Vector2 getIntersectionPosition();

    /*
     * This function should generate Roads from the newest Intersection created for the chunk. It must ensure that all
     * Roads generated must be valid, and don't overlap other Roads or Intersections. It only generates straight roads,
     * based on the Manhattan layout.
     */
    virtual void generateRoads(Chunk *chunk, Intersection *newIntersection);
};