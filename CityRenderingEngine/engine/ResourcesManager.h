/*
 * Author: Rodrigo Castro Azevedo
 * Date: 17/05/2014
 *
 * Description: This class stores, loads and serves as a dynamic database for all resources used in the game.
 * Models, Materials, Shaders, Textures, Colours, Audio files, all of these can be stored here and retrieved without
 * having to load them again, saving useful memory space and computational time to reload them.
 */

#pragma once

#include <map>
#include <string>
#include "Resource.h"

class ResourcesManager {
public:

    /*
     * Loads a resource to memory and add it to the loaded resources list. The resource shouldn't be preloaded before
     * adding it to the manager. The resource should already have an unique name assigned to it, otherwise it won't be
     * added to the manager, and false will be returned. This function will also return false if the resource is null,
     * or if the manager hasn't been initialized. The flag load determines if the Resource should also be loaded.
     * Setting this to true will load the Resource now, setting it to false will alow this loading to be done later.
     */
    static bool addResource(Resource *resource, bool load);

    /*
     * Removes a resource from the list. This also calls the resource's unload function. It will return false if the
     * resource doesn't exists, or when the manager hasn't been initialized.
     */
    static bool removeResource(std::string name);

    /* Retrieves a resource that match the provided name, or null if there's no resource with this name. */
    static Resource *getResource(std::string name);

    /*
     * Checks if a resource is loaded on the manager. This method will always return false if the manager hasn't been
     * initialized before.
     */
    static bool resourceExists(std::string name);

    /* Starts the resource manager, instantiating the resource list */
    static void initialize();

    /* Unloads all resources and releases all memory allocated. This should be called when the game ends. */
    static void terminate();

    /* Returns the number of resources currently loaded in this manager. */
    static int getResourcesCount() { return (int) resources->size(); }


protected:

    ResourcesManager(void) {}
     ~ResourcesManager(void) {}

    /* The mapping of the resources loaded. They are identified using a string */
    static std::map<std::string, Resource*> *resources;

};