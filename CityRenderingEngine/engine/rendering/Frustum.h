/*
 * Author: Rodrigo Castro Azevedo
 * Date: 31/05/2014
 *
 * Description: This is a Frustum class, used to store the six planes that define a view frustum. This frustum will be
 * used by the Scene to check which entities are inside the camera's view and which aren't, so it can cull all entities
 * that are outside the camera's view, so they don't need to be rendered.
 */

#pragma once

#include "../math/Matrix4.h"
#include "../Entity.h"
#include "Plane.h"

class Entity;

class Frustum {
public:

    Frustum(void) {}
    Frustum(const Matrix4 &mvp);
    ~Frustum(void) {}

    /* Updates the Frustum planes by recalculating them, based on the new Matrix4. */
    void updateMatrix(const Matrix4 &mvp);

    /* Returns true if the entity is inside the Frustum, ie. on the positive side of all the six planes. */
    bool isEntityInside(Entity *entity);

protected:

    /* The six planes that make up the Frustum: right, left, bottom, top, far and near. */
    Plane planes[6];
};