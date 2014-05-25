/*
 * Author: Rodrigo Castro Azevedo
 * Date: 17/05/2014
 *
 * Description: This class defines a resource interface, to be inherited by all classes that should be considered a
 * resource. It contains 2 functions, load(), that loads the data, and unload() to unload it. This is the class that
 * is used by ResourceManager to manage the resources.
 *
 * A Resource should be persistent, its data should not be unloaded when the object is destroyed, only when the unload
 * function is called. It should also only load the data inside the specific load function. This prevents data loading
 * at unwanted times when a new object is created. This allows for greater flexibility, with the object being created
 * anytime but its data only being loaded when necessary or desired.
 */

#pragma once

#include <string>

class Resource {
public:

    Resource(void) {
        loaded = false;
        name = "";
    }

    Resource(const Resource &copy) {
        this->name = std::string(copy.name);
        this->loaded = copy.loaded;
    }

    Resource(std::string name) {
        loaded = false;
        this->name = std::string(name);
    }

    virtual ~Resource(void) {};

    /* This function should load the specific resource into memory */
    virtual void load() = 0;

    /* This function should destroy the resource, unloading and releasing its data from memory. */
    virtual void unload() = 0;

    bool isLoaded() { return loaded; }
    std::string getName() { return std::string(name); }

protected:

    /* The unique name of the Resource. This name can be used to quickly identity resources. */
    std::string name;

    /*
     * This should reflect the resource's data state. It's used by the ResourceManager, but should also be managed by
     * the implementation of the functions load() and unload().
     */
    bool loaded;

};