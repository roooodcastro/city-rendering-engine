/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: A collision body is the body that will be used to calculate and respond to collisions.
 * A physicalBody may have none, one or many collisionBodies, depending on the shape of the body.
 * A collision body can be a sphere, a plane or an inverted cylinder, the later being a cilinder
 * that is open on the inside and responds to collisions on its inside walls.
 *
 * The inverted cylinder will always be a cylinder running along the Z axis.
 */

#pragma once

#include "../../math/Vector3.h"
#include "../PhysicalBody.h"

class PhysicalBody;

enum BodyType {
	SPHERE = 0,
	INV_CYLINDER = 1,
	PLANE = 2,
};

struct CollisionResult;

class CollisionBody {
public:

	CollisionBody(void);
	CollisionBody(PhysicalBody *parent, BodyType type);
	/* Creates a sphere */
	CollisionBody(PhysicalBody *parent, Vector3 &position, float radius);
	/* Creates an inverted cylinder */
	CollisionBody(PhysicalBody *parent, Vector3 &position, float radius, float length, Vector3 &direction);
	/* Creates a place */
	CollisionBody(PhysicalBody *parent, Vector3 &position, Vector3 &normal, float distance);
	CollisionBody(const CollisionBody &copy);
	~CollisionBody(void);

	CollisionBody &operator=(const CollisionBody &other);

	/* General getters and setters */
	void setType(BodyType type) { this->type = type; }
	BodyType getType() { return type; }
	void setRadius(float radius) { this->radius = radius; }
	float getRadius() { return radius; }
	void setLength(float length) { this->length = length; }
	float getLength() { return length; }
	void setNormal(Vector3 &normal) { *(this->normal) = normal; }
	Vector3 *getNormal() { return normal; }
	void setParent(PhysicalBody *body) { parent = body; }
	PhysicalBody *getParent() { return parent; }
	void setPosition(Vector3 &position) { *(this->position) = position; }
	Vector3 *getPosition() { return position; }
	void setAbsolutePosition(Vector3 &absolutePosition) { *(this->absolutePosition) = absolutePosition; }
	Vector3 *getAbsolutePosition() { return absolutePosition; }

	/* Return the boundaries positions of this collision body.
	 * These are the absolute world positions, and not the positions
	 * relative to the parent. */
	Vector3 getMinPosition();
	Vector3 getMaxPosition();

	CollisionResult checkIfCollide(CollisionBody *body2, float deltaT);

protected:

	/* The type of this collision body. Defaults to sphere */
	BodyType type;
	/* The radius of the sphere or of the cylinder circunference. Defaults to 0 */
	float radius;
	/* The HALF length (from the center to one edge) of the cylinder, or the distance of the plane from origin. Defaults to 0 */
	float length;
	/* The normal of the plane, or the direction of the cylinder. Only used for planes, defaults to (0, 0, 0) */
	Vector3 *normal;
	/* The relative position of this collision body in relation to its parent */
	Vector3 *position;
	/* The absolute position of this collision body in relation to the world. */
	Vector3 *absolutePosition;
	/* The physical body that owns this collision body */
	PhysicalBody *parent;
};

/*
 * This struct represents the result of a collision, and is used by the physical body to respond to it.
 */
struct CollisionResult {
	CollisionBody *body1;
	CollisionBody *body2;
	float penetration;
	Vector3 normal;

	CollisionResult(CollisionBody *body1, CollisionBody *body2, float penetration, Vector3 normal) :
	body1(body1), body2(body2), penetration(penetration), normal(normal) {}

	bool isColliding() { return penetration > 0; }
};