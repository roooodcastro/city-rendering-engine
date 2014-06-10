#include "City.h"

City::City(void) {
    intersections = new std::vector<Intersection*>();
    cityBlocks = new std::vector<CityBlock*>();
}

City::~City(void) {
    if (intersections != nullptr) {
        intersections->clear();
        delete intersections;
        intersections = nullptr;
    }
    if (cityBlocks != nullptr) {
        cityBlocks->clear();
        delete cityBlocks;
        cityBlocks = nullptr;
    }
}

Intersection *City::getIntersectionAt(Vector3 position) {
    for (int i = 0; i < intersections->size(); i++) {
        if (intersections->at(i)->getPosition() == position) {
            return intersections->at(i);
        }
    }
    return nullptr;
}

City *City::generateManhattanGrid(int width, int height) {
    City *manhattan = new City();
    Vector2 gridOffset = Vector2((float) (width * 110), (float) (height * 110));
    for (int i = 0; i <= width; i++) { // North-South roads (columns)
        for (int j = 0; j <= height; j++) { // East-West roads (rows)
            Vector3 position = Vector3((j * 220.0f) - gridOffset.x, 0, (i * 220.0f) - gridOffset.y);
            Intersection *intersection = new Intersection(Vector3(position));
            std::stringstream intersectionName;
            intersectionName << "I" << intersection->getPosition();
            Naquadah::getInstance()->getCurrentScene()->addEntity(intersection, intersectionName.str());
            manhattan->intersections->emplace_back(intersection);
            if (j > 0) {
                // Create road between j-1 and j
                Vector3 previousPosition = Vector3(((j - 1) * 220.0f) - gridOffset.x, 0, (i * 220.0f) - gridOffset.y);
                intersection->connectTo(manhattan->getIntersectionAt(previousPosition));
            }
            if (i > 0) {
                // Create road between i-1 and i
                Vector3 previousPosition = Vector3((j * 220.0f) - gridOffset.x, 0, ((i - 1) * 220.0f) - gridOffset.y);
                intersection->connectTo(manhattan->getIntersectionAt(previousPosition));
            }
            if (i > 0 && j > 0) {
                // Create a CityBlock around ABCD, with A=(i-1,j-1), B=(i-1,j), C=(i,j-1), D=(i,j)
                CityBlock *cityBlock = new CityBlock();
                cityBlock->addVertice(manhattan->getIntersectionAt(Vector3(((j - 1) * 220.0f) - gridOffset.x, 0, ((i - 1) * 220.0f) - gridOffset.y)));
                cityBlock->addVertice(manhattan->getIntersectionAt(Vector3((j * 220.0f) - gridOffset.x, 0, ((i - 1) * 220.0f) - gridOffset.y)));
                cityBlock->addVertice(manhattan->getIntersectionAt(Vector3(((j - 1) * 220.0f) - gridOffset.x, 0, (i * 220.0f) - gridOffset.y)));
                cityBlock->addVertice(intersection);
                cityBlock->generateBuildings();
                manhattan->cityBlocks->emplace_back(cityBlock);
                std::stringstream blockName;
                blockName << "CB" << cityBlock->getPosition();
                Naquadah::getInstance()->getCurrentScene()->addEntity(cityBlock, blockName.str());
            }
        }
    }
    return manhattan;
}