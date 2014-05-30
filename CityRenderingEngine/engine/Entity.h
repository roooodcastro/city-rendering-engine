/*
 * Author: Rodrigo Castro Azevedo
 * Date: 22/05/2014
 *
 * Description: The Entity class is the container for all game objects that aren't part of the interface, and that are
 * part of the game itself. An entity needs to have a 3D Model, a Shader, and its transform matrix and vectors. It can
 * also have a PhysicalBody, if it needs to respond to physics and collision.
 *
 * An entity may have one or more children entities, that are organized in a hierarchy, having all children relative to
 * their parent. This way, if the parent moves, rotates or change scale, its children will also do so, proportionally.
 */

#pragma once

#include <vector>
#include "math/Vector3.h"
#include "math/Matrix4.h"
#include "rendering/Model.h"
#include "rendering/Shader.h"
#include "Naquadah.h"
#include "rendering/Texture.h"
#include "physics/PhysicalBody.h"

class Naquadah;
class Model;
class Shader;

class Entity {
public:

    /* The Transform vectors of the Entity. */
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    Entity(void);
    Entity(const Entity &copy);
    Entity(Vector3 position, Vector3 rotation, Vector3 scale);
    virtual ~Entity(void);

    virtual void update(float millisElapsed);
    virtual void draw(float millisElapsed);

    /* Mouse events */
    virtual void onMouseMoved(Vector2 &position, Vector2 &amount); // Will fire every time the mouse moves
    virtual void onMouseClick(Uint8 button, Vector2 &position); // Will fire once a mouse button is released
    virtual void onMouseDoubleClick(Uint8 button, Vector2 &position); // Will fire on a double click
    virtual void onMouseButtonDown(Uint8 button, Vector2 &position); // Will fire in every tick that a button is down
    virtual void onMouseButtonUp(Uint8 button, Vector2 &position); // Will fire every time a mouse button is released
    virtual void onMouseWheelScroll(int amount); // Will fire every time the mouse wheel scrolls
    /* Keyboard events */
    virtual void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
    virtual void onKeyDown(SDL_Keysym key); // Will fire in every tick that a key is down
    virtual void onKeyUp(SDL_Keysym key); // Will fire every time a key is released

    /* General getters and setters */
    Entity *getParent() { return parent; }
    void setShader(Shader *shader) { this->shader = shader; }
    Shader *getShader() { return shader; }
    Matrix4 getModelMatrix() { return *modelMatrix; }
    void setModel(Model *model) { this->model = model; }
    Model *getModel() { return model; }
    void setPhysicalBody(PhysicalBody &body);
    PhysicalBody *getPhysicalBody() { return physicalBody; }

    /*
     * This functions returns all the children, grandchildren, etc of an entity, recursively. Notice that the function
     * also includes the calling entity on the list.
     */
    static std::vector<Entity*> getAllChildren(Entity *entity);

    /* Adds a new child to this entity */
    void addChild(Entity *child);

    /* Removes a child from this entity's child list */
    void removeChild(Entity *child);

    /* Makes this entity an orphan, removing it from its parent's child list, if this entity is a child. */
    void makeOrphan();

    Entity &operator=(const Entity &other);

protected:

    /* The Transform variables, for the values of the last frame. */
    Vector3 lastPosition;
    Vector3 lastRotation;
    Vector3 lastScale;

    /*
     * Here we get the entity's attributes and calculate the final model matrix. If the entity has children, it will
     * update its child's matrices as well. The three parameter Vector3 are the offset of the Transform, and should be
     * set to make children position, rotate and scale relative to their parents and grandparents. If the Entity don't
     * have any parent, these vectors should be passed at their neutral values. The bool parameters indicate if their
     * correspondent offset Vector3 has changed since the last frame. If they changed, the modelMatrix will have to be
     * updated even if this Entity's Transform didn't change.
     */
    void calculateModelMatrix(Vector3 addPos, Vector3 addRot, Vector3 addSiz, bool pDiff, bool rDiff, bool sDiff);

    /* The final transform matrix applied to this Entity. This is calculated and should not be modified directly. */
    Matrix4 *modelMatrix;

    /*
     * List of child entities for this entity. The attributes of child entities, like position, size and rotation will
     * be relative to its parent, instead of being relative to the world. For example, if and entity is in the position
     * (10, 0, 0), and have a child with the position attribute set to (-2, 0, 0), the child's world position will be
     * actually (8, 0, 0).
     */
    std::vector<Entity*> *childEntities;

    /* The number of childEntities in this Entity. */
    int numChildEntities;

    /*
     * If this entity is the child of another entity, it must know its parent, in order to calculate its world
     * attributes, such as position, velocity and rotation, relative to its parent's.
     */
    Entity *parent;

    /* The 3D model for this entity. Notice this CAN be null, the entity could be just a particle, for example */
    Model *model;

    /* The entity's shader. This is required to render the entity on the screen. */
    Shader *shader;

    /* The physical body of this entity */
    PhysicalBody *physicalBody;
};