/*
 * Author: Rodrigo Castro Azevedo
 * Date: 26/05/2014
 * 
 * Description: This is the main class of the City Rendering Generator. It represents a city environment, storing all
 * its data that are necessary to represent and render it.
 */

#pragma once

#include <vector>
#include "Intersection.h"
#include "CityBlock.h"
#include "../engine/math/Vector2.h"

class City {
public:

    City(void);
    ~City(void);

    /* Returns an intersection at the specified position. If no intersections are found, this will return null. */
    Intersection *getIntersectionAt(Vector3 position);

    /* Returns the vector containint the Intersections. */
    std::vector<Intersection*> *getIntersections() { return intersections; }

    /* Returns the vector containing the CityBlocks. */
    std::vector<CityBlock*> *getCityBlocks() { return cityBlocks; }

    /*
     * Generates a City that is composed of square blocks, like NYC. The parameters width and height relate to the size
     * of the city, in blocks. All roads and intersections are parallel.
     */
    static City *generateManhattanGrid(int witdh, int height);

protected:

    // TODO: turn this into an ordered map, with the position as key, and implement binary search or similar to find.
    std::vector<Intersection*> *intersections;

    /* The CityBlocks that exist in this city. */
    std::vector<CityBlock*> *cityBlocks;
};
