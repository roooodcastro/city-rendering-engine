/*
 * Author: Rodrigo Castro Azevedo
 * Date: 02/07/2014
 *
 * Description: This is a Singleton class designed specifically to load and unload Chunks. It operates on a separate
 * worker thread, allowing the main solution to run smoothly. It implements a Queue, that will queue the Chunks that
 * shoule be loaded or unloaded. The main thread function will run a loop, checking if there's any Chunks in the queue.
 * It there is, it will load/unload the Chunk and move on to the next in the queue.
 */

#pragma once

#include <SDL.h>
#include <deque>
#include "City.h"
#include "../engine/math/Vector2.h"

/* A struct to indicate the queue what operation to perform on a given Chunk. */
struct ChunkOperation {

    /* The position of the Chnk to be loaded or unloaded. */
    Vector2 chunkPos;

    /* The City in which the Chunk is or will be located. */
    City *city;

    /* The operation. True will load a Chunk, false will unload it. */
    bool load;

    /* Simple helper constructors. */
    ChunkOperation(void) : chunkPos(0, 0), city(nullptr), load(true) {}
    ChunkOperation(Vector2 chunkPos, City *city, bool load) : chunkPos(chunkPos), city(city), load(load) {}

};

class ChunkLoader {
public:

    ~ChunkLoader(void) {
        if (queue != nullptr) {
            queue->clear();
            delete queue;
            queue = nullptr;
        }
        if (mutex != nullptr) {
            SDL_DestroyMutex(mutex);
            mutex = nullptr;
        }
    }

    /* Returns the Singleton instance of ChunkLoader. If the instance doesn't exist yet, it will be created. */
    static ChunkLoader *getInstance() {
        if (instance == nullptr) {
            instance = new ChunkLoader();
        }
        return instance;
    }

    /* Deletes the instance of the ChunkLoader. This should be called when the game exits. */
    static void terminate() {
        if (instance != nullptr) {
            instance->executing = false;
            int result;
            SDL_WaitThread(instance->getThread(), &result);
        }
    }

    /* Returns true if this ChunkLoader is still executing, or false otherwise. */
    bool isExecuting() { return executing; }

    /* Returns the SDL_Thread that is executing this ChunkLoader. */
    SDL_Thread *getThread() { return thread; }

    std::deque<ChunkOperation> *getQueue() { return queue; }

    /* Returns the first operation on the queue, without removing it from the queue. */
    ChunkOperation getFirstInTheQueue() {
        if (queue->size() > 0) {
            return queue->front();
        }
        return ChunkOperation();
    }

    /*
     * Adds chunkPos to the queue. The Chunk corresponding to chunkPos will be loaded when possible. If chunkPos is
     * already on the queue, it will not be added a second time.
     */
    void loadChunk(const Vector2 &chunkPos, City *city);

    /*
     * Adds Chunk to the queue to be unloaded. It will be unloaded when possible. If Chunk is already on the queue for
     * unloading, it will not be added a second time. It it's already on the queue but waiting to be loaded, the load
     * operation will be cancelled and the unloading operation ignored.
     */
    void unloadChunk(Chunk *chunk, City *city);

    /* Returns true if chunkPos is already in the queue for either loading or unloading. */
    bool isChunkInQueue(Vector2 chunkPos);

    /* Locks and unlocks the mutex, to prevent racing conditions. */
    void lockMutex();
    void unlockMutex();

protected:

    ChunkLoader(void);

    /*
     * The Queue of ChunkOperations that will hold the Chunks to be loaded or unloaded. ChunkOperation is a struct that
     * contains the Chunk and a bool indicating if the Chunk is to be loaded or unloaded.
     */
    std::deque<ChunkOperation> *queue;

    /* A bool indicating that the ChunkLoader is not yet terminated. Defaults to true. */
    bool executing;

    /* The worker thread that executes this ChunkLoader. */
    SDL_Thread *thread;

    /* Mutex to prevent racing conditions. */
    SDL_mutex *mutex;

    /* The Singleton instance. */
    static ChunkLoader *instance;

};