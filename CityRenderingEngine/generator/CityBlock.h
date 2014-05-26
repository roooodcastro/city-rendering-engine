/*
 * Author: Rodrigo Castro Azevedo
 * Date: 26/05/2014
 * 
 * Description: 
 */

#pragma once

#include <vector>
#include "Intersection.h"

class CityBlock {
public:

    CityBlock(void);
    ~CityBlock(void);

protected:

    std::vector<Intersection*> *vertices;
};
