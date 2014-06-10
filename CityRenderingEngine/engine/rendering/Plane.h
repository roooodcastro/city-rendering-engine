/*
 * Author: Rodrigo Castro Azevedo
 * Date: 31/05/2014
 *
 * Description: This ia a plane in 3D space, represented by a normal vector and a distance from the origin. It contains
 * methods to check if a point, sphere or box is in front or behind the plane.
 */

#pragma once

#include "../math/Vector3.h"
#include "../math/Matrix4.h"

class Plane {
public:

    Plane(void) {}
    Plane(Vector3 normal, float distance, bool normalise=false);
    ~Plane(void) {}

    void setNormal(const Vector3 &normal) { this->normal = normal; }
    Vector3 getNormal() const { return normal; }
    void setDistance(float distance) { this->distance = distance; }
    float getDistance() const { return distance; }

    bool isPointInPlane(const Vector3 &position);
    bool isSphereInPlane(const Vector3 &position, float radius);
    bool isBoxInPlane(const Vector3 &minPos, const Vector3 &maxPos);

protected:

    Vector3 normal;
    float distance;
};