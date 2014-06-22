#include "Intersection.h"

Intersection::Intersection(void) {
    model = Model::getOrCreate("Intersection", "resources/meshes/plane.obj", false);
    shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl",
        "resources/shaders/fragLight.glsl", false);
    // Calculate the plane's model rotation, position and scale
    this->position = Vector3();
    this->rotation = Vector3();
    this->scale = Vector3(10, 0, 10);
    this->setRenderRadius(15);
    this->numChunksSharing = 0;
    connections = new std::vector<Intersection*>();
    roads = new std::vector<Road*>();
}

Intersection::Intersection(Vector3 position) {
    model = Model::getOrCreate("Intersection", "resources/meshes/plane.obj", false);
    model->setTexture(Texture::getOrCreate("RoadIntersection", "resources/textures/road_intersection.png", false));
    shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl",
        "resources/shaders/fragLight.glsl", false);
    // Calculate the plane's model rotation, position and scale
    this->position = position;
    this->rotation = Vector3();
    this->scale = Vector3(10, 0, 10);
    this->setRenderRadius(15);
    this->numChunksSharing = 0;
    connections = new std::vector<Intersection*>();
    roads = new std::vector<Road*>();
}

Intersection::~Intersection(void) {
    if (connections != nullptr) {
        connections->clear();
        delete connections;
        connections = nullptr;
    }
    if (roads != nullptr) {
        roads->clear();
        delete roads;
        roads = nullptr;
    }
}

Road *Intersection::connectTo(Intersection *other) {
    for (auto it = connections->begin(); it != connections->end(); it++) {
        if ((*it) == other) return nullptr; // Avoid duplicate connections
    }
    this->connections->push_back(other);
    other->connections->push_back(this);
    Road *road = new Road(this, other);
    this->roads->push_back(road);
    other->roads->push_back(road);
    return road;
}