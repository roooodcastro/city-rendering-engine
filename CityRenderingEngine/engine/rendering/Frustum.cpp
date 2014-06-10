#include "Frustum.h"

Frustum::Frustum(const Matrix4 &mvp) {
    updateMatrix(mvp);
}

void Frustum::updateMatrix(const Matrix4 &mvp) {
    Vector3 xaxis = Vector3(mvp.values[0], mvp.values[4], mvp.values[8]);
    Vector3 yaxis = Vector3(mvp.values[1], mvp.values[5], mvp.values[9]);
    Vector3 zaxis = Vector3(mvp.values[2], mvp.values[6], mvp.values[10]);
    Vector3 waxis = Vector3(mvp.values[3], mvp.values[7], mvp.values[11]);

    // Right plane
    planes[0] = Plane(waxis - xaxis, (mvp.values[15] - mvp.values[12]), true);
    // Left plane
    planes[1] = Plane(waxis + xaxis, (mvp.values[15] + mvp.values[12]), true);
    // Bottom plane
    planes[2] = Plane(waxis + yaxis, (mvp.values[15] + mvp.values[13]), true);
    // Top plane
    planes[3] = Plane(waxis - yaxis, (mvp.values[15] - mvp.values[13]), true);
    // Far plane
    planes[4] = Plane(waxis - zaxis, (mvp.values[15] - mvp.values[14]), true);
    // Near plane
    planes[5] = Plane(waxis + zaxis, (mvp.values[15] + mvp.values[14]), true);
}

bool Frustum::isEntityInside(Entity *entity) {
    for (int p = 0; p < 6; p++) {
        if (!planes[p].isSphereInPlane(entity->getWorldPosition(), entity->getRenderRadius())) {
            return false;
        }
    }
    return true;
}