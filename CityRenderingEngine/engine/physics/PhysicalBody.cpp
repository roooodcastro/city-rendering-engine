#include "PhysicalBody.h"

PhysicalBody::PhysicalBody(void) {
	mass = 1.0f;
	position = new Vector3(0, 0, 0);
	lastPosition = new Vector3(0, 0, 0);
	rotation = new Vector3(0, 0, 0);
	scale = new Vector3(1, 1, 1);
	force = new Vector3(0, 0, 0);
	elasticity = 1.0f;
	dragFactor = 0.0f;
	collisionBodies = new std::vector<CollisionBody*>();
	moveable = true;
	collisionGroup = 0;
	entity = NULL;
	unprocessedCollisions = 0;
}

PhysicalBody::PhysicalBody(const PhysicalBody &copy) {
	mass = copy.mass;
	position = new Vector3(*(copy.position));
	lastPosition = new Vector3(*(copy.lastPosition));
	rotation = new Vector3(*(copy.rotation));
	scale = new Vector3(*(copy.scale));
	force = new Vector3(*(copy.force));
	elasticity = copy.elasticity;
	dragFactor = copy.dragFactor;
	collisionBodies = new std::vector<CollisionBody*>(*(copy.collisionBodies));
	moveable = copy.moveable;
	collisionGroup = copy.collisionGroup;
	entity = copy.entity;
	unprocessedCollisions = copy.unprocessedCollisions;
}

PhysicalBody::PhysicalBody(Entity *entity, float mass, Vector3 &position) {
	this->mass = mass;
	this->position = new Vector3(position);
	this->lastPosition = new Vector3(position);
	this->rotation = new Vector3(0, 0, 0);
	this->scale = new Vector3(1, 1, 1);
	this->force = new Vector3(0, 0, 0);
	this->elasticity = 1.0f;
	this->dragFactor = 0.0f;
	this->collisionBodies = new std::vector<CollisionBody*>();
	this->moveable = true;
	this->collisionGroup = 0;
	this->entity = entity;
	unprocessedCollisions = 0;
}

PhysicalBody::~PhysicalBody(void) {
	delete position;
	position = NULL;
	delete lastPosition;
	lastPosition = NULL;
	delete rotation;
	rotation = NULL;
	delete scale;
	scale = NULL;
	delete force;
	force = NULL;
	for (unsigned i = 0; i < collisionBodies->size(); i++) {
		delete (*collisionBodies)[i];
	}
	collisionBodies->clear();
	delete collisionBodies;
	collisionBodies = NULL;
	entity = NULL;
}

PhysicalBody &PhysicalBody::operator=(const PhysicalBody &other) {
	mass = other.mass;
	*position = *(other.position);
	*lastPosition = *(other.lastPosition);
	*rotation = *(other.rotation);
	*scale = *(other.scale);
	*force = *(other.force);
	elasticity = other.elasticity;
	dragFactor = other.dragFactor;
	*collisionBodies = *(other.collisionBodies);
	moveable = other.moveable;
	collisionGroup = other.collisionGroup;
	entity = other.entity;
	unprocessedCollisions = other.unprocessedCollisions;
	return *this;
}

void PhysicalBody::update(float millisElapsed) {
	integrateNextFrame(millisElapsed);
}

void PhysicalBody::integrateNextFrame(float millisElapsed) {
	float deltaT = (float) (millisElapsed / 1000.0f);
	setPosition(*position + (*position - *lastPosition) + (getTotalAcceleration(deltaT * 1000.0f) * (deltaT * deltaT)));
}

bool PhysicalBody::isAtRest(float millisElapsed) {
	float totalVel = getVelocity(millisElapsed).getLength();
	float totalAccel = getTotalAcceleration(millisElapsed).getLength();
	if ((totalVel <= 0.05f) && (totalAccel < 0.05f)) {
		setVelocity(Vector3(0, 0, 0), millisElapsed);
		return true;
	}
	return false;
}

void PhysicalBody::setVelocity(Vector3 &velocity, float millisElapsed) {
	*(this->lastPosition) = *(this->position) + Vector3(velocity.inverse() * millisElapsed / 1000.0f);
}

Vector3 PhysicalBody::getVelocity(float millisElapsed) {
	return Vector3((*position - *lastPosition) / (millisElapsed / 1000.0f));
}

void PhysicalBody::setAcceleration(Vector3 &acceleration) {
	*force = acceleration * mass; // F = ma
}

Vector3 PhysicalBody::getAcceleration() {
	return Vector3(*force / mass); // a = F/m
}

Vector3 PhysicalBody::getTotalAcceleration(float millisElapsed) {
	// Calculates gravity force
	Vector3 gravityForce = Vector3(0, Simulation::getInstance()->getGravity(), 0);
	if (gravityForce.getLength() < 0.01f) {
		gravityForce = Vector3(0, 0, 0);
	}
	// Calculates drag force
	Vector3 dragForce = getVelocity(millisElapsed) * mass * dragFactor;
	if (dragForce.getLength() < 0.01f) {
		dragForce = Vector3(0, 0, 0);
	}
	dragForce.invert(); // Invert it so it slows the body down instead of accelerating it
	// Calculates and returns total force acting on this body
	Vector3 outForce = Vector3(*force);
	if (outForce.getLength() < 0.01f) {
		outForce = Vector3(0, 0, 0);
	}
	return Vector3(outForce + gravityForce + dragForce);
}

void PhysicalBody::checkCollision(PhysicalBody *body1, PhysicalBody *body2, float deltaT) {
	for (unsigned i = 0; i < body1->collisionBodies->size(); i++) {
		CollisionBody *colBody1 = (*(body1->collisionBodies))[i];
		for (unsigned j = 0; j < body2->collisionBodies->size(); j++) {
			CollisionBody *colBody2 = (*(body2->collisionBodies))[j];
			PhysicalBody *swappedBody1 = NULL;
			PhysicalBody *swappedBody2 = NULL;
			if (colBody1->getType() != SPHERE) {
				// We always test collision between a sphere and something else
				// If after the swap, colBody1 still isn't a sphere, the collision
				// will just be ignored, as it's not currently supported
				CollisionBody *swap = colBody1;
				colBody1 = colBody2;
				colBody2 = swap;
				swappedBody1 = body2;
				swappedBody2 = body1;
			} else {
				swappedBody1 = body1;
				swappedBody2 = body2;
			}
			CollisionResult result = colBody1->checkIfCollide(colBody2, deltaT);
			if (result.isColliding()) {
				// Calculate some more collision parameters
				float elasticity = (swappedBody1->getElasticity() + swappedBody2->getElasticity()) / 2.0f;
				Vector3 body1Vel = swappedBody1->getVelocity(deltaT * 1000.0f);
				Vector3 body2Vel = swappedBody2->getVelocity(deltaT * 1000.0f);
				float normalDot = Vector3::dot(result.normal, result.normal);
				Vector3 combinedVel = (body1Vel - body2Vel);
				// Calculate the impulse and the resulting velocity
				float impulse = 0.0f;
				Vector3 vel1 = Vector3(0, 0, 0);
				Vector3 vel2 = Vector3(0, 0, 0);
				if (swappedBody2->canMove()) {
					impulse = Vector3::dot(combinedVel, (result.normal)) * (-1.0f * (1 + elasticity)) / (normalDot * ((1.0f / swappedBody1->mass) + (1.0f / swappedBody2->mass)));
					vel1 = body1Vel + (result.normal * (impulse / swappedBody1->mass));
					vel2 = body2Vel - (result.normal * (impulse / swappedBody2->mass));
					// Correct the inconsistency moving the bodies away from each other, then apply final velocity
					swappedBody1->setPosition((*(swappedBody1->position) + ((vel1 + (result.normal * (1.0f + (result.penetration / 2.0f)))) * (float) deltaT)));
					swappedBody2->setPosition((*(swappedBody2->position) + ((vel2 - (result.normal * (1.0f + (result.penetration / 2.0f)))) * (float) deltaT)));
					swappedBody1->setVelocity(vel1, deltaT * 1000.0f);
					swappedBody2->setVelocity(vel2, deltaT * 1000.0f);
				} else {
					if (swappedBody2->getCollisionGroup() == 1) {
						body1->unprocessedCollisions++;
					}
					impulse = Vector3::dot(body1Vel, result.normal) * (-1.0f * (1 + elasticity)) / (normalDot * (1.0f / swappedBody1->mass));
					vel1 = (result.normal * (impulse / swappedBody1->mass));
					// Correct the inconsistency moving the sphere away from the other static body
					swappedBody1->setPosition((*(swappedBody1->position) + ((vel1 + (result.normal * (1.0f - (result.penetration / 2.0f)))) * (float) deltaT)));
					swappedBody1->setVelocity(body1Vel + vel1, deltaT * 1000.0f);
				}
			}
		}
	}
}

void PhysicalBody::addCollisionBody(CollisionBody *colBody) {
	collisionBodies->emplace_back(colBody);
	Matrix4 rotMatrix = Matrix4::Rotation(rotation->x, Vector3(1, 0, 0)) * Matrix4::Rotation(rotation->y, Vector3(0, 1, 0)) * Matrix4::Rotation(rotation->z, Vector3(0, 0, 1));
	Matrix4 transform = Matrix4::Translation(getAbsolutePosition()) * rotMatrix;
	colBody->setAbsolutePosition((transform * Matrix4::Translation(*(colBody->getPosition())) * Vector3(0, 0, 0)));
}

void PhysicalBody::removeCollisionBody(CollisionBody *colBody) {
	collisionBodies->erase(std::remove(collisionBodies->begin(), collisionBodies->end(), colBody), collisionBodies->end());
}

Vector3 PhysicalBody::getAbsolutePosition() {
	Vector3 absPos = Vector3(*position);
	if (entity != NULL && entity->getParent() != NULL) {
		absPos += entity->getParent()->getPhysicalBody()->getAbsolutePosition();
	}
	return absPos;
}

Vector3 PhysicalBody::getAbsoluteScale() {
	Vector3 absScale = Vector3(*scale);
	if (entity != NULL && entity->getParent() != NULL) {
		absScale += entity->getParent()->getPhysicalBody()->getAbsoluteScale();
	}
	return absScale;
}

void PhysicalBody::setPosition(Vector3 &position) {
	*(this->lastPosition) = *(this->position);
	*(this->position) = position;
	if (collisionBodies->size() > 0) {
		// If we have collision bodies, calculate their absolute position
		// This is done here to speed up collision detection later
		Matrix4 rotMatrix = Matrix4::Rotation(rotation->x, Vector3(1, 0, 0)) * Matrix4::Rotation(rotation->y, Vector3(0, 1, 0)) * Matrix4::Rotation(rotation->z, Vector3(0, 0, 1));
		Matrix4 transform = Matrix4::Translation(getAbsolutePosition()) * rotMatrix;
		for (unsigned i = 0; i < collisionBodies->size(); i++) {
			CollisionBody *colBody = (*collisionBodies)[i];
			colBody->setAbsolutePosition((transform * Matrix4::Translation(*(colBody->getPosition())) * Vector3(0, 0, 0)));
		}
	}
}