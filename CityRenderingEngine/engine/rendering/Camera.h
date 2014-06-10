/*
 * Author: Rodrigo Castro Azevedo
 * Date: 31/05/2014
 *
 * Description: This is the Camera for the Scene. A Scene can only have one camera active at a time, but many cameras
 * may exist at the same time. A Camera has only a position and a rotation, while the Scene will control the camera's
 * projection.
 *
 * The camera will also build and return the viewMatrix for the Scene when requested.
 */

#pragma once

#include "../math/Vector3.h"
#include "../math/Matrix4.h"

class Camera {
public:

    Camera(void);
    Camera(const Vector3 &position, const Vector3 &rotation);
    ~Camera(void) {}

    /* Getters and setters. */
    void setPosition(const Vector3 &position) { this->position = position; }
    void setRotation(const Vector3 &rotation) { this->rotation = rotation; }
    void setChanged(bool changed) { this->changed = changed; }
    Vector3 getPosition() const { return position; }
    Vector3 getRotation() const { return rotation; }
    bool hasChanged() { return changed; }

    /* Moves the camera in world space by amount. */
    void moveCamera(const Vector3 &amount) { this->position += amount; changed = true; }
    /* Rotates the camera in world space by amount on each axis. */
    void rotateCamera(const Vector3 &amount) { this->rotation += amount; changed = true; }

    /* Builds and returns the ViewMatrix for the camera. */
    Matrix4 buildViewMatrix();

protected:

    /* The camera's position in world space. Defaults to (0, 0, 0). */
    Vector3 position;

    /* The camera's rotation in world space. Defaults to (0, 0, 0). */
    Vector3 rotation;

    /* Indicates if the camera has changed position or rotation. */
    bool changed;

};