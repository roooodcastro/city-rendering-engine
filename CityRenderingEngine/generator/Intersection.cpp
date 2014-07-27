#include "Intersection.h"
#include "Chunk.h"

Intersection::Intersection(void) : Entity() {
    setModel(Model::getOrCreate(MODEL_INTERSECTION, "resources/meshes/plane.obj", false));
    model->setTexture(Texture::getOrCreate(TEXTURE_ROAD_INTERSECTION_1, "resources/textures/road_intersection.png",
        false));
    shader = Shader::getOrCreate(SHADER_LIGHT_BASIC, "resources/shaders/vertNormal.glsl",
        "resources/shaders/fragLight.glsl", false);
    // Calculate the plane's model rotation, position and scale
    this->position = Vector3();
    this->rotation = Vector3();
    this->scale = Vector3(10, 0, 10);
    this->setRenderRadius(15);
    this->numChunksSharing = 0;
    //connections = new std::vector<Intersection*>();
    roads = new std::vector<Road*>();
}

Intersection::Intersection(Vector3 position) : Entity() {
    setModel(Model::getOrCreate(MODEL_INTERSECTION, "resources/meshes/plane.obj", false));
    model->setTexture(Texture::getOrCreate(TEXTURE_ROAD_INTERSECTION_1, "resources/textures/road_intersection.png",
        false));
    shader = Shader::getOrCreate(SHADER_LIGHT_BASIC, "resources/shaders/vertNormal.glsl",
        "resources/shaders/fragLight.glsl", false);
    // Calculate the plane's model rotation, position and scale
    this->position = position;
    this->rotation = Vector3();
    this->scale = Vector3(10, 0, 10);
    this->setRenderRadius(15);
    this->numChunksSharing = 0;
    //connections = new std::vector<Intersection*>();
    roads = new std::vector<Road*>();
}

Intersection::~Intersection(void) {
    /*if (connections != nullptr) {
        connections->clear();
        delete connections;
        connections = nullptr;
    }*/
    if (roads != nullptr) {
        roads->clear();
        delete roads;
        roads = nullptr;
    }
}

Road *Intersection::connectTo(Intersection *other) {
    for (auto it = roads->begin(); it != roads->end(); it++) {
        if ((*it)->getOtherEnd(this) == other)
            return nullptr; // Avoid duplicate connections
    }
    //this->connections->push_back(other);
    //other->connections->push_back(this);
    Road *road = new Road(this, other);
    this->roads->push_back(road);
    other->roads->push_back(road);
    return road;
}

void Intersection::disconnectFrom(Road *connection) {
    if (connection != nullptr) {
        if (connection->getPointA() == this || connection->getPointB() == this) {
            // If this Intersection actually is in the Road, remove the Road from the connections
            auto itEnd = roads->end();
            roads->erase(std::remove(roads->begin(), itEnd, connection), itEnd);
        }
    }
}

void Intersection::disconnectFromAll(Chunk *chunk) {
    if (roads != nullptr) {
        auto itEnd = roads->end();
        for (auto it = roads->begin(); it != itEnd; it++) {
            Intersection *other = (*it)->getOtherEnd(this);
            other->disconnectFrom(*it);
            chunk->removeRoad(*it);
            //delete (*it);
        }
        roads->clear();
    }
}

float Intersection::distanceBetween(Intersection *a, Intersection *b) {
    return (a->getPosition() - b->getPosition()).getLength();
}

Intersection *Intersection::getClosestIntersectionTo(Intersection *origin, std::vector<Intersection*> *list) {
    Intersection *closest = nullptr;
    float minDistance = (float) MAX_INT;
    auto itEnd = list->end();
    for (auto it = list->begin(); it != itEnd; it++) {
        float distance = distanceBetween(origin, (*it));
        if (distance < minDistance) {
            minDistance = distance;
            closest = (*it);
        }
    }
    return closest;
}