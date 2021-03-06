#include "CityScene.h"

CityScene::CityScene() : Scene(new CitySceneInterface()) {
    this->city = nullptr;
    this->reloadShaders = false;
    this->reloadTextures = false;
}

CityScene::CityScene(const CityScene &copy) : Scene(copy) {
    this->city = new City(*(copy.city));
    this->reloadShaders = false;
    this->reloadTextures = false;
}

CityScene::CityScene(City *city) : Scene(new CitySceneInterface()) {
    this->city = city;
    this->skybox = new Skybox("resources/textures/skyboxes/desert/posX.png",
        "resources/textures/skyboxes/desert/negX.png",
        "resources/textures/skyboxes/desert/posY.png",
        "resources/textures/skyboxes/desert/negY.png",
        "resources/textures/skyboxes/desert/posZ.png",
        "resources/textures/skyboxes/desert/negZ.png",
        ResourcesManager::generateNextName());
    this->reloadShaders = false;
    this->reloadTextures = false;
}

CityScene::~CityScene(void) {
    if (city != nullptr) {
        delete city;
        city = nullptr;
    }
}

void CityScene::onKeyPress(SDL_Keysym key) {
    if (key.sym == SDLK_F3) {
        reloadTextures = true;
    }
    if (key.sym == SDLK_F4) {
        reloadShaders = true;
    }
}

void CityScene::update(float millisElapsed) {
    Scene::update(millisElapsed);
    lockUpdateMutex();

    // TODO: Move distance that new chunks should be loaded to config
    float chunkViewDistance = 4000.0f;

    // Use current camera position to calculate which Chunks must be loaded or unloaded.
    Vector3 cameraPos = camera->getPosition();
    Vector2 correctedCameraPos = Vector2(cameraPos.x, cameraPos.z);
    correctedCameraPos.x = (float) ((int) (cameraPos.x / (float) Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE);
    correctedCameraPos.y = (float) ((int) (cameraPos.z / (float) Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE);
    correctedCameraPos.x += Chunk::CHUNK_SIZE / 2.0f; // Make it to the centre of the Chunk, so we can calculate based
    correctedCameraPos.y += Chunk::CHUNK_SIZE / 2.0f; // on the centre of the chunk instead of from one of the corners.
    Vector2 chunkMin = Vector2(correctedCameraPos.x - chunkViewDistance, correctedCameraPos.y - chunkViewDistance);
    Vector2 chunkMax = Vector2(correctedCameraPos.x + chunkViewDistance, correctedCameraPos.y + chunkViewDistance);

    // Load Chunks that are inside the ChunkViewArea
    Vector2 cameraPos2f = Vector2(cameraPos.x, cameraPos.z);
    Vector2 chunkPosToBeLoaded = Vector2((float) MAX_INT, (float) MAX_INT);
    float minDistance = (float) MAX_INT;
    for (int x = (int) chunkMin.x; x < (int) chunkMax.x; x += Chunk::CHUNK_SIZE) {
        for (int y = (int) chunkMin.y; y < (int) chunkMax.y; y += Chunk::CHUNK_SIZE) {
            Vector2 chunkCentre = Vector2((float) x, (float) y);
            Vector2 chunkPos = Vector2((float) x - (Chunk::CHUNK_SIZE / 2.0f), (float) y - (Chunk::CHUNK_SIZE / 2.0f));
            float distance = (cameraPos2f - chunkCentre).getLength();
            if ((distance - Chunk::CHUNK_SIZE) < chunkViewDistance) {
                // Chunk should be loaded, so we load/generate it if it's not already loaded or being loaded
                if (!city->isChunkLoaded(chunkPos)) {
                    if (distance <= minDistance) {
                        // Prioritize the Chunks that are closer to the player
                        // TODO: Prioritize the Chunks that are close and in front of the player
                        chunkPosToBeLoaded = chunkPos;
                        minDistance = distance;
                    }
                }
            }
        }
    }
    if (abs(chunkPosToBeLoaded.x - (float) MAX_INT) > EPS)
        ChunkLoader::getInstance()->loadChunk(chunkPosToBeLoaded, city);

    // Unload Chunks that are outside the ChunkViewArea
    float toleranceMultiplier = 1.5f;
    /* This multiplier creates an unloading area bigger than the loading area, so the Chunks will load closer and
     * unload a little further away from the Player. This prevents loading/unloading chaos when the Player gets near
     * the loading limit.
     */
    Chunk *toBeUnloaded = nullptr;
    city->lockMutex();
    auto itBegin = city->getChunks()->begin();
    auto itEnd = city->getChunks()->end();
    for (auto it = itBegin; it != itEnd; it++) {
        Vector2 chunkCentre = (*it)->getCentrePos();
        float distance = (cameraPos2f - chunkCentre).getLength();
        if (distance > (chunkViewDistance * toleranceMultiplier)) {
            toBeUnloaded = *it;
        }
    }
    if (toBeUnloaded != nullptr) {
        ChunkLoader::getInstance()->unloadChunk(toBeUnloaded, city);
    }
    city->unlockMutex();
    unlockUpdateMutex();
}

void CityScene::render(Renderer *renderer, float millisElapsed) {
    // Render the scene
    Scene::render(renderer, millisElapsed);

    // Check if there's a Chunk waiting for the OpenGL stuff be unloaded
    //Profiler::getTimer(4)->startMeasurement();
    bool unloaded = false;
    ChunkOperation chunkOp = ChunkLoader::getInstance()->getFirstInTheQueue();
    if (chunkOp.city != nullptr && !chunkOp.load) {
        Chunk *chunk = chunkOp.city->getChunkAt(chunkOp.chunkPos, false);
        if (chunk != nullptr && !chunk->isSafeToDelete()) {
            chunk->unloadOpenGL();
            chunk->setSafeToDelete(true);
            unloaded = true;
        }
    }

    // Check debug tools

    // Shader reload (F4)
    if (reloadShaders) {
        ((Shader*) ResourcesManager::getResource(SHADER_LIGHT_BASIC))->reload();
        ((Shader*) ResourcesManager::getResource(SHADER_LIGHT_ROAD))->reload();
        ((Shader*) ResourcesManager::getResource(SHADER_SKY_BOX))->reload();
        std::cout << "Shaders reloaded!" << std::endl;
        reloadShaders = false;
    }

    // Textures reload (F3)
    if (reloadTextures) {
        for (int i = 1000; i < 1050; i++) {
            Texture *texture = (Texture*) ResourcesManager::getResource(i);
            if (texture != nullptr) {
                texture->unload();
                texture->load();
            }
        }
        reloadTextures = false;
    }


    //Profiler::getTimer(4)->finishMeasurement();
    //Profiler::getTimer(4)->resetCycle();
    //if (unloaded)
        //std::cout << chunkOp.chunkPos << " unloaded in " << Profiler::getTimer(4)->getAverageTime() << "ms" << std::endl;
}

void CityScene::addEntity(Entity *entity, std::string name) {
    if (entity && name != "") {
        if (entity->getParent() == nullptr) {
            // Only add to this map if it's a root entity 
            entities->insert(std::pair<std::string, Entity*>(name, entity));
        }
    }
}