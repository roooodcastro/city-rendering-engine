/*
 * Author: Rodrigo Castro Azevedo
 * Date: 17/05/2014
 *
 * Description: This class defines a resource interface, to be inherited by all classes that should be considered a
 * resource. It contains 2 functions, load(), that loads the data, and unload() to unload it. This is the class that
 * is used by ResourceManager to manage the resources.
 *
 * Each Resource is indentified by an unique integer name. The Resource's name must be a valid int greater than zero.
 *
 * A Resource should be semi-persistent, its data should usually not be unloaded when the object is destroyed, only if
 * the Resource is not being used by anyone anymore. It should also only load the data inside the specific load
 * function. This prevents data loading or unloading at unwanted times when a new object is created. This allows for
 * greater flexibility, with the object being created anytime but its data only being loaded when necessary or desired.
 * Another advantage to this is the ability to track who and how many objects are using each Resource, so it will know
 * when it's not being used anymore, and if it's not explicitly unloaded, it will unload by itself if it's not been
 * used for a certain amount of time, defined in ResourcesManager::IDLE_TTL.
 */

#pragma once

#include <string>
#include <iostream>
#include "ResourceNames.h"

class Resource {
public:

    Resource(void) {
        this->loaded = false;
        this->valid = true;
        this->numUsers = 0;
        this->idleTime = 0;
        this->name = 0;
    }

    Resource(const Resource &copy) {
        this->name = copy.name;
        this->loaded = copy.loaded;
        this->valid = copy.valid;
        this->numUsers = copy.numUsers;
        this->idleTime = copy.idleTime;
    }

    Resource(int name) {
        this->loaded = false;
        this->valid = false;
        this->name = name;
        this->numUsers = 0;
        this->idleTime = 0;
    }

    virtual ~Resource(void) {};

    /* This function should load the specific resource into memory */
    virtual void load() = 0;

    /* This function should destroy the resource, unloading and releasing its data from memory. */
    virtual void unload() = 0;

    /* Returns the number of objects that are currently using this Resource. */
    int getNumUsers() { return numUsers; }

    /* Tells this Resource that a new user will use the Resource. */
    void addUser() { numUsers++; idleTime = 0; }

    /* Tells this Resource that an user will no longer use the Resource. */
    void removeUser() {
        numUsers = numUsers < 1 ? 0 : numUsers - 1;
    }

    /* Returns true if this Resource is loaded, false otherwise. */
    bool isLoaded() { return loaded; }

    /*
     * Returns true if this Resource is both valid AND loaded, false otherwise. The Resource wil only be valid if it's
     * loaded, this is the expected behaviour of it, as Resources should require loading, and shouldn't be valid before
     * it, or after it's unloaded.
     */
    bool isValid() { return loaded && valid; }

    /* Returns the unique name identifier of this Resource. */
    int getName() { return name; }

protected:

    /* The unique name of the Resource. This name can be used to quickly identity resources. Defaults to "". */
    int name;

    /* The number of "users" that are currently using this Resource. Defaults to zero. */
    int numUsers;

    /* This stores the amount of time passed since this Resource last had any users. Defaults to zero. */
    // TODO: Use the idleTime variable to implement the garbage collector
    int idleTime;

    /*
     * This should reflect the resource's data state. It's used by the ResourceManager, but should also be managed by
     * the implementation of the functions load() and unload(). When load() is called, this should be set to true, even
     * if the loading has failed. To determine if a Resource is in a valid state after load(), use the variable valid.
     * Defaults to false.
     */
    bool loaded;

    /*
     * Defines if this Resource is valid or not. This is only useful after it's been loaded, and can tell if the
     * Resource was successfully loaded or not. Its use does is not necessarily limited to that. It should be set to
     * true after load() was called upon success, or false if the loading was not successful. Defaults to false.
     */
    bool valid;

};