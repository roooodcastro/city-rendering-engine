#include "ManhattanGridLayout.h"

ManhattanGridLayout::ManhattanGridLayout(const Vector2 &posMin, const Vector2 &posMax)
    : GridLayout(MANHATTAN_GRID_LAYOUT_ID, posMin, posMax) {}

Vector2 ManhattanGridLayout::getIntersectionPosition() {
    Vector2 position = Vector2(-100, -100);
    int blockWidth = 200;
    int blockDepth = 100;
    Vector2 posAvg = (posMax + posMin) / 2;
    int offsetX = ((int) (posAvg.x)) % blockWidth;
    int offsetY = ((int) (posAvg.y)) % blockDepth;
    offsetX = abs(offsetX) > Chunk::SUBCHUNK_SIZE ? offsetX - blockWidth : offsetX;
    offsetY = abs(offsetY) > Chunk::SUBCHUNK_SIZE ? offsetY - blockDepth : offsetY;
    int maxOffsetAllowed = Chunk::SUBCHUNK_SIZE / 2;
    if (abs(offsetX) <= maxOffsetAllowed && abs(offsetY) <= maxOffsetAllowed) {
        position.x = posAvg.x - (offsetX * generateRandom(0, 4));
        position.y = posAvg.y - (offsetY * generateRandom(0, 4));
    }
    return position;
}

void ManhattanGridLayout::generateRoads(Chunk *chunk, Intersection *newIntersection) {
    std::vector<Intersection*> closestInter = chunk->getClosestIntersectionsTo(newIntersection, 4);
    if (closestInter.size() > 0) {
        float bestDistX = 999999.0f;
        float bestDistZ = 999999.0f;
        Intersection *bestConnectionX = nullptr;
        Intersection *bestConnectionZ = nullptr;
        Vector3 newInterPos = newIntersection->getWorldPosition();
        for (int i = 0; i < closestInter.size(); i++) {
            Intersection *intersection = closestInter.at(i);
            Vector3 interPos = intersection->getWorldPosition();
            Vector3 posDiff = interPos - newInterPos;
            float angle = abs((int) posDiff.toVec2(Vector3(0, 1, 0)).getAngle());
            if (abs(angle - 90) < 15 && (interPos.x <= newInterPos.x || interPos.z < newInterPos.z)) {
                float dist = posDiff.getLength();
                if (dist < bestDistZ) {
                    bestDistZ = dist;
                    bestConnectionZ = intersection;
                }
            }
            if (abs(angle - 180) < 10 && (interPos.x <= newInterPos.x || interPos.z < newInterPos.z)) {
                float dist = posDiff.getLength();
                if (dist < bestDistX) {
                    bestDistX = dist;
                    bestConnectionX = intersection;
                }
            }
        }
        if (bestConnectionZ != nullptr) {
            bool intersectsOtherIntersection = false;
            auto itEnd = chunk->getIntersections()->end();
            for (auto it = chunk->getIntersections()->begin(); it != itEnd; it++) {
                if ((*it) == bestConnectionZ || (*it) == newIntersection)
                    continue;
                Vector2 lineA = bestConnectionZ->getPosition().toVec2(Vector3(0, 1, 0));
                Vector2 lineB = newInterPos.toVec2(Vector3(0, 1, 0));
                Vector2 point = (*it)->getPosition().toVec2(Vector3(0, 1, 0));
                if (distancePointToLineSeg(lineA, lineB, point) < 20) {
                    // TODO: Change this hardcoded 20 to the width of the road
                    intersectsOtherIntersection = true;
                }
            }
            if (!intersectsOtherIntersection) {
                bool tooClose = false;
                Vector3 newRoadPos = Vector3::average(newInterPos, bestConnectionZ->getPosition());
                // Check if the road overlaps another road
                auto itEnd = chunk->getRoads()->end();
                for (auto it = chunk->getRoads()->begin(); it != itEnd; it++) {
                    if (((*it)->getPosition() - newRoadPos).getLength() < 20) {
                        tooClose = true;
                    }
                }
                if (!tooClose) {
                    Road *road = bestConnectionZ->connectTo(newIntersection);
                    if (road != nullptr) chunk->addRoad(road);
                }
            }
        }

        if (bestConnectionX != nullptr) {
           bool intersectsOtherIntersection = false;
            auto itEnd = chunk->getIntersections()->end();
            for (auto it = chunk->getIntersections()->begin(); it != itEnd; it++) {
                if ((*it) == bestConnectionX || (*it) == newIntersection)
                    continue;
                Vector2 lineA = bestConnectionX->getPosition().toVec2(Vector3(0, 1, 0));
                Vector2 lineB = newInterPos.toVec2(Vector3(0, 1, 0));
                Vector2 point = (*it)->getPosition().toVec2(Vector3(0, 1, 0));
                if (distancePointToLineSeg(lineA, lineB, point) < 20) {
                    // TODO: Change this hardcoded 20 to the width of the road
                    intersectsOtherIntersection = true;
                }
            }
            if (!intersectsOtherIntersection) {
                Road *substitute = nullptr;
                Vector3 newRoadPos = Vector3::average(newInterPos, bestConnectionX->getPosition());
                // Check if the road overlaps another road
                auto itEnd = chunk->getRoads()->end();
                for (auto it = chunk->getRoads()->begin(); it != itEnd; it++) {
                    if (((*it)->getPosition() - newRoadPos).getLength() < 20) {
                        substitute = *it;
                    }
                }
                if (substitute == nullptr) {
                    Road *road = bestConnectionX->connectTo(newIntersection);
                    if (road != nullptr) chunk->addRoad(road);
                } else {
                    // connect to existing road
                    //bestConnectionX->connectTo(newIntersection, substitute);
                    //chunk->addRoad(substitute);
                }
            }
        }
    }
}