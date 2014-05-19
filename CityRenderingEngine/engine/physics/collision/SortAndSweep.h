/*
 * Author: Rodrigo Castro Azevedo
 *
 * Description: This broadphase detector works based on the Sort and Sweep (or Sweep and Prune)
 * algorithm. But unlike the basic version of the algorithm, here we perform the sorting on all
 * three axis, keeping an already pre-sorted list of collision bodies that will be updated each
 * frame with only the changes, instead of rebuilding and reordering the whole thing. The sorting
 * is made using insertion sort.
 */

#pragma once

#include "BroadphaseDetector.h"

class SortAndSweep : public BroadphaseDetector {
public:

	SortAndSweep(void);
	~SortAndSweep(void);

	/*
	 * The main method of the detector. This will perform the broadphase partitioning
	 * of the bodies and perform narrowphase detections on the bodies that are near to
	 * each other.
	 */
	virtual void performDetection(std::vector<PhysicalBody*> *bodies, float millisElapsed);

protected:

	// TODO: read the article at http://www.codercorner.com/SAP.pdf and implement it. If it does
	// not give satisfatory results, try and implement multi-axis SAP
};