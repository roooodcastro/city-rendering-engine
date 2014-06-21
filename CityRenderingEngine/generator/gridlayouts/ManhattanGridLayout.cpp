#include "ManhattanGridLayout.h"

ManhattanGridLayout::ManhattanGridLayout(const Vector2 &posMin, const Vector2 &posMax)
    : GridLayout(MANHATTAN_GRID_LAYOUT_ID, posMin, posMax) {}

Vector2 ManhattanGridLayout::getIntersectionPosition() {
    Vector2 position = Vector2(-1, -1);
    int blockWidth = 200;
    int blockDepth = 100;
    Vector2 posAvg = (posMax + posMin) / 2;
    int offsetX = ((int) (posAvg.x)) % blockWidth;
    int offsetY = ((int) (posAvg.y)) % blockDepth;
    offsetX = abs(offsetX) > Chunk::SUBCHUNK_SIZE ? offsetX - blockWidth : offsetX;
    offsetY = abs(offsetY) > Chunk::SUBCHUNK_SIZE ? offsetY - blockDepth : offsetY;
    int maxOffsetAllowed = Chunk::SUBCHUNK_SIZE / 2;
    if (abs(offsetX) <= maxOffsetAllowed && abs(offsetY) <= maxOffsetAllowed) {
        position.x = posAvg.x - offsetX;
        position.y = posAvg.y - offsetY;
    }
    return position;
}

void ManhattanGridLayout::generateRoads(Chunk *chunk, Intersection *newIntersection) {
    std::vector<Intersection*> closestInter = chunk->getClosestIntersectionsTo(newIntersection, 10);
    if (closestInter.size() > 0) {
        for (int i = 0; i < min(2, closestInter.size()); i++) {
            Intersection *intersection = closestInter.at(i);
            Vector3 interPos = intersection->getWorldPosition();
            Vector3 newInterPos = newIntersection->getWorldPosition();
            if (interPos.x == newInterPos.x || interPos.z == newInterPos.z) {
                Road *road = intersection->connectTo(newIntersection);
                if (road != nullptr) chunk->addRoad(road);
            }
        }
    }
}