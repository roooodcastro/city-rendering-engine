#include "Road.h"

Road::Road(void) : Entity() {
    model = Model::getOrCreate("Road", "resources/meshes/plane.obj");
    model->setTexture(Texture::getOrCreate("RoadStraight1", "resources/textures/road_straight_1.png"));
    shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl", "resources/shaders/fragLight.glsl");
    pointA = nullptr;
    pointB = nullptr;
}

Road::Road(Intersection *pointA, Intersection *pointB) : Entity() {
    model = Model::getOrCreate("Road", "resources/meshes/plane.obj");
    model->setTexture(Texture::getOrCreate("RoadStraight1", "resources/textures/road_straight_1.png"));
    shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl", "resources/shaders/fragLight.glsl");
    this->pointA = pointA;
    this->pointB = pointB;

    // Calculate the quad model rotation, position and scale
    this->position = Vector3::average(pointA->getPosition(), pointB->getPosition());
    this->rotation = pointB->getPosition() - pointA->getPosition();
    Vector2 horizDiff = Vector2(rotation.x, rotation.z);
    float angle = atan2f(horizDiff.x, horizDiff.y) * (180 / PI);
    float distance = (pointA->getPosition() - pointB->getPosition()).getLength() - 20.0f;
    this->rotation = Vector3(0, angle, 0);
    this->scale = Vector3(10, 0, distance / 2.0f);
    this->setRenderRadius(distance / 2.0f);
}

Road::~Road(void) {
    if (pointA != nullptr)
        delete pointA;
    pointA = nullptr;
    if (pointB != nullptr)
        delete pointB;
    pointB = nullptr;
}