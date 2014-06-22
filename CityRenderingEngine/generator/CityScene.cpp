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

    // Distance that new chunks should be loaded
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
    for (int x = (int) chunkMin.x; x < (int) chunkMax.x; x += 1000) {
        for (int y = (int) chunkMin.y; y < (int) chunkMax.y; y += 1000) {
            Vector2 chunkCentre = Vector2((float) x, (float) y);
            Vector2 chunkPos = Vector2((float) x - 500.0f, (float) y - 500.0f); // Take it back from the centre of the Chunk
            float distance = (cameraPos2f - chunkCentre).getLength();
            if ((distance - Chunk::CHUNK_SIZE) < chunkViewDistance) {
                // Chunk should be loaded, so we load/generate it if it's not already loaded or being loaded
                if (!city->isChunkLoaded(chunkPos)) {
                    if (chunkPos == Vector2(2000, -4000)) {
                        int a = 1;
                    }
                    city->loadChunk(chunkPos);
                }
            }
        }
    }

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
    if (toBeUnloaded != nullptr) city->unloadChunk(toBeUnloaded);
    city->unlockMutex();


    unlockUpdateMutex();
}

void CityScene::addEntity(Entity *entity, std::string name) {
    if (entity && name != "") {
        if (entity->getParent() == nullptr) {
            // Only add to this map if it's a root entity 
            entities->insert(std::pair<std::string, Entity*>(name, entity));
        }
    }
}