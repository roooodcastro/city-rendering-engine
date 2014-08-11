#include "Road.h"

Road::Road(void) : Entity() {
    setModel(Model::getOrCreate(MODEL_ROAD, "resources/meshes/plane.obj", false));
    model->setTexture(Texture::getOrCreate(TEXTURE_ROAD_1, "resources/textures/road_straight_4.png", false));
    shader = Shader::getOrCreate(SHADER_LIGHT_ROAD,
        "resources/shaders/vertRoad.glsl", "resources/shaders/fragRoad.glsl", false);
    pointA = nullptr;
    pointB = nullptr;
    this->numChunksSharing = 0;
}

Road::Road(Intersection *pointA, Intersection *pointB) : Entity() {
    setModel(Model::getOrCreate(MODEL_ROAD, "resources/meshes/plane.obj", false));
    model->setTexture(Texture::getOrCreate(TEXTURE_ROAD_1, "resources/textures/road_straight_4.png", false));
    shader = Shader::getOrCreate(SHADER_LIGHT_ROAD,
        "resources/shaders/vertRoad.glsl", "resources/shaders/fragRoad.glsl", false);
    this->pointA = pointA;
    this->pointB = pointB;
    this->numChunksSharing = 0;

    // Calculate the quad model rotation, position and scale
    this->position = Vector3::average(pointA->getPosition(), pointB->getPosition());
    this->rotation = pointB->getPosition() - pointA->getPosition();
    Vector2 horizDiff = Vector2(rotation.x, rotation.z);
    float angle = atan2f(horizDiff.x, horizDiff.y) * (180 / PI);
    float distance = (pointA->getPosition() - pointB->getPosition()).getLength() - 20.0f;
    this->rotation = Vector3(0, angle, 0);
    this->scale = Vector3(10, 0, distance / 2.0f);
    this->setRenderRadius(distance / 2.0f);
    shader->addShaderParameter("roadScale", PARAMETER_FLOAT, &distance);
}

Road::~Road(void) {
    if (pointA != nullptr) {
        pointA->disconnectFrom(this);
    }
    if (pointB != nullptr) {
        pointB->disconnectFrom(this);
    }
    pointA = nullptr;
    pointB = nullptr;
}

void Road::draw(float millisElapsed) {
    if (model != nullptr) {
        Naquadah::getInstance()->getCurrentScene()->useShader(shader);
        Naquadah::getRenderer()->updateShaderMatrix("modelMatrix", modelMatrix);
        float distance = scale.z * 2.0f;
        shader->getShaderParameter("roadScale")->setValue(&distance, false);
        shader->updateShaderParameters(false);
        model->draw();
    }
}

void Road::setPointA(Intersection *pointA) {
    this->pointA = pointA;
    if (pointA == nullptr) {
        // If we break a connection, tell the other Intersection to break it too
        pointB->disconnectFrom(this);
    }
}

void Road::setPointB(Intersection *pointB) {
    this->pointB = pointB;
    if (pointB == nullptr) {
        // If we break a connection, tell the other Intersection to break it too
        pointA->disconnectFrom(this);
    }
}