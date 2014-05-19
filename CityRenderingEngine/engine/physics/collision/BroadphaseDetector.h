/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This is a templace class to enable Broadphase collision detectors.
 * Different classes should inherit from this templace, to allow different
 * collision detection methods, like world partitioning and sweep and prune.
 */

#pragma once

#include <vector>
#include "Simulation.h"

class Simulation;
class PhysicalBody;

class BroadphaseDetector {
public:

	BroadphaseDetector(void);
	virtual ~BroadphaseDetector(void);

	/*
	 * The main method of the detector. This will perform the broadphase partitioning
	 * of the bodies and perform narrowphase detections on the bodies that are near to
	 * each other.
	 */
	virtual void performDetection(std::vector<PhysicalBody*> *bodies, float millisElapsed) = 0;
};