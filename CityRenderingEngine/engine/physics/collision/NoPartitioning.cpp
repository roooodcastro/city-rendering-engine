#include "NoPartitioning.h"

NoPartitioning::NoPartitioning(void) {
}

NoPartitioning::~NoPartitioning(void) {
}

void NoPartitioning::performDetection(std::vector<PhysicalBody*> *bodies, float millisElapsed) {
	for (unsigned i = 0; i < bodies->size(); i++) {
		for (unsigned j = i + 1; j < bodies->size(); j++) {
			PhysicalBody *body1 = (*bodies)[i];
			PhysicalBody *body2 = (*bodies)[j];
			if (body1->getCollisionGroup() != body2->getCollisionGroup()) {
				PhysicalBody::checkCollision(body1, body2, millisElapsed / 1000.0f);
			}
		}
	}
}