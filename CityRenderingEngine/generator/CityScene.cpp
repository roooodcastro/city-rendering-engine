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
    lockMutex();

    // Distance that new chunks should be loaded
    float chunkViewDistance = 3000.0f;

    // Use current camera position to calculate which Chunks must be loaded or unloaded.
    Vector3 cameraPos = camera->getPosition();
    Vector3 correctedCameraPos = Vector3(cameraPos);
    correctedCameraPos.x = (float) ((int) (cameraPos.x / (float) Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE);
    correctedCameraPos.z = (float) ((int) (cameraPos.z / (float) Chunk::CHUNK_SIZE) * Chunk::CHUNK_SIZE);
    correctedCameraPos.x += Chunk::CHUNK_SIZE / 2.0f; // Make it to the centre of the Chunk, so we can calculate based
    correctedCameraPos.z += Chunk::CHUNK_SIZE / 2.0f; // on the centre of the chunk instead of from one of the corners.
    std::cout << cameraPos << " - " << correctedCameraPos << std::endl;
    Vector2 chunkMin = Vector2(correctedCameraPos.x - chunkViewDistance, correctedCameraPos.z - chunkViewDistance);
    Vector2 chunkMax = Vector2(correctedCameraPos.x + chunkViewDistance, correctedCameraPos.z + chunkViewDistance);

    for (int x = (int) chunkMin.x; x < (int) chunkMax.x; x += 1000) {
        for (int y = (int) chunkMin.y; y < (int) chunkMax.y; y += 1000) {
            Vector2 cameraPos2f = Vector2(cameraPos.x, cameraPos.z);
            Vector2 chunkCentre = Vector2((float) x, (float) y);
            Vector2 chunkPos = Vector2((float) x - 500.0f, (float) y - 500.0f); // Take it back from the centre of the Chunk
            float distance = (cameraPos2f - chunkCentre).getLength();
            if ((distance - Chunk::CHUNK_SIZE) < chunkViewDistance) {
                // Chunk should be loaded, so we load/generate it if it's not already loaded or being loaded
                if (!city->isChunkLoaded(chunkPos)) {
                    city->loadChunk(chunkPos);
                }
            }
        }
    }

    // We now have a bounding square in which all Chunks inside should be loaded, and all outside should be unloaded.

    unlockMutex();
}

void CityScene::addEntity(Entity *entity, std::string name) {
    if (entity && name != "") {
        if (entity->getParent() == nullptr) {
            // Only add to this map if it's a root entity 
            entities->insert(std::pair<std::string, Entity*>(name, entity));
        }
    }
}