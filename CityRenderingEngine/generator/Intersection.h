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

protected:

    std::vector<Intersection*> *connections;
    std::vector<Road*> *roads;
};
