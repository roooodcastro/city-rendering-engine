#include "Building.h"

Building::Building(void) : Entity() {
    model = Model::getOrCreate("cube", "resources/meshes/cube.obj");
    shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl", "resources/shaders/fragLight.glsl");
    this->cityBlock = nullptr;
    this->blockPosition = Vector3();
}

Building::Building(CityBlock *cityBlock, Vector3 blockPosition) : Entity() {
    model = Model::getOrCreate("cube", "resources/meshes/cube.obj");
    shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl", "resources/shaders/fragLight.glsl");
    this->cityBlock = cityBlock;
    float height = generateRandom(10, 50);
    this->scale = Vector3(25, height, 25);
    this->position = cityBlock->getCentralPosition() + blockPosition;
    this->position.x += 25;
    this->position.y += height;
    this->position.z += 25;
    this->blockPosition = blockPosition;
}

Building::~Building(void) {
    if (cityBlock != nullptr)
        delete cityBlock;
    cityBlock = nullptr;
}
