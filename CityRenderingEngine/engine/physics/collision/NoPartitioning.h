/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: As the name suggests, this is a broadphase collision detector that does
 * not partition the entities in any way, it just calculates all possible collisions.
 * 
 */

#pragma once

#include "BroadphaseDetector.h"

class NoPartitioning : public BroadphaseDetector {
public:

	NoPartitioning(void);
	~NoPartitioning(void);

	/*
	 * The main method of the detector. This will perform no broadphase partitioning
	 * of the bodies at all and perform narrowphase detections on all possible bodies.
	 */
	virtual void performDetection(std::vector<PhysicalBody*> *bodies, float millisElapsed);
};

