#include "GridLayout.h"

GridLayout::GridLayout(const int layoutId, const Vector2 &posMin, const Vector2 &posMax) : GRID_LAYOUT_ID(layoutId) {
    this->posMin = posMin;
    this->posMax = posMax;
}

CityBlock *GridLayout::generateCityBlock(Chunk *chunk, Intersection *start) {
    std::vector<Intersection*> vertices = std::vector<Intersection*>();
    Intersection *current = start;
    Intersection *last = nullptr;
    int numTries = 0; // A limiter to avoid infinite loop
    do {
        if (numTries > 10) return nullptr;
        std::vector<Road*> *connections = current->getConnections();
        Intersection *bestAlternative = nullptr;
        if (last == nullptr) {
            // If we don't have a reference to the last vertex added, add the one closest to the positive X-axis line.
            int angle = 0;
            int bestAngle = MAX_INT;
            for (auto it = connections->begin(); it != connections->end(); it++) {
                // Calculate angle between the connection and the current Intersection
                Intersection *intersection = (*it)->getOtherEnd(current);
                Vector3 posDiff = (intersection->getPosition() - current->getPosition());
                angle = abs((int) posDiff.toVec2(Vector3(0, 1, 0)).getAngle());
                if (angle < bestAngle) {
                    // If we have a better Intersection to choose, choose it.
                    // A better Intersection is the one that was the smallest angle
                    bestAngle = angle;
                    bestAlternative = intersection;
                }
            }
        } else {
            // If we do have a last vertex, add the one that's in the most clockwise position
            for (auto it = connections->begin(); it != connections->end(); it++) {
                Intersection *intersection = (*it)->getOtherEnd(current);
                Vector3 a = current->getPosition() - last->getPosition();
                Vector3 b = intersection->getPosition() - current->getPosition();
                a.y = a.z;
                b.y = b.z;
                a.z = 0;
                b.z = 0;
                Vector3 c = Vector3::cross(a, b);
                if (c.z < 0) {
                    bestAlternative = intersection;
                }
            }
        }
        
        if (bestAlternative != nullptr) {
            // Trace a triangle using the current, next and start points to check if it overlaps other Intersections.
            std::vector<Intersection*> overlaps = std::vector<Intersection*>();
            auto itAllEnd = chunk->getIntersections()->end();
            for (auto itAll = chunk->getIntersections()->begin(); itAll != itAllEnd; itAll++) {
                Vector2 a = start->getPosition().toVec2(Vector3(0, 1, 0));
                Vector2 b = current->getPosition().toVec2(Vector3(0, 1, 0));
                Vector2 c = bestAlternative->getPosition().toVec2(Vector3(0, 1, 0));
                Vector2 point = (*itAll)->getPosition().toVec2(Vector3(0, 1, 0));
                if (isPointInTriangle(point, a, b, c)) {
                    overlaps.push_back((*itAll));
                }
            }
            if (overlaps.size() > 0) {
                // If there's an Intersection that will be overlapped by the CityBlock, use it instead of the best.
                if (overlaps.size() == 1) {
                    bestAlternative = overlaps.at(0); // Just so we don't run a complex algorithm without need
                } else {
                    bestAlternative = Intersection::getClosestIntersectionTo(current, &overlaps);
                }
                Vector3 posDiff = (bestAlternative->getPosition() - current->getPosition());
            }
            vertices.push_back(bestAlternative);
            last = current;
            current = bestAlternative;
        } else return nullptr; // If we can't find the next vertex, it means there's no possible CityBlock to be found
        numTries++;
    } while (current != start);
    if (current == start) { // Checks if the CityBlock is continuous
        if (vertices.size() >= 3) { // A CityBlock must be at least a triangle
            CityBlock *cityBlock = new CityBlock();
            for (auto it = vertices.begin(); it != vertices.end(); it++) {
                cityBlock->addVertice((*it));
            }
            chunk->addCityBlock(cityBlock);
            return cityBlock;
        }
    }
    // If the CityBlock can't be created, just return null
    return nullptr;
}