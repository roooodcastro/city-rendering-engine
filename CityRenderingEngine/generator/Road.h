/*
 * Author: Rodrigo Castro Azevedo
 * Date: 26/05/2014
 * 
 * Description: A Road is the connecting path between 2 intersections. It's an Entity, its main purpose is to provide
 * an entity to the City to render the roads and streets, but can also be used to find connections between
 * Intersections.
 */

#pragma once

#include <vector>
#include "Intersection.h"
#include "../engine/Entity.h"

class Intersection;

class Road : public Entity {
public:

    Road(void);
    Road(Intersection *pointA, Intersection *pointB);
    virtual ~Road(void);

    Intersection *getPointA() { return pointA; }
    Intersection *getPointB() { return pointB; }

    /*
     * Returns the other Intersection that makes up this road that is not point. This is to get the other Intersection
     * of the connection when you already have the first one, but don't know which is pointA and which is pointB. If
     * point is neither pointA nor pointB, pointA will be erroneously returned.
     */
    Intersection *getOtherEnd(Intersection *point) {
        if (pointA == point)
            return pointB;
        return pointA;
    }

    /* Calculates and returns the world position of this entity. */
    virtual Vector3 getWorldPosition() {
        return Vector3(position);
    }

protected:

    /* One of the Intersection ends of this Road. */
    Intersection *pointA;

    /* The second Intersection end of this Road. */
    Intersection *pointB;
};
