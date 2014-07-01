#include "Chunk.h"

Chunk::Chunk(void) : Entity(), Resource() {
    this->intersections = new std::vector<Intersection*>();
    this->roads = new std::vector<Road*>();
    this->cityBlocks = new std::vector<CityBlock*>();
    this->setRenderRadius((Chunk::CHUNK_SIZE * 1.42f) / 2.0f);
    this->city = nullptr;
}

Chunk::Chunk(const Vector2 &position, City *city) : Entity(), Resource() {
    this->position = Vector3(position.x + 500.0f, 0, position.y + 500.0f);
    this->intersections = new std::vector<Intersection*>();
    this->roads = new std::vector<Road*>();
    this->cityBlocks = new std::vector<CityBlock*>();
    this->setRenderRadius((Chunk::CHUNK_SIZE * 1.42f) / 2.0f);
    this->city = city;
}

Chunk::~Chunk(void) {
    if (intersections != nullptr) {
        intersections->clear();
        delete intersections;
        intersections = nullptr;
    }
    if (roads != nullptr) {
        roads->clear();
        delete roads;
        roads = nullptr;
    }
    if (cityBlocks != nullptr) {
        cityBlocks->clear();
        delete cityBlocks;
        cityBlocks = nullptr;
    }
    city = nullptr;
}

void Chunk::calculateModelMatrix(Vector3 addPos, Vector3 addRot, Vector3 addSiz, bool pDiff, bool rDiff, bool sDiff) {
    // A Chunk won't be rendered itself, so it does not need a modelMatrix
    // We only need to update its children's modelMatrix
    if (numChildEntities > 0) {
        for(auto it = childEntities->begin(); it != childEntities->end(); it++) {
            // We also don't pass any derived transform values, as all children will have world coordinate transforms
            (*it)->calculateModelMatrix(Vector3(), Vector3(), Vector3(1, 1, 1), false, false, false);
        }
    }
}

void Chunk::update(float millisElapsed) {
    calculateModelMatrix(Vector3(), Vector3(), Vector3(1, 1, 1), false, false, false);
    // Update the distanceToCamera if that's changed
    if (Naquadah::getInstance()->getCurrentScene()->getCamera()->hasChanged()) {
        Camera *camera = Naquadah::getInstance()->getCurrentScene()->getCamera();
        Vector3 dir = position - camera->getPosition();
        distanceToCamera = Vector3::dot(dir, dir);
    }

    if (numChildEntities > 0) {
        for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); it++) {
            (*it)->update(millisElapsed);
        }
    }
}

void Chunk::draw(float millisElapsed) {
    if (numChildEntities > 0) {
        Frustum *frustum = Naquadah::getInstance()->getCurrentScene()->getFrustum();
        for (auto it = childEntities->begin(); it != childEntities->end(); ++it) {
            if (frustum->isEntityInside(*it)) {
                (*it)->draw(millisElapsed);
            }
        }
    }
}

void Chunk::addIntersection(Intersection *intersection) {
    intersections->push_back(intersection);
    intersection->addChunkSharing();
    addChild(intersection);
}

void Chunk::removeIntersection(Intersection *intersection) {
    auto itEnd = intersections->end();
    intersections->erase(std::remove(intersections->begin(), itEnd, intersection), itEnd);
    intersection->removeChunkSharing();
    removeChild(intersection);
}

void Chunk::addRoad(Road *road) {
    roads->push_back(road);
    addChild(road);
}

void Chunk::addCityBlock(CityBlock *cityBlock) {
    cityBlocks->push_back(cityBlock);
    addChild(cityBlock);
}

Intersection *Chunk::getClosestIntersectionTo(Intersection *intersection) {
    auto beginIt = intersections->begin();
    auto endIt = intersections->end();
    Intersection *closest = nullptr;
    float closestDistance = 999999.9f;
    for (auto it = beginIt; it != endIt; it++) {
        float distance = (intersection->getWorldPosition() - (*it)->getWorldPosition()).getLength();
        if (distance < closestDistance) {
            closest = (*it);
            closestDistance = distance;
        }
    }
    return closest;
}

std::vector<Intersection*> Chunk::getClosestIntersectionsTo(Intersection *intersection, int number) {
    std::vector<Intersection*> orderedIntersections = std::vector<Intersection*>();
    std::vector<float> distances = std::vector<float>();
    for (auto it = intersections->begin(); it != intersections->end(); it++) {
        float distance = abs((intersection->getWorldPosition() - (*it)->getWorldPosition()).getLength());
        bool inserted = false;
        int distantesSize = (int) distances.size();
        for (int i = 0; i < distantesSize; i++) {
            if (distance < distances.at(i) && distance != 0.0f) {
                orderedIntersections.insert(orderedIntersections.begin() + i, (*it));
                distances.insert(distances.begin() + i, distance);
                inserted = true;
                break;
            }
        }
        if (!inserted && distance != 0.0f) {
            orderedIntersections.push_back((*it));
            distances.push_back(distance);
        }
    }
    if (orderedIntersections.size() > number) {
        orderedIntersections.erase(orderedIntersections.begin() + number, orderedIntersections.end());
        orderedIntersections.shrink_to_fit();
    }
    return orderedIntersections;
}

void Chunk::load() {

}

void Chunk::unload() {
    for (auto it = roads->begin(); it != roads->end(); it++) {
        if ((*it)->getPointA()->getNumChunksSharing() == 1 && (*it)->getPointB()->getNumChunksSharing() == 1) {
            delete *it;
        }
    }
    roads->clear();
    for (auto it = intersections->begin(); it != intersections->end(); it++) {
        if ((*it)->getNumChunksSharing() == 1) {
            delete *it;
        } else {
            (*it)->removeChunkSharing();
        }
    }
    intersections->clear();
    // TODO: Find and fix a small memoryleak on the chunk loading/unloading, about 4kb each 20 or so chunks
}

void Chunk::saveToFile() {

}

std::string Chunk::getFileName() {
    return Chunk::getFileName(Vector2(position.x, position.z));
}

bool Chunk::chunkExists(const Vector2 &position) {
    return FileIO::fileExists(Chunk::getFileName(position));
}

Chunk *Chunk::loadChunk(const Vector2 &position, City *city) {
    Chunk *chunk = new Chunk(position, city);
    return chunk;
}