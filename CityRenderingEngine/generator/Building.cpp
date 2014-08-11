#include "Building.h"

Building::Building(void) : Entity() {
    shader = Shader::getOrCreate(SHADER_LIGHT_BASIC, "resources/shaders/vertNormal.glsl",
        "resources/shaders/fragLight.glsl", false);
    shader->addUser();
    this->cityBlock = nullptr;
}

Building::Building(CityBlock *cityBlock, Vector3 blockPosition) : Entity() {
    setModel(Model::getOrCreate(MODEL_CUBE, "resources/meshes/cube.obj", false));
    shader = Shader::getOrCreate(SHADER_LIGHT_BASIC, "resources/shaders/vertNormal.glsl",
        "resources/shaders/fragLight.glsl", false);
    shader->addUser();
    this->cityBlock = cityBlock;
    int numFloors = (int) generateRandom(2, 10);
    float height = numFloors * 3.2f;
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

Building::Building(std::vector<Vector2> lotArea, CityBlock *cityBlock, bool connects, const Vector2 &roadNormal) {
    shader = Shader::getOrCreate(SHADER_LIGHT_BASIC, "resources/shaders/vertNormal.glsl",
        "resources/shaders/fragLight.glsl", false);
    shader->addUser();
    this->lotArea = new std::vector<Vector2>(lotArea);
    this->cityBlock = cityBlock;
    this->model = nullptr;
    Vector2 centrePos;
    auto itEnd = this->lotArea->end();
    int numSidesLot = (int) this->lotArea->size();
    for (auto it = this->lotArea->begin(); it != itEnd; it++) {
        centrePos += (*it);
    }
    centrePos /= (float) numSidesLot;
    this->connectsToRoad = connects;
    this->roadConnection = roadNormal;

    // Set the Building position within its CityBlock
    Vector3 cityBlockPos = cityBlock->getPosition();
    this->position = Vector3(centrePos.x - cityBlockPos.x, 0, centrePos.y - cityBlockPos.z);
    this->rotChanged = true; // Forces the ModelMatrix to be updated

    /*******************************************************************************************
     * Use density and CityBlock position to choose the height and texture of this Building
     *******************************************************************************************/
    
    float baseHeight = cityBlock->getPosition().y;
    float density = cityBlock->getDensity();
    int posSum = (int) abs(centrePos.x + centrePos.y); // Used to choose stuff
    // TODO: Switch generateRandom with the same MOD calculations as in the CityBlock
    switch (cityBlock->getType()) {
    case CITY_BLOCK_RESIDENTIAL_LOW: {
        numFloors = (int) (1 + ((posSum % 8) * density));
        textureName = "house";
    }
    break;
    case CITY_BLOCK_RESIDENTIAL_MEDIUM: {
        numFloors = (int) ((10 + posSum % 32) * density);
        if (numFloors <= 3)
            textureName = "house";
        else
            textureName = "residential_condo";
    }
    break;
    case CITY_BLOCK_RESIDENTIAL_HIGH: {
        numFloors = (int) ((40 + posSum % 24) * density);
        if (numFloors <= 20)
            textureName = "residential_condo";
        else
            textureName = "residential_building";
    }
    break;
    case CITY_BLOCK_COMMERCIAL_LOW: {
        numFloors = (int) (1 + ((posSum % 16) * density));
        int mod5 = posSum % 5;
        // Choose from a number of different shops
        switch (mod5) {
        case 0:
            textureName = "barber_shop";
            break;
        case 1:
            textureName = "tosco_express";
            break;
        case 2:
            textureName = "sainsbobs_local";
            break;
        case 3:
            textureName = "borisclays";
            break;
        case 4:
            textureName = "wetherforks";
            break;
        }
    }
    break;
    case CITY_BLOCK_COMMERCIAL_MEDIUM: {
        numFloors = (int) ((15 + posSum % 24) * density);
        // Choose from a number of different shops and buildings
        if (numFloors == 3) {
            textureName = "sainsbobs_super";
        }
        int mod3 = posSum % 3;
        switch (mod3) {
        case 0:
            textureName = "office_small";
            break;
        case 1:
            textureName = "art_gallery";
            break;
        case 2:
            textureName = "office_medium";
            break;
        }
    }
    break;
    case CITY_BLOCK_COMMERCIAL_HIGH: {
        numFloors = (int) ((40 + posSum % 64) * density);
        int mod5 = posSum % 5;
        // Choose from a number of different office building designs
        switch (mod5) {
        case 0:
            textureName = "office_big_1";
            break;
        case 1:
            textureName = "office_big_2";
            break;
        case 2:
            textureName = "office_big_3";
            break;
        case 3:
            textureName = "office_big_4";
            break;
        case 4:
            textureName = "office_big_5";
            break;
        }
    }
    break;
    case CITY_BLOCK_PARK: {
        numFloors = 0;
        textureName = "grass";
        break;
    }
    default:
        break;
    }
    height = numFloors * 3.2f; // Assuming each floor has 3.2m of height
    shader->addShaderParameter("numFloors", PARAMETER_INT, &numFloors);
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
    if (shader != nullptr) {
        //ResourcesManager::releaseResource(shader->getName());
        shader = nullptr;
    }
}

void Building::draw(float millisElapsed) {
    if (model != nullptr) {
        Naquadah::getInstance()->getCurrentScene()->useShader(shader);
        Naquadah::getRenderer()->updateShaderMatrix("modelMatrix", modelMatrix);
        shader->getShaderParameter("numFloors")->setValue(&numFloors, false);
        shader->updateShaderParameters(false);
        model->draw();
    }
}

void Building::constructGeometry() {
    if (cityBlock->getType() == CITY_BLOCK_RESIDENTIAL_LOW) {
        // Use small house models
        setModel(Model::getOrCreate(MODEL_SIMPLE_HOUSE, "resources/meshes/simple_house.obj", false));
        setScale(Vector3(7, 7, 7));
        float angle = 270 - roadConnection.getAngle();
        // We get the front side of the building
        Vector2 roadA = lotArea->at(0);
        Vector2 roadB = lotArea->at(1);
        Vector2 roadMiddle = (roadA + roadB) / 2.0f;
        Vector3 cityBlockPos = cityBlock->getPosition();
        roadMiddle += roadConnection.normalised() * 14.0f;
        roadMiddle.x -= cityBlockPos.x;
        roadMiddle.y -= cityBlockPos.z;
        setPosition(Vector3(roadMiddle.x, position.y, roadMiddle.y));
        setRotation(Vector3(0, angle, 0));
        numFloors = -1;
    } else {
        // Generate custom cubes to use as the building model
        if (lotArea != nullptr && lotArea->size() > 2) {
            // Triangulate the footprint of the Building.
            std::vector<Vector2> baseTriangles;
            bool triangulated = Triangulation::triangulate(*lotArea, baseTriangles);
            if (triangulated) {
                Vector3 cityBlockPos = cityBlock->getPosition();
                cityBlock->setPosition(cityBlockPos); // Forces the ModelMatrix to be updated

                Vector2 centrePos;
                auto itEnd = lotArea->end();
                int numSidesLot = (int) this->lotArea->size();
                for (auto it = this->lotArea->begin(); it != itEnd; it++) {
                    centrePos += (*it);
                }
                centrePos /= (float) numSidesLot;

                std::vector<Vector3> vertices = std::vector<Vector3>();
                std::vector<Vector2> uv_maps = std::vector<Vector2>();

                // Transform the base faces into the roof faces, applying the correct height.
                itEnd = baseTriangles.end();
                for (auto it = baseTriangles.begin(); it != itEnd; it++) {
                    // The lotArea is in world position, we need to transform it to model coordinates.
                    // A model should have (0, 0, 0) at its centre, so we subtract centrePos from it.
                    // The Building will actually have (0, height / 2, 0) as its centre.
                    vertices.push_back(Vector3((*it).x - centrePos.x, height, (*it).y - centrePos.y));
                }
                // TODO: Calculate uv_map here
                uv_maps.push_back(Vector2(-1.0f, -1.0f));
                uv_maps.push_back(Vector2(-1.0f, 0.0f));
                uv_maps.push_back(Vector2(0.0f, 0.0f));
                uv_maps.push_back(Vector2(0.0f, 0.0f));
                uv_maps.push_back(Vector2(0.0f, -1.0f));
                uv_maps.push_back(Vector2(-1.0f, -1.0f));
                // For each side of the roof polygon (lotArea), create a quad that will be the walls of the Building.
                for (int i = 1; i < numSidesLot + 1; i++) {
                    Vector2 a2 = lotArea->at(i - 1) - centrePos;
                    Vector2 b2 = ((i == numSidesLot) ? lotArea->at(0) : lotArea->at(i)) - centrePos;
                    Vector3 a = Vector3(a2.x, height, a2.y); // A ---- B
                    Vector3 b = Vector3(b2.x, height, b2.y); // | \    |
                    Vector3 c = Vector3(a2.x, 0, a2.y);      // |   \  |
                    Vector3 d = Vector3(b2.x, 0, b2.y);      // C ---- D
                    float width = (a - b).getLength();
                    float texScaleX = width / 10.0f;
                    // Add the 2 triangles that form up the quad for each wall
                    vertices.push_back(a); // Triangle 1: ABD
                    vertices.push_back(b);
                    vertices.push_back(d);
                    vertices.push_back(a); // Triangle 2: ADC
                    vertices.push_back(d);
                    vertices.push_back(c);
                    uv_maps.push_back(Vector2(0.0f, 0.0f));
                    uv_maps.push_back(Vector2(texScaleX, 0.0f));
                    uv_maps.push_back(Vector2(texScaleX, (float) numFloors));
                    uv_maps.push_back(Vector2(0.0f, 0.0f));
                    uv_maps.push_back(Vector2(texScaleX, (float) numFloors));
                    uv_maps.push_back(Vector2(0.0f, (float) numFloors));
                }
                // Create the Model using the vertices
                std::string modelName = getEntityName();
                modelName.replace(0, 8, "MODEL");
                std::stringstream texFileName;
                int texIndex = (int) generateRandom(1, 6);
                texFileName << "resources/textures/buildings/office_" << texIndex << ".png";
                Texture *texture = Texture::getOrCreate(texIndex + 1010, texFileName.str(), false);

                Model::getOrCreate(ResourcesManager::generateNextName(), vertices, uv_maps, Colour::WHITE, texture, false);

                setModel(Model::getOrCreate(ResourcesManager::generateNextName(), vertices, uv_maps, Colour::WHITE,
                    texture, false));
            }
        }
    }
}