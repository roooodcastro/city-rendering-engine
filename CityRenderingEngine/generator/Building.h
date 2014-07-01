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
#include "../engine/math/Vector2.h"
#include "../engine/math/Triangulation.h"

class CityBlock;

class Building : public Entity {
public:

    Building(void);
    Building(CityBlock *cityBlock, Vector3 blockPosition);
    /* Constructor with a vector of Vector2 to delimiter the area that this Building will occupy. */
    Building(std::vector<Vector2> lotArea, CityBlock *cityBlock);

    virtual ~Building(void);

    virtual void draw(float millisElapsed);

    /*
     * This function should only be called after both the lotArea and the type attributes are set. It will define and
     * build the 3D model for the building. If the building already has a 3D model, it will erase and unload the older
     * one and recreate it.
     */
    void constructGeometry();

    /* Returns a unique name identifier for this Building. */
    std::string getEntityName() {
        std::stringstream name;
        Vector3 worldPos = getWorldPosition();
        name << "BUILDING_" << worldPos.x << "-" << worldPos.y << "-" << worldPos.z;
        return name.str();
    }

protected:

    /* A polygon that delimiters the area in which the Building must be. */
    std::vector<Vector2> *lotArea;

    /* The CtyBlock in which this building is located. */
    CityBlock *cityBlock;
};
