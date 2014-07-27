#include "City.h"
#include "ChunkLoader.h"

City::City(void) {
    chunks = new std::vector<Chunk*>();
    mutex = SDL_CreateMutex();
}

City::~City(void) {
    if (chunks != nullptr) {
        chunks->clear();
        delete chunks;
        chunks = nullptr;
    }
    if (mutex != nullptr) {
        SDL_DestroyMutex(mutex);
        mutex = nullptr;
    }
}

void City::addChunk(Chunk *chunk) {
    lockMutex();
    chunks->push_back(chunk);
    Naquadah::getInstance()->getCurrentScene()->addEntity(chunk, chunk->getEntityName());
    unlockMutex();
}

void City::removeChunk(Chunk *chunk) {
    lockMutex();
    if (chunk != nullptr) {
        chunks->erase(std::remove(chunks->begin(), chunks->end(), chunk), chunks->end());
    }
    unlockMutex();
}

bool City::isChunkLoaded(const Vector2 &chunkPos) {
    lockMutex();
    for (auto it = chunks->begin(); it != chunks->end(); it++) {
        if (*it) {
            Vector2 pos = (*it)->getChunkPos();
            if (pos == chunkPos) {
                unlockMutex();
                return true;
            }
        }
    }
    unlockMutex();
    return false;
}

Chunk *City::getChunkAt(const Vector2 &chunkPos, bool loadFromDisk) {
    lockMutex();
    for (auto it = chunks->begin(); it != chunks->end(); it++) {
        if (*it) {
            Vector2 pos = (*it)->getChunkPos();
            if (pos == chunkPos) {
                unlockMutex();
                return (*it);
            }
        }
    }
    unlockMutex();
    return nullptr;
}

std::vector<Chunk*> City::getNeighbourChunks(Chunk *chunk, bool loadFromDisk) {
    lockMutex();
    std::vector<Chunk*> neighbours = std::vector<Chunk*>();
    Vector2 minPos = chunk->getChunkPos() - 1000.0f;
    Vector2 maxPos = chunk->getChunkPos() + 1000.0f;
    for (int i = (int) minPos.x; i <= (int) maxPos.x; i += 1000) {
        for (int j = (int) minPos.y; j <= (int) maxPos.y; j += 1000) {
            Chunk *neighbour = getChunkAt(Vector2((float) i, (float) j), loadFromDisk);
            if (neighbour != nullptr && neighbour != chunk) {
                neighbours.push_back(neighbour);
            }
        }
    }
    unlockMutex();
    return neighbours;
}

void City::lockMutex() {
    SDL_mutexP(mutex);
}

void City::unlockMutex() {
    SDL_mutexV(mutex);
}