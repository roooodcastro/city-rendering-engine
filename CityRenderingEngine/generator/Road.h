/*
 * Author: Rodrigo Castro Azevedo
 * Date: 26/05/2014
 * 
 * Description: 
 */

#pragma once

#include <vector>
#include "Intersection.h"

class Intersection;

class Road {
public:

    Road(void);
    ~Road(void);

protected:

    Intersection *pointA;
    Intersection *pointB;
};
