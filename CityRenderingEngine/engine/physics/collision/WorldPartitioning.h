#pragma once

#include "BroadphaseDetector.h"

class WorldPartitioning : public BroadphaseDetector {
public:

	WorldPartitioning(void);
	WorldPartitioning(int numPartitionsX, int numPartitionsY, int numPartitionsZ, Vector3 &minPos, Vector3 &maxPos);
	virtual ~WorldPartitioning(void);

	WorldPartitioning &operator=(const WorldPartitioning &other);

	/* Adds a new body to the world, calculating its partition. If the body is already in the world, the function updates its partitions */
	void addPhysicalBody(PhysicalBody *body);
	/* Removes a body from the world */
	void removePhysicalBody(PhysicalBody *body);

	/* Initializes that 3D array of vectors */
	void initializeArray();

	/*
	 * The main method of the detector. This will perform the broadphase partitioning
	 * of the bodies and perform narrowphase detections on the bodies that are near to
	 * each other.
	 */
	virtual void performDetection(std::vector<PhysicalBody*> *bodies, float millisElapsed);

protected:
	
	int numPartitionsX;
	int numPartitionsY;
	int numPartitionsZ;
	Vector3 *minPos;
	Vector3 *maxPos;

	// God only knows where this pointer goes!
	std::vector<PhysicalBody*> ****partitionedBodies;
};