#pragma once

#include <vector>
#include "math/Vector3.h"
#include "math/Matrix4.h"
#include "rendering/Model.h"
#include "rendering/Shader.h"
#include "Naquadah.h"
#include "rendering/Texture.h"
//#include "BoundingBox.h"
//#include "PhysicalBody.h"

class Naquadah;
class Model;
class Shader;
//class PhysicalBody;

class Entity {
public:
	Entity(void);
	Entity(const Entity &copy);
	Entity(Vector3 &position, Vector3 &velocity, Vector3 &rotation, Vector3 &scale);
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
	void setCustomShader(Shader &shader);
	Shader *getCustomShader() { return customShader; }
	Matrix4 getModelMatrix() { return *modelMatrix; }
	void setModel(Model *model) { this->model = model; }
	Model *getModel() { return model; }
	//void setPhysicalBody(PhysicalBody &body);
	//PhysicalBody *getPhysicalBody() { return physicalBody; }

	/*
	 * This functions returns all the children, grandchildren, etc of an entity, recursively.
	 * Notice that the function also includes the parameter Entity in the list
	 */
	static std::vector<Entity*> getAllChildren(Entity *entity);

	/* Adds a new child to this entity */
	void addChild(Entity *child);

	/* Removes a child from this entity's child list */
	void removeChild(Entity *child);

	/*
	 * Makes this entity an orphan, removing it from its parent's child list, if this entity is a child.
	 */
	void makeOrphan();

	Entity &operator=(const Entity &other);

protected:

	/*
	 * Here we get the entity's attributes and calculate the final model matrix.
	 * If the entity has children, it will update its child's matrices as well.
	 * To save time, this method will be called once in every tick, instead of
	 * calling it for every frame (the result will only be different after a tick
	 * anyway).
	 */
	void calculateModelMatrix();

	Matrix4 *modelMatrix;

	/*
	 * List of child entities for this entity. A child entity can be anything, from an attachment part to spark particles.
	 * The attributes of child entities, like position, size and rotation will be relative to its parent, instead of being
	 * relative to the world. For example, if and entity is in the position (10, 0, 0), and have a child with the position
	 * attribute set to (-2, 0, 0), the child's world position will be actually (8, 0, 0).
	 */
	std::vector<Entity*> *childEntities;

	/*
	 * If this entity is the child of another entity, it must know its parent, in order to calculate its world
	 * attributes, such as position, velocity and rotation, relative to its parent's.
	 */
	Entity *parent;

	/*
	 * The 3D model for this entity. Notice this CAN be null, the entity could be just a particle, for example
	 */
	Model *model;

	/*
	 * This entity's custom shader. This is optional and will be replaced by the default shader loaded in GameApp if not specified.
	 */
	Shader *customShader;

	/*
	 * The physical body of this entity
	 */
	//PhysicalBody *physicalBody;
};