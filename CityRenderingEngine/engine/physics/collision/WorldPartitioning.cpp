#include "WorldPartitioning.h"

WorldPartitioning::WorldPartitioning(void) : BroadphaseDetector() {
	// Initializes the world with the default size of 4 partitions in each dimension
	this->minPos = new Vector3(0, 0, 0);
	this->maxPos = new Vector3(1, 1, 1);
	this->numPartitionsX = 4;
	this->numPartitionsY = 4;
	this->numPartitionsZ = 4;
	initializeArray();
}

WorldPartitioning::WorldPartitioning(int numPartitionsX, int numPartitionsY, int numPartitionsZ, Vector3 &minPos, Vector3 &maxPos) : BroadphaseDetector() {
	this->minPos = new Vector3(minPos);
	this->maxPos = new Vector3(maxPos);
	this->numPartitionsX = numPartitionsX;
	this->numPartitionsY = numPartitionsY;
	this->numPartitionsZ = numPartitionsZ;
	initializeArray();
}

//WorldPartitioning::WorldPartitioning(const WorldPartitioning &copy) {
//	minPos = new Vector3(*(copy.minPos));
//	maxPos = new Vector3(*(copy.maxPos));
//	numPartitionsX = copy.numPartitionsX;
//	numPartitionsY = copy.numPartitionsY;
//	numPartitionsZ = copy.numPartitionsZ;
//	initializeArray();
//	for (int i = 0; i < numPartitionsX; i++) {
//		for (int j = 0; j < numPartitionsY; j++) {
//			for (int k = 0; k < numPartitionsZ; k++) {
//				delete partitionedBodies[i][j][k];
//				partitionedBodies[i][j][k] = new std::vector<PhysicalBody*>(*(copy.partitionedBodies[i][j][k]));
//			}
//		}
//	}
//}

WorldPartitioning::~WorldPartitioning(void) {
	delete minPos;
	delete maxPos;
	
	for (int i = 0; i < numPartitionsX; i++) {
		for (int j = 0; j < numPartitionsY; j++) {
			for (int k = 0; k < numPartitionsZ; k++) {
				delete partitionedBodies[i][j][k];
			}
			delete[] partitionedBodies[i][j];
		}
		delete[] partitionedBodies[i];
	}
	delete[] partitionedBodies;
}

WorldPartitioning &WorldPartitioning::operator=(const WorldPartitioning &other) {
	*minPos = *(other.minPos);
	*maxPos = *(other.maxPos);
	numPartitionsX = other.numPartitionsX;
	numPartitionsY = other.numPartitionsY;
	numPartitionsZ = other.numPartitionsZ;
	for (int i = 0; i < numPartitionsX; i++) {
		for (int j = 0; j < numPartitionsY; j++) {
			for (int k = 0; k < numPartitionsZ; k++) {
				delete partitionedBodies[i][j][k];
			}
			delete[] partitionedBodies[i][j];
		}
		delete[] partitionedBodies[i];
	}
	delete[] partitionedBodies;
	initializeArray();
	for (int i = 0; i < numPartitionsX; i++) {
		for (int j = 0; j < numPartitionsY; j++) {
			for (int k = 0; k < numPartitionsZ; k++) {
				*(partitionedBodies[i][j][k]) = *(other.partitionedBodies[i][j][k]);
			}
		}
	}
	return *this;
}

void WorldPartitioning::initializeArray() {
	partitionedBodies = new std::vector<PhysicalBody*>***[numPartitionsX];
	for (int i = 0; i < numPartitionsX; i++) {
		partitionedBodies[i] = new std::vector<PhysicalBody*>**[numPartitionsY];
		for (int j = 0; j < numPartitionsY; j++) {
			partitionedBodies[i][j] = new std::vector<PhysicalBody*>*[numPartitionsZ];
			for (int k = 0; k < numPartitionsZ; k++) {
				partitionedBodies[i][j][k] = new std::vector<PhysicalBody*>();
			}
		}
	}
}

void WorldPartitioning::addPhysicalBody(PhysicalBody *body) {
	bool hasPlane = false;
	Vector3 minBodyPos = Vector3(0, 0, 0);
	Vector3 maxBodyPos = Vector3(0, 0, 0);
	for (unsigned i = 0; i < body->getCollisionBodies()->size(); i++) {
		CollisionBody *colBody = (*(body->getCollisionBodies()))[i];
		Vector3 colBodyMin = colBody->getMinPosition();
		Vector3 colBodyMax = colBody->getMaxPosition();
		// Find the boundaries of all the combined collision bodies
		minBodyPos.x = min(minBodyPos.x, colBodyMin.x);
		minBodyPos.y = min(minBodyPos.y, colBodyMin.y);
		minBodyPos.z = min(minBodyPos.z, colBodyMin.z);
		maxBodyPos.x = max(maxBodyPos.x, colBodyMax.x);
		maxBodyPos.y = max(maxBodyPos.y, colBodyMax.y);
		maxBodyPos.z = max(maxBodyPos.z, colBodyMax.z);
		if ((*(body->getCollisionBodies()))[i]->getType() == PLANE) {
			hasPlane = true;
		}
	}
	// If the body has at least one plane, just add it to every partition, as it would be
	// over complicated to correctly calculate its partitions, as the plane is infinite
	if (hasPlane) {
		for (int i = 0; i < numPartitionsX; i++) {
			for (int j = 0; j < numPartitionsY; j++) {
				for (int k = 0; k < numPartitionsZ; k++) {
					partitionedBodies[i][j][k]->emplace_back(body);
				}
			}
		}
		return;
	}
	// If entity is outside the world bounds, disregard it
	if (minBodyPos.x < minPos->x || maxBodyPos.x > maxPos->x) {
		return;
	}
	if (minBodyPos.y < minPos->y || maxBodyPos.y > maxPos->y) {
		return;
	}
	if (minBodyPos.z < minPos->z || maxBodyPos.z > maxPos->z) {
		return;
	}
	// Calculates the partitions of the entity
	float cellSizeX = (maxPos->x - minPos->x) / numPartitionsX;
	float cellSizeY = (maxPos->y - minPos->y) / numPartitionsY;
	float cellSizeZ = (maxPos->z - minPos->z) / numPartitionsZ;

	int partitionMinX = (int) floor(minBodyPos.x / cellSizeX) + (numPartitionsX / 2);
	int partitionMaxX = (int) floor(maxBodyPos.x / cellSizeX) + (numPartitionsX / 2);
	int partitionMinY = (int) floor(minBodyPos.y / cellSizeY) + (numPartitionsY / 2);
	int partitionMaxY = (int) floor(maxBodyPos.y / cellSizeY) + (numPartitionsY / 2);
	int partitionMinZ = (int) floor(minBodyPos.z / cellSizeZ) + (numPartitionsZ / 2);
	int partitionMaxZ = (int) floor(maxBodyPos.z / cellSizeZ) + (numPartitionsZ / 2);

	// Then put the entity in the cells
	for (int i = partitionMinX; i <= partitionMaxX; i++) {
		for (int j = partitionMinY; j <= partitionMaxY; j++) {
			for (int k = partitionMinZ; k <= partitionMaxZ; k++) {
				partitionedBodies[i][j][k]->emplace_back(body);
			}
		}
	}
}

void WorldPartitioning::performDetection(std::vector<PhysicalBody*> *bodies, float millisElapsed) {
	// First recreate the partitions
	for (int i = 0; i < numPartitionsX; i++) {
		for (int j = 0; j < numPartitionsY; j++) {
			for (int k = 0; k < numPartitionsZ; k++) {
				delete partitionedBodies[i][j][k];
				partitionedBodies[i][j][k] = new std::vector<PhysicalBody*>();
			}
		}
	}
	for (unsigned i = 0; i < bodies->size(); i++) {
		addPhysicalBody((*bodies)[i]);
	}

	// Then process the collisions on each partition
	for (int i = 0; i < numPartitionsX; i++) {
		for (int j = 0; j < numPartitionsY; j++) {
			for (int k = 0; k < numPartitionsZ; k++) {
				std::vector<PhysicalBody*> *partBodies = partitionedBodies[i][j][k];
				for (unsigned l = 0; l < partBodies->size(); l++) {
					for (unsigned m = l + 1; m < partBodies->size(); m++) {
						PhysicalBody *body1 = (*partBodies)[l];
						PhysicalBody *body2 = (*partBodies)[m];
						if (body1->getCollisionGroup() != body2->getCollisionGroup()) {
							PhysicalBody::checkCollision(body1, body2, millisElapsed / 1000.0f);
						}
					}
				}
			}
		}
	}
}