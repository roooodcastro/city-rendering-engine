/*
 * Author: Rodrigo Castro Azevedo
 * Date: 26/05/2014
 * 
 * Description: This is the main class of the City Rendering Generator. It represents a city environment, storing all
 * its data that are necessary to represent and render it.
 */

#pragma once

#include <vector>
#include "Road.h"

class Road;

class Intersection {
public:

    Intersection(void);
    ~Intersection(void);

protected:

    std::vector<Intersection*> *connections;
    std::vector<Road*> *roads;
};
