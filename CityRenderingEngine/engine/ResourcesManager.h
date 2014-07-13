/*
 * Author: Rodrigo Castro Azevedo
 * Date: 17/05/2014
 *
 * Description: This class stores, loads and serves as a dynamic database for all resources used in the game.
 * Models, Materials, Shaders, Textures, Colours, Audio files, all of these can be stored here and retrieved without
 * having to load them again, saving useful memory space and computational time to reload them.
 */

// TODO: Make this smarter, implementing a list of who's using each resource, when no one is using it, unload from
// memory, while keeping it in the manager. Only load it again when someone requests it again. Basically make it more
// like a MemoryManager than a ResourceManager.

#pragma once

#include <map>
#include <SDL.h>
#include <string>
#include "Resource.h"

class ResourcesManager {
public:

    /* Amount of time, in milliseconds, that a Resource will remain loaded, if there's no single object using it. */
    static const int IDLE_TTL = 10000;

    /*
     * Loads a resource to memory and add it to the loaded resources list. The resource shouldn't be preloaded before
     * adding it to the manager. The resource should already have an unique name assigned to it, otherwise it won't be
     * added to the manager, and false will be returned. This function will also return false if the resource is null,
     * or if the manager hasn't been initialized. The flag load determines if the Resource should also be loaded now.
     * Setting this to true will load the Resource now, setting it to false will alow this loading to be done later.
     */
    static bool addResource(Resource *resource, bool load);

    /*
     * Removes a resource from the list. This also calls the resource's unload function. It will return false if the
     * resource doesn't exists, or when the manager hasn't been initialized.
     */
    static bool removeResource(int name);

    /*
     * Tells the ResourcesManager that the caller is not going to use the Resource anymore. If the caller was the only
     * user of the Resource, it will also unload and remove the Resource from the ResourcesManager. If there are still
     * other objects using the same Resource, this will be the same as calling the Resource's function removeUser().
     * Return true if it also removed the Resource from the ResourcesManager, or false otherwise.
     */
    static bool releaseResource(int name);

    /* Retrieves a resource that match the provided name, or null if there's no resource with this name. */
    static Resource *getResource(int name);

    /*
     * Checks if a resource is loaded on the manager. This method will always return false if the manager hasn't been
     * initialized before.
     */
    static bool resourceExists(int name);

    /* Starts the resource manager, instantiating the resource list */
    static void initialize();

    /* Unloads all resources and releases all memory allocated. This should be called when the game ends. */
    static void terminate();

    /* Returns the number of resources currently loaded in this manager. */
    static int getResourcesCount() { return (int) resources->size(); }

    /*
     * This function will iterate through every loaded Resource and check if it's still being used by at least one
     * object. If it isn't, it will unload the Resource if the Resource's idle time is greater than IDLE_TTL.
     */
    static void checkForIdles();

    /*
     * Increases the nameSequence counter and returns the next valid name for a Resource. This will always return a
     * different name, until the int wraps around and starts again from zero. The first valid generated name is
     * 1,000,000, while the numbers before that are reserved for fixed naming instead of this generated method.
     */
    static int generateNextName() {
        return nameSequence++;
    }

    /* Locks and unlocks the mutex, to prevent errors while accessing and editting the entities map */
    static void lockMutex() {
        //SDL_mutexP(mutex);
    }

    static void unlockMutex() {
        //SDL_mutexV(mutex);
    }

protected:

    ResourcesManager(void) {}
    ~ResourcesManager(void) {}

    /* The mapping of the resources loaded. They are identified using a string */
    static std::map<int, Resource*> *resources;

    /* This stores the last name that was issued by the ResourcesManager. The first valid name is 1000000. */
    static int nameSequence;

    /* Mutex to prevent errors caused by racing conditions, especially when loading Chunks on worker threads. */
    static SDL_mutex *mutex;

};