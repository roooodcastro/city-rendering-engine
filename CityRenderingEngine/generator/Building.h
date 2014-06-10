/*
 * Author: Rodrigo Castro Azevedo
 * Date: 26/05/2014
 * 
 * Description: 
 */

#pragma once

#include <vector>
#include "CityBlock.h"
#include "../engine/Entity.h"
#include "../engine/math/Common.h"

class CityBlock;

class Building : public Entity {
public:

    Building(void);
    Building(CityBlock *cityBlock, Vector3 blockPosition);
    virtual ~Building(void);

protected:

    /* The CtyBlock in which this building is located. */
    CityBlock *cityBlock;
};
