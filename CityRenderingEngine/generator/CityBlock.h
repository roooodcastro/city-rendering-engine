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
};
