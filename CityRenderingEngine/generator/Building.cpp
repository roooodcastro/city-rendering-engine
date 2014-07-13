#include "Building.h"

Building::Building(void) : Entity() {
    shader = Shader::getOrCreate(SHADER_LIGHT_BASIC, "resources/shaders/vertNormal.glsl",
        "resources/shaders/fragLight.glsl", false);
    this->cityBlock = nullptr;
}

Building::Building(CityBlock *cityBlock, Vector3 blockPosition) : Entity() {
    setModel(Model::getOrCreate(MODEL_CUBE, "resources/meshes/cube.obj", false));
    model->addUser();
    shader = Shader::getOrCreate(SHADER_LIGHT_BASIC, "resources/shaders/vertNormal.glsl",
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
    this->lotArea = new std::vector<Vector2>();
}

Building::Building(std::vector<Vector2> lotArea, CityBlock *cityBlock) {
    shader = Shader::getOrCreate(SHADER_LIGHT_BASIC, "resources/shaders/vertNormal.glsl",
        "resources/shaders/fragLight.glsl", false);
    this->lotArea = new std::vector<Vector2>(lotArea);
    this->cityBlock = cityBlock;
    this->model = nullptr;
}

Building::~Building(void) {
    if (cityBlock != nullptr) {
        cityBlock = nullptr;
    }
    if (lotArea != nullptr) {
        lotArea->clear();
        delete lotArea;
        lotArea = nullptr;
    }
    if (model != nullptr) {
        ResourcesManager::releaseResource(model->getName());
        model = nullptr;
    }
}

void Building::draw(float millisElapsed) {
    if (model != nullptr) {
        Naquadah::getInstance()->getCurrentScene()->useShader(shader);
        Naquadah::getRenderer()->updateShaderMatrix("modelMatrix", modelMatrix);
        model->draw();
    }
}

void Building::constructGeometry() {
    if (model != nullptr) {
        // TODO: Unload and delete old model
    }
    if (lotArea != nullptr && lotArea->size() > 2) {
        // Triangulate the footprint of the Building.
        std::vector<Vector2> baseTriangles;
        bool triangulated = Triangulation::triangulate(*lotArea, baseTriangles);
        if (triangulated) {

            // Calculate world center position to offset later
            auto itEnd = lotArea->end();
            int numSidesLot = (int) lotArea->size();
            Vector2 centrePos;
            for (auto it = lotArea->begin(); it != itEnd; it++) {
                centrePos += (*it);
            }
            centrePos /= (float) numSidesLot;
            Vector3 cityBlockPos = cityBlock->getPosition();

            // Set the Building position within its CityBlock
            this->position = Vector3(centrePos.x - cityBlockPos.x, 0, centrePos.y - cityBlockPos.z);
            this->rotChanged = true; // Forces the ModelMatrix to be updated
            cityBlock->setPosition(cityBlockPos); // Forces the ModelMatrix to be updated

            float baseHeight = cityBlockPos.y;
            float height = generateRandom(20, 100);

            std::vector<Vector3> vertices = std::vector<Vector3>();
            std::vector<Vector2> uv_maps = std::vector<Vector2>();

            // Transform the base faces into the roof faces, applying the correct height.
            itEnd = baseTriangles.end();
            for (auto it = baseTriangles.begin(); it != itEnd; it++) {
                // The lotArea is in world position, we need to transform it to model coordinates.
                // A model should have (0, 0, 0) at its centre, so we subtract centrePos from it.
                // The Building will actually have (0, height / 2, 0) as its centre.
                vertices.push_back(Vector3((*it).x - centrePos.x, height, (*it).y - centrePos.y));
                // TODO: Calculate uv_map here
                uv_maps.push_back(Vector2());
            }
            // For each side of the roof polygon (lotArea), create a quad that will be the walls of the Building.
            for (int i = 1; i < numSidesLot + 1; i++) {
                Vector2 a2 = lotArea->at(i - 1) - centrePos;
                Vector2 b2 = ((i == numSidesLot) ? lotArea->at(0) : lotArea->at(i)) - centrePos;
                Vector3 a = Vector3(a2.x, height, a2.y); // A ---- B
                Vector3 b = Vector3(b2.x, height, b2.y); // | \    |
                Vector3 c = Vector3(a2.x, 0, a2.y);      // |   \  |
                Vector3 d = Vector3(b2.x, 0, b2.y);      // C ---- D
                // Add the 2 triangles that form up the quad for each wall
                vertices.push_back(a); // Triangle 1: ABD
                vertices.push_back(b);
                vertices.push_back(d);
                vertices.push_back(a); // Triangle 2: ADC
                vertices.push_back(d);
                vertices.push_back(c);
                uv_maps.push_back(Vector2(0.0f, 0.0f));
                uv_maps.push_back(Vector2(1.0f, 0.0f));
                uv_maps.push_back(Vector2(1.0f, 1.0f));
                uv_maps.push_back(Vector2(0.0f, 0.0f));
                uv_maps.push_back(Vector2(1.0f, 1.0f));
                uv_maps.push_back(Vector2(0.0f, 1.0f));
            }
            // Create the Model using the vertices
            std::string modelName = getEntityName();
            modelName.replace(0, 8, "MODEL");
            std::stringstream texFileName;
            int texIndex = (int) generateRandom(1, 6);
            texFileName << "resources/textures/buildings/office_" << texIndex << ".jpg";
            Texture *texture = Texture::getOrCreate(texIndex + 1010, texFileName.str(), false);

            Model::getOrCreate(ResourcesManager::generateNextName(), vertices, uv_maps, Colour::WHITE, texture, false);

            setModel(Model::getOrCreate(ResourcesManager::generateNextName(), vertices, uv_maps, Colour::WHITE,
                texture, false));
        }
    }
}