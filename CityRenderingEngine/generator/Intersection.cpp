#include "Intersection.h"

Intersection::Intersection(void) {
    model = Model::getOrCreate("Intersection", "resources/meshes/plane.obj");
    shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl", "resources/shaders/fragLight.glsl");
    // Calculate the plane's model rotation, position and scale
    this->position = Vector3();
    this->rotation = Vector3();
    this->scale = Vector3(10, 0, 10);
    this->setRenderRadius(15);
    connections = new std::vector<Intersection*>();
    roads = new std::vector<Road*>();
}

Intersection::Intersection(Vector3 position) {
    model = Model::getOrCreate("Intersection", "resources/meshes/plane.obj");
    model->setTexture(Texture::getOrCreate("RoadIntersection", "resources/textures/road_intersection.png"));
    shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl", "resources/shaders/fragLight.glsl");
    // Calculate the plane's model rotation, position and scale
    this->position = position;
    this->rotation = Vector3();
    this->scale = Vector3(10, 0, 10);
    this->setRenderRadius(15);
    connections = new std::vector<Intersection*>();
    roads = new std::vector<Road*>();
}

Intersection::~Intersection(void) {
    connections->clear();
    delete connections;
    connections = nullptr;
    roads->clear();
    delete roads;
    roads = nullptr;
}

void Intersection::connectTo(Intersection *other) {
    this->connections->emplace_back(other);
    other->connections->emplace_back(this);
    Road *road = new Road(this, other);
    std::stringstream roadName;
    roadName << this->position << other->position;
    Naquadah::getInstance()->getCurrentScene()->addEntity(road, roadName.str());
    this->roads->emplace_back(road);
    other->roads->emplace_back(road);
}