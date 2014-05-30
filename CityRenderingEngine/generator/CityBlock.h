/*
 * Author: Rodrigo Castro Azevedo
 * Date: 26/05/2014
 * 
 * Description: 
 */

#pragma once

#include <vector>
#include "Intersection.h"
#include "Building.h"

class Building;

class CityBlock {
public:

    CityBlock(void);
    ~CityBlock(void);

    /* Adds an intersection to this CityBlock. */
    void addVertice(Intersection *intersection);

    /* Returns the vector containing the Buildings. */
    std::vector<Building*> *getBuildings() { return buildings; }

    /*
     * This function should be called after all vertices are set. This will calculate the space of the block and decide
     * the number, size and position of buildings inside the block. This will erase all previous buildings, if any, and
     * recreate the buildings vector.
     */
    void generateBuildings();

    /* Returns the center point of this CityBlock, calculated averaging the position of all its Intersections. */
    Vector3 getCentralPosition();

protected:

    /* The Intersections that are "vertices" to this CityBlock. A CityBlock must have at least 3 vertices. */
    std::vector<Intersection*> *vertices;

    /* The Buildings that are part of this CityBlock. */
    std::vector<Building*> *buildings;
};
