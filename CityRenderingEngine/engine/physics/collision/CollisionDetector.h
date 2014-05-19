#pragma once

#include "BoundingBox.h"

class CollisionDetector {
public:

	~CollisionDetector(void);

	static bool isColliding(const BoundingBox &box1, const BoundingBox &box2);


protected:
	
	CollisionDetector(void);

};