/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This class represents the physical body of the object. It contains the different attributes that a physical
 * object can have, except for velocity and acceleration. Because I'm using Verlet integration, the velocity
 * can easily be calculated using the actual and previous position of the body, and the acceleration can also be calculated
 * using the forces acting on the body and its mass.
 *
 * To manage collisions, a physical body may have zero, one or more than one collision bodies. If a physical body has no
 * collision bodies, it will not collide with anything and the collision algorithm will ignore it. If it has one or more,
 * collisions will be calculated for each of the bodies and summed and averaged up to respond to it.
 */

#pragma once

#include <vector>
#include "../math/Vector3.h"
#include "Simulation.h"
#include "collision/CollisionBody.h"
#include "../Entity.h"

class CollisionBody;
class Entity;

class PhysicalBody {
public:
	PhysicalBody(void);
	PhysicalBody(const PhysicalBody &copy);
	PhysicalBody(Entity *entity, float mass, Vector3 &position);
	~PhysicalBody(void);

	PhysicalBody &operator=(const PhysicalBody &other);

	/*
	 * This method calculates and updates the body's physical properties,
	 * calculating and applying forces, and integrating the position for the
	 * next frame.
	 */
	void update(float millisElapsed);

	/*
	 * Calculates and updates the position of this body for the next frame
	 */
	void integrateNextFrame(float millisElapsed);

	/*
	 * Calculates if this body is in rest state.
	 * Rest state should be when it has no speed and no forces acting on it.
	 * If gravity is turned on, the body cannot enter rest state, because
	 * gravity will always be trying to pull it down. The same happens if
	 * there is an external force acting on the body.
	 * The deltaT parameter is necessary because we need to calculate velocity
	 * to determine if the body is in rest
	 */
	bool isAtRest(float millisElapsed);

	/* General getters and setters */
	void setPosition(Vector3 &position);
	Vector3 *getPosition() { return position; }
	/* Returns the position of this body relative to the world, not to its parent */
	Vector3 getAbsolutePosition();
	void setLastPosition(Vector3 &lastPosition) { *(this->lastPosition) = lastPosition; }
	Vector3 *getLastPosition() { return lastPosition; }
	void setRotation(Vector3 &rotation) { *(this->rotation) = rotation; }
	Vector3 *getRotation() { return rotation; }
	void setScale(Vector3 &scale) { *(this->scale) = scale; }
	Vector3 *getScale() { return scale; }
	/* Returns the scale of this body relative to the world, not to its parent */
	Vector3 getAbsoluteScale();
	void setForce(Vector3 &force) { *(this->force) = force; }
	void addForce(Vector3 &force) { *(this->force) += force; }
	Vector3 *getForce() { return force; }
	void setMass(float mass) { this->mass = mass; }
	float getMass() { return mass; }
	/* Returns true of the body has moved since the last frame, false otherwise */
	bool hasMoved() { return *lastPosition != *position; }
	void setElasticity(float elasticity) { this->elasticity = elasticity; }
	float getElasticity() { return elasticity; }
	void setDragFactor(float dragFactor) { this->dragFactor = dragFactor; }
	float getdragFactor() { return dragFactor; }
	/* Velocity is an indirect attribute, so we need to calculate it using the deltaT */
	void setVelocity(Vector3 &velocity, float millisElapsed);
	Vector3 getVelocity(float millisElapsed);
	/* Acceleration here is the outside acceleration, excluding drag or gravity. These are calculated elsewhere. Accel is also an indirect value */
	void setAcceleration(Vector3 &acceleration);
	Vector3 getAcceleration(); // Acceleration of the external forces
	Vector3 getTotalAcceleration(float millisElapsed); // Acceleration of the external forces AND gravity and drag
	void setCanMove(bool canMove) { this->moveable = canMove; }
	bool canMove() { return moveable; }
	std::vector<CollisionBody*> *getCollisionBodies() { return collisionBodies; }
	void addCollisionBody(CollisionBody *colBody);
	void removeCollisionBody(CollisionBody *colBody);
	void setCollisionGroup(int collisionGroup) { this->collisionGroup = collisionGroup; }
	int getCollisionGroup() { return collisionGroup; }
	void setEntity(Entity *entity) { this->entity = entity; }
	Entity *getEntity() { return entity; }
	int getUnprocessedCollisions() { return unprocessedCollisions; }

	/* Checks if this physical body has any collision bodies */
	bool isCollideable();

	/*
	 * Checks whether 2 bodies collided and respond to that collision.
	 * If both bodies are spheres, update the position and speed of both to
	 * correct the inconsistency and bounce them off.
	 * If one of them is a plane, bounce the ball off it, always using
	 * elasticity = 1.0.
	 * If both of them are planes, don't calculate or do anything, planes
	 * don't interact with each other in this simulation
	 */
	static void checkCollision(PhysicalBody *body1, PhysicalBody *body2, float deltaT);

protected:
	
	// World position of the body. Defaults to (0, 0, 0)
	Vector3 *position;
	// World position of the body in the previous frame. Defaults to (0, 0, 0)
	Vector3 *lastPosition;
	// World rotation of the body. Defaults to (0, 0, 0)
	Vector3 *rotation;
	// Scale factor of the body. This does not influence mass directly. Defaults to (1, 1, 1)
	Vector3 *scale;
	// Sum of external forces applied to the body. Defaults to (0, 0, 0)
	Vector3 *force;

	// The mass of the body, defaults to 1.0
	float mass;
	// The elasticity of the body, defaults to 1.0
	float elasticity;
	// The drag factor of the body, 0 being totally aerodynamic, 1 being full drag. Defaults to 0.
	float dragFactor;
	/* Indicates if the body can have acceleration or velocity */
	bool moveable;
	/* The collision group of this body. The body will NOT collide with other bodies of the same group */
	int collisionGroup;
	/* The vector of all collision bodies acting on this physical body*/
	std::vector<CollisionBody*> *collisionBodies;
	/* The entity that represents this physical body */
	Entity *entity;
	/* The number of collisions that weren't processed yet */
	int unprocessedCollisions;
};