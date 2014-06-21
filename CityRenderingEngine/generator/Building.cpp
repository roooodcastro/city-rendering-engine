#include "Building.h"

Building::Building(void) : Entity() {
    model = Model::getOrCreate("cube", "resources/meshes/cube.obj", false);
    shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl",
        "resources/shaders/fragLight.glsl", false);
    this->cityBlock = nullptr;
}

Building::Building(CityBlock *cityBlock, Vector3 blockPosition) : Entity() {
    model = Model::getOrCreate("cube", "resources/meshes/cube.obj", false);
    shader = Shader::getOrCreate("LightShader", "resources/shaders/vertNormal.glsl",
        "resources/shaders/fragLight.glsl", false);
    this->cityBlock = cityBlock;
    float height = generateRandom(20, 100);
    float width = 50.0f; // Width relative to the pavement
    float depth = 50.0f; // Depth relative to going inwards the city block, to the centre of it
    this->scale = Vector3(width / 2.0f, height / 2.0f, depth / 2.0f);
    this->position = blockPosition;
    this->position.x += width / 2.0f;
    this->position.y += height / 2.0f;
    this->position.z += depth / 2.0f;
    this->renderRadius = Vector3(width, height, depth).getLength();
}

Building::~Building(void) {
    if (cityBlock != nullptr) {
        delete cityBlock;
        cityBlock = nullptr;
    }
}
