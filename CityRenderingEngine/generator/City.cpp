#include "City.h"

City::City(void) {
    intersections = new std::vector<Intersection*>();
    cityBlocks = new std::vector<CityBlock*>();
    chunks = new std::vector<Chunk*>();
    chunksLoading = std::vector<Vector2>();
    mutex = SDL_CreateMutex();
}

City::~City(void) {
    if (intersections != nullptr) {
        intersections->clear();
        delete intersections;
        intersections = nullptr;
    }
    if (cityBlocks != nullptr) {
        cityBlocks->clear();
        delete cityBlocks;
        cityBlocks = nullptr;
    }
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

Intersection *City::getIntersectionAt(Vector3 position) {
    for (int i = 0; i < intersections->size(); i++) {
        if (intersections->at(i)->getPosition() == position) {
            return intersections->at(i);
        }
    }
    return nullptr;
}

/* Just a struct to send parameters with the function SDL_CreateThread. */
typedef struct {
    Vector2 chunkPos;
    City *city;
} ThreadParams;

/* Function called by SDL_CraeteThread to run this code on a worker thread, so it won't lag the simulation. */
int loadChunkWorkerThread(void *data) {
    ThreadParams *params = (ThreadParams*) data;
    Vector2 chunkPos = params->chunkPos;
    Chunk *chunk = nullptr;
    if (Chunk::chunkExists(chunkPos)) {
        // Load it from the disk
        chunk = Chunk::loadChunk(chunkPos);
    } else {
        // Generate it
        chunk = ChunkGenerator::generateChunk(params->city, chunkPos);
    }
    // Add it to the Scene
    params->city->addChunk(chunk);
    delete params;
    return 0;
}

void City::loadChunk(const Vector2 &chunkPos) {
    // First we check if position is valid (if both X and Y are multiple of 1000)
    if ((int) chunkPos.x % 1000 != 0 || (int) chunkPos.y % 1000 != 0) {
        return;
    }
    // Check if the Chunks that were loading before have finished loading
    auto chunkToBeRemoved = chunksLoading.end();
    bool duplicate = false;
    for (auto it = chunksLoading.begin(); it != chunksLoading.end(); it++) {
        // We also check if we're trying to load a Chunks that's already being loaded
        if (isChunkLoaded(*it)) {
            chunkToBeRemoved = std::find(chunksLoading.begin(), chunksLoading.end(), *it);
        }
        if (*it == chunkPos) duplicate = true;
    }
    if (chunkToBeRemoved != chunksLoading.end()) {
        chunksLoading.erase(chunkToBeRemoved);
    }
    if (chunksLoading.size() < MAX_PARALEL_LOADING_CHUNKS && !duplicate) {
        chunksLoading.push_back(chunkPos);
        ThreadParams *params = new ThreadParams();
        params->chunkPos = chunkPos;
        params->city = this;
        SDL_Thread *thread = SDL_CreateThread(&loadChunkWorkerThread, "", (void*) params);
    }
}

bool City::isChunkLoaded(const Vector2 &chunkPos) {
    lockMutex();
    for (auto it = chunks->begin(); it != chunks->end(); it++) {
        if (*it) {
            Vector3 pos3 = (*it)->getPosition();
            Vector2 pos = Vector2(pos3.x, pos3.z);
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
            Vector3 pos3 = (*it)->getPosition();
            Vector2 pos = Vector2(pos3.x, pos3.z);
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
    for (int i = (int) minPos.x; i < (int) maxPos.x; i += 1000) {
        for (int j = (int) minPos.y; j < (int) maxPos.y; j += 1000) {
            Chunk *neighbour = getChunkAt(Vector2(i, j), loadFromDisk);
            if (neighbour != nullptr && neighbour != chunk) {
                neighbours.push_back(neighbour);
            }
        }
    }
    unlockMutex();
    return neighbours;
}

City *City::generateManhattanGrid(int width, int height) {
    City *manhattan = new City();
    Vector2 gridOffset = Vector2((float) (width * 110), (float) (height * 110));
    for (int i = 0; i <= width; i++) { // North-South roads (columns)
        for (int j = 0; j <= height; j++) { // East-West roads (rows)
            Vector3 position = Vector3((j * 220.0f) - gridOffset.x, 0, (i * 220.0f) - gridOffset.y);
            Intersection *intersection = new Intersection(Vector3(position));
            std::stringstream intersectionName;
            intersectionName << "I" << intersection->getPosition();
            Naquadah::getInstance()->getCurrentScene()->addEntity(intersection, intersectionName.str());
            manhattan->intersections->emplace_back(intersection);
            if (j > 0) {
                // Create road between j-1 and j
                Vector3 previousPosition = Vector3(((j - 1) * 220.0f) - gridOffset.x, 0, (i * 220.0f) - gridOffset.y);
                intersection->connectTo(manhattan->getIntersectionAt(previousPosition));
            }
            if (i > 0) {
                // Create road between i-1 and i
                Vector3 previousPosition = Vector3((j * 220.0f) - gridOffset.x, 0, ((i - 1) * 220.0f) - gridOffset.y);
                intersection->connectTo(manhattan->getIntersectionAt(previousPosition));
            }
            if (i > 0 && j > 0) {
                // Create a CityBlock around ABCD, with A=(i-1,j-1), B=(i-1,j), C=(i,j-1), D=(i,j)
                CityBlock *cityBlock = new CityBlock();
                cityBlock->addVertice(manhattan->getIntersectionAt(Vector3(((j - 1) * 220.0f) - gridOffset.x, 0, ((i - 1) * 220.0f) - gridOffset.y)));
                cityBlock->addVertice(manhattan->getIntersectionAt(Vector3((j * 220.0f) - gridOffset.x, 0, ((i - 1) * 220.0f) - gridOffset.y)));
                cityBlock->addVertice(manhattan->getIntersectionAt(Vector3(((j - 1) * 220.0f) - gridOffset.x, 0, (i * 220.0f) - gridOffset.y)));
                cityBlock->addVertice(intersection);
                cityBlock->generateBuildings();
                manhattan->cityBlocks->emplace_back(cityBlock);
                std::stringstream blockName;
                blockName << "CB" << cityBlock->getPosition();
                Naquadah::getInstance()->getCurrentScene()->addEntity(cityBlock, blockName.str());
            }
        }
    }
    return manhattan;
}

void City::addChunk(Chunk *chunk) {
    lockMutex();
    chunks->push_back(chunk);
    Naquadah::getInstance()->getCurrentScene()->addEntity(chunk, chunk->getEntityName());
    unlockMutex();
}

void City::lockMutex() {
    SDL_mutexP(mutex);
}

void City::unlockMutex() {
    SDL_mutexV(mutex);
}