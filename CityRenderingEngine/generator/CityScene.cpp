#include "CityScene.h"

CityScene::CityScene() : Scene() {
    this->city = nullptr;
}

CityScene::CityScene(const CityScene &copy) : Scene(copy) {
    this->city = new City(*(copy.city));
}

CityScene::CityScene(City *city) : Scene() {
    this->city = city;
}

CityScene::~CityScene(void) {
    if (city != nullptr) {
        delete city;
        city = nullptr;
    }
}

void CityScene::update(float millisElapsed) {
    Scene::update(millisElapsed);
    lockUpdateMutex();

    // TODO: Move distance that new chunks should be loaded to config
    float chunkViewDistance = 3000.0f;

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
    Scene::render(renderer, millisElapsed);

    // Check if there's a Chunk waiting for the OpenGL stuff be unloaded
    //Profiler::getTimer(4)->startMeasurement();
    bool unloaded = false;
    ChunkOperation chunkOp = ChunkLoader::getInstance()->getFirstInTheQueue();
    if (chunkOp.city != nullptr && !chunkOp.load) {
        Chunk *chunk = chunkOp.city->getChunkAt(chunkOp.chunkPos, false);
        if (chunk != nullptr && !chunk->isSafeToDelete()) {
            //chunk->unloadOpenGL();
            chunk->setSafeToDelete(true);
            unloaded = true;
        }
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