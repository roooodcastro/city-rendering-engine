#include "CityBlock.h"

CityBlock::CityBlock(void) : Entity() {
    vertices = new std::vector<Intersection*>();
    model = Model::getOrCreate("cube", "resources/meshes/cube.obj", false);
    shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl",
        "resources/shaders/fragLight.glsl", false);
    this->scale = Vector3(50, 50, 50);
}

CityBlock::~CityBlock(void) {
    if (vertices != nullptr) {
        vertices->clear();
        delete vertices;
        vertices = nullptr;
    }
}

void CityBlock::update(float millisElapsed) {
    if (posChanged || rotChanged || scaleChanged) {
        calculateModelMatrix(Vector3(), Vector3(), Vector3(1, 1, 1), false, false, false);
    }
    // Update the distanceToCamera if that's changed, and if it is, also update the distance of the children.
    if (Naquadah::getInstance()->getCurrentScene()->getCamera()->hasChanged()) {
        Camera *camera = Naquadah::getInstance()->getCurrentScene()->getCamera();
        Vector3 dir = position - camera->getPosition();
        distanceToCamera = Vector3::dot(dir, dir);
        for (std::vector<Entity*>::iterator it = childEntities->begin(); it != childEntities->end(); it++) {
            (*it)->update(millisElapsed);
        }
    }
}

void CityBlock::addVertice(Intersection *intersection) {
    this->vertices->push_back(intersection);
    this->position = getCentralPosition();
    this->posChanged = true;
}

void CityBlock::generateBuildings() {
    if (childEntities != nullptr) {
        childEntities->clear();
        delete childEntities;
    }
    childEntities = new std::vector<Entity*>();
    // For now just generate a 4x4 grid of buildings
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            Vector3 position = Vector3(((float) i * 50) - 100, 0, ((float) j * 50) - 100);
            Building *building = new Building(this, position);
            addChild(building);
        }
    }
    numChildEntities = (int) childEntities->size();
    this->posChanged = true;
    this->rotChanged = true;
    this->scaleChanged = true;
}

Vector3 CityBlock::getCentralPosition() {
    int numVertices = (int) vertices->size();
    Vector3 centralPosition = Vector3();
    for (int i = 0; i < numVertices; i++) {
        centralPosition += vertices->at(i)->getPosition();
    }
    return centralPosition / (float) numVertices;
}