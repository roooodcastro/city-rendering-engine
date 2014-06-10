#include "Plane.h"

Plane::Plane(Vector3 normal, float distance, bool normalise) {
    if (normalise) {
        float length = sqrt(Vector3::dot(normal, normal));
        this->normal = normal / length;
        this->distance = distance / length;
    } else {
        this->normal = normal;
        this->distance = distance;
    }
}

bool Plane::isSphereInPlane(const Vector3 &position, float radius) {
    return (Vector3::dot(position, normal) + distance > -radius);
}