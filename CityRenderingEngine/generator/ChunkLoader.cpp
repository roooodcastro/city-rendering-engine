#include "ChunkLoader.h"

ChunkLoader *ChunkLoader::instance = nullptr;

/* This function is the loop in which the ChunkLoader will perform the loading and unloading operations. */
int chunkLoaderLoop(void *data) {
    ChunkLoader *loader = (ChunkLoader*) data;
    while (loader->isExecuting()) {
        if (loader->getQueue()->size() > 0) {
            loader->lockMutex();
            ChunkOperation operation = loader->getQueue()->front();
            loader->unlockMutex();
            if (operation.load) {
                // Load the Chunk
                Chunk *chunk = nullptr;
                // We first check if while on the queue, the Chunk wasn't loaded already
                City *city = operation.city;
                city->lockMutex();
                bool duplicate = false;
                auto itEnd = city->getChunks()->end();
                for (auto it = city->getChunks()->begin(); it != itEnd; it++) {
                    if (operation.chunkPos == (*it)->getChunkPos()) {
                        duplicate = true;
                    }
                }
                city->unlockMutex();
                if (!duplicate) {
                    if (Chunk::chunkExists(operation.chunkPos)) {
                        // Load it from the disk
                        chunk = Chunk::loadChunk(operation.chunkPos, operation.city);
                    } else {
                        // Generate it
                        chunk = ChunkGenerator::generateChunk(operation.city, operation.chunkPos);
                    }
                    // Add it to the Scene
                    if (chunk != nullptr) {
                        operation.city->addChunk(chunk);
                    }
                }
                loader->lockMutex();
                loader->getQueue()->pop_front();
                loader->unlockMutex();
            } else {
                // Unload the Chunk
                Chunk *chunk = operation.city->getChunkAt(operation.chunkPos, false);
                if (chunk != nullptr) {
                    /* The unload works in three steps: The Chunk is first removed from the entity list of CityScene,
                     * then when CityScene finishes using it, the render thread will be notified of the Chunk's unload
                     * and will unload and delete all OpenGL resources on the end of its next render() call. Finally,
                     * when the Chunk is void of all OpenGL references and is not being used by anything anymore, it's
                     * safe to actually unload and delete it.
                     */
                    if (Naquadah::getInstance()->getCurrentScene()->isEntityInScene(chunk->getEntityName())) {
                        Naquadah::getInstance()->getCurrentScene()->removeEntity(chunk->getEntityName());
                    } else if (chunk->isSafeToDelete()) {
                        // This gives the update() method of the Chunk time to finish, and makes sure it won't be called again.
                        // Only remove the Chunk from the queue once it's fully unloaded
                        loader->lockMutex();
                        loader->getQueue()->pop_front();
                        loader->unlockMutex();
                        operation.city->removeChunk(chunk);
                        chunk->unload();
                        delete chunk;
                        chunk = nullptr;
                    }
                } else {
                    // If Chunk is null, it probably was deleted already, so just remove it from the queue.
                    loader->lockMutex();
                    loader->getQueue()->pop_front();
                    loader->unlockMutex();
                }
            }
        }
    }
    // Clean up and finish
    delete loader;
    return 0;
}

ChunkLoader::ChunkLoader(void) {
    executing = true;
    queue = new std::deque<ChunkOperation>();
    thread = SDL_CreateThread(&chunkLoaderLoop, "", (void*) this);
    mutex = SDL_CreateMutex();
}

void ChunkLoader::loadChunk(const Vector2 &chunkPos, City *city) {
    lockMutex();
    if (!isChunkInQueue(chunkPos)) {
        queue->push_back(ChunkOperation(chunkPos, city, true));
    }
    unlockMutex();
}

void ChunkLoader::unloadChunk(Chunk *chunk, City *city) {
    Vector2 chunkPos = chunk->getChunkPos();
    lockMutex();
    if (!isChunkInQueue(chunkPos)) {
        queue->push_back(ChunkOperation(chunkPos, city, false));
    } else {
        auto itEnd = queue->end();
        for (auto it = queue->begin(); it != itEnd; it++) {
            if ((*it).chunkPos == chunkPos && (*it).load) {
                // Remove the loading operation by turning it into an unloading one. Unloading what's not loaded will
                // not do anything.
                (*it).load = false;
            }
        }
    }
    unlockMutex();
}

bool ChunkLoader::isChunkInQueue(Vector2 chunkPos) {
    lockMutex();
    bool isInTheQueue = false;
    auto itEnd = queue->cend();
    for (auto it = queue->cbegin(); it != itEnd; it++) {
        if ((*it).chunkPos == chunkPos)
            isInTheQueue = true;
    }
    unlockMutex();
    return isInTheQueue;
}

void ChunkLoader::lockMutex() {
    SDL_mutexP(mutex);
}

void ChunkLoader::unlockMutex() {
    SDL_mutexV(mutex);
}