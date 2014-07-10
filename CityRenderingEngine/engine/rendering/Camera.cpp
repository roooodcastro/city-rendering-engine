#include "Camera.h"

Camera::Camera(void) {
    position = Vector3();
    rotation = Vector3();
    cameraMatrix = Matrix4::Translation(Vector3());
}

Camera::Camera(const Vector3 &position, const Vector3 &rotation) {
    this->position = position;
    this->rotation = rotation;
    buildViewMatrix();
}

Matrix4 Camera::buildViewMatrix() {
    Matrix4 rotationMatrix = Matrix4::Rotation(rotation.x, Vector3(1, 0, 0)) *
        Matrix4::Rotation(rotation.y, Vector3(0, 1, 0)) * Matrix4::Rotation(rotation.z, Vector3(0, 0, 1));
    this->cameraMatrix = rotationMatrix * Matrix4::Translation(-position);
    return cameraMatrix;
}