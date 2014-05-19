#include "CollisionBody.h"

CollisionBody::CollisionBody(void) {
	type = SPHERE;
	radius = 0;
	length = 0;
	normal = new Vector3(0, 0, 0);
	parent = NULL;
	position = new Vector3(0, 0, 0);
	absolutePosition = new Vector3(0, 0, 0);
}

CollisionBody::CollisionBody(PhysicalBody *parent, BodyType type) {
	this->type = type;
	radius = 0;
	length = 0;
	normal = new Vector3(0, 0, 0);
	this->parent = parent;
	position = new Vector3(0, 0, 0);
	absolutePosition = new Vector3(0, 0, 0);
}

/* Creates a sphere */
CollisionBody::CollisionBody(PhysicalBody *parent, Vector3 &position, float radius) {
	this->type = SPHERE;
	this->radius = radius;
	length = 0;
	normal = new Vector3(0, 0, 0);
	this->parent = parent;
	this->position = new Vector3(position);
	absolutePosition = new Vector3(position);
}

/* Creates an inverted cylinder */
CollisionBody::CollisionBody(PhysicalBody *parent, Vector3 &position, float radius, float length, Vector3 &direction) {
	this->type = INV_CYLINDER;
	this->radius = radius;
	this->length = length;
	normal = new Vector3(direction);
	normal->normalise();
	this->parent = parent;
	this->position = new Vector3(position);
	absolutePosition = new Vector3(position);
}

/* Creates a plane */
CollisionBody::CollisionBody(PhysicalBody *parent, Vector3 &position, Vector3 &normal, float distance) {
	this->type = PLANE;
	this->radius = 0;
	this->length = distance;
	this->normal = new Vector3(normal);
	this->parent = parent;
	this->position = new Vector3(position);
	absolutePosition = new Vector3(position);
}

CollisionBody::~CollisionBody(void) {
	delete normal;
	normal = NULL;
	delete position;
	position = NULL;
	delete absolutePosition;
	absolutePosition = NULL;
	parent = NULL;
}

CollisionResult CollisionBody::checkIfCollide(CollisionBody *body2, float deltaT) {
	// First of all, don't check collision between collision bodies of the same physical body,
	// or of the same group
	if (parent == body2->parent || parent->getCollisionGroup() == body2->parent->getCollisionGroup()) {
		return CollisionResult(this, body2, 0, Vector3(0, 0, 0));
	}
	Vector3 body1Vel = this->parent->getVelocity(deltaT * 1000.0f);
	Vector3 body2Vel = body2->parent->getVelocity(deltaT * 1000.0f);
	Vector3 body1Pos = *getAbsolutePosition();
	Vector3 body2Pos = *(body2->getAbsolutePosition());
	// Sphere-Sphere collision
	if (type == SPHERE && body2->type == SPHERE) {
		float distance = pow(body2Pos.x - body1Pos.x, 2) + pow(body2Pos.y - body1Pos.y, 2) + pow(body2Pos.z - body1Pos.z, 2);
		if (distance < pow(this->radius + body2->radius, 2)) {
			// We have a collision!
			float penetration = sqrt(pow(this->radius + body2->radius, 2) - distance);
			Vector3 normal = Vector3(body1Pos - body2Pos).normalised();
			return CollisionResult(this, body2, penetration, normal);
		}
	}
	// Sphere-Plane collision
	if (type == SPHERE && body2->type == PLANE) {
		float distancePlaneToSphere = Vector3::dot(*(body2->normal), body1Pos) + body2->radius;
		if (distancePlaneToSphere < radius) {
			// We have a collision!
			float penetration = radius - distancePlaneToSphere;
			return CollisionResult(this, body2, penetration, Vector3(*(body2->normal)));
		}
	}
	// Sphere-InvertedCylinder collision
	if (type == SPHERE && body2->type == INV_CYLINDER) {
		float cylMin = ((body2Pos * *(body2->normal)) - (*(body2->normal) * body2->length)).getLength();
		float cylMax = ((body2Pos * *(body2->normal)) + (*(body2->normal) * body2->length)).getLength();
		float body1AxisPos = (body1Pos * *(body2->normal)).getLength();
		float cylMinX = body2Pos.x - body2->length;
		float cylMaxX = body2Pos.x + body2->length;
		/* This is the downside of this implementation: if the sphere is only slightly after
		* the end of the cylinder, but the lower half is still colliding with it, this
		* algorithm won't detect it.
		*/
		if (body1AxisPos >= cylMin && body1AxisPos <= cylMax) {
			Vector3 circunferenceDistance = (body2Pos - body1Pos) * (Vector3(1, 1, 1) - *(body2->normal));
			float distance = pow(circunferenceDistance.x, 2) + pow(circunferenceDistance.y, 2) + pow(circunferenceDistance.z, 2);
			if (distance > pow(body2->radius - this->radius, 2)) {
				// We have a collision!
				float penetration = sqrt(distance - pow(body2->radius - this->radius, 2));
				Vector3 normal = ((body1Pos - body2Pos) * (Vector3(1, 1, 1) - *(body2->normal))).normalised();
				return CollisionResult(this, body2, penetration, normal);
			}
		}
	}
	// If there's no collision, or we're trying to detect collision between planes and cylinders, return a no-collision result
	return CollisionResult(this, body2, 0, Vector3(0, 0, 0));
}

Vector3 CollisionBody::getMinPosition() {
	if (type == SPHERE) {
		return *getAbsolutePosition() - (Vector3(radius, radius, radius) * *(parent->getScale()));
	} else if (type == INV_CYLINDER) {
		Vector3 lenVec = *normal * length;
		Vector3 radVec = (Vector3(1, 1, 1) - *normal) * radius;
		return *getAbsolutePosition() - (lenVec + radVec);
	} else if (type == PLANE) {
		return Vector3(*getAbsolutePosition());
	}
}

Vector3 CollisionBody::getMaxPosition() {
	if (type == SPHERE) {
		return *getAbsolutePosition() + (Vector3(radius, radius, radius) * *(parent->getScale()));
	} else if (type == INV_CYLINDER) {
		Vector3 lenVec = *normal * length;
		Vector3 radVec = (Vector3(1, 1, 1) - *normal) * radius;
		return *getAbsolutePosition() + (lenVec + radVec);
	} else if (type == PLANE) {
		return *getAbsolutePosition();
	}
}