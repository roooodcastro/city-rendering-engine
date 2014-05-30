#include "CityBlock.h"

CityBlock::CityBlock(void) {
    vertices = new std::vector<Intersection*>();
    buildings = new std::vector<Building*>();
}

CityBlock::~CityBlock(void) {
    if (vertices != nullptr) {
        vertices->clear();
        delete vertices;
        vertices = nullptr;
    }
    if (buildings != nullptr) {
        buildings->clear();
        delete buildings;
        buildings = nullptr;
    }
}

void CityBlock::addVertice(Intersection *intersection) {
    this->vertices->emplace_back(intersection);
}

void CityBlock::generateBuildings() {
    if (buildings != nullptr) {
        buildings->clear();
        delete buildings;
    }
    buildings = new std::vector<Building*>();
    // For now just generate a 4x4 grid of buildings
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            Vector3 position = Vector3(((float) i * 50) - 100, 0, ((float) j * 50) - 100);
            Building *building = new Building(this, position);
            buildings->emplace_back(building);
        }
    }
}

Vector3 CityBlock::getCentralPosition() {
    int numVertices = (int) vertices->size();
    Vector3 centralPosition = Vector3();
    for (int i = 0; i < numVertices; i++) {
        centralPosition += vertices->at(i)->position;
    }
    return centralPosition / (float) numVertices;
}