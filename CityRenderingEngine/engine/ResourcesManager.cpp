#include "ResourcesManager.h"

std::map<int, Resource*> *ResourcesManager::resources = nullptr;
int ResourcesManager::nameSequence = 1000000;
SDL_mutex *ResourcesManager::mutex = nullptr;

void ResourcesManager::initialize() {
    ResourcesManager::resources = new std::map<int, Resource*>();
    //mutex = SDL_CreateMutex();
}

void ResourcesManager::terminate() {
    lockMutex();
    // First unload all loaded resources, then delete the map
    for (auto it = resources->begin(); it != resources->end(); it++) {
        (*it).second->unload();
        delete (*it).second;
    }
    delete resources;
    resources = nullptr;
    unlockMutex();
    SDL_DestroyMutex(mutex);
}

bool ResourcesManager::addResource(Resource *resource, bool load) {
    lockMutex();
    if (resource != nullptr && !resourceExists(resource->getName())) {
        resources->insert(std::pair<int, Resource*>(resource->getName(), resource));
        if (load) resource->load();
        unlockMutex();
        return true;
    }
    unlockMutex();
    return false;
}

bool ResourcesManager::removeResource(int name) {
    lockMutex();
    if (resourceExists(name)) {
        Resource *resource = getResource(name);
        resource->unload();
        resources->erase(name);
        delete resource;
        unlockMutex();
        return true;
    }
    unlockMutex();
    return false;
}

bool ResourcesManager::releaseResource(int name) {
    lockMutex();
    if (resourceExists(name)) {
        Resource *resource = getResource(name);
        resource->removeUser();
        if (resource->getNumUsers() <= 0) {
            resource->unload();
            resources->erase(name);
            delete resource;
            unlockMutex();
            return true;
        }
    }
    unlockMutex();
    return false;
}

Resource* ResourcesManager::getResource(int name) {
    lockMutex();
    if (ResourcesManager::resourceExists(name)) {
        Resource *resource = resources->at(name);
        unlockMutex;
        return resource;
    }
    unlockMutex();
    return nullptr;
}

bool ResourcesManager::resourceExists(int name) {
    if (resources != nullptr && name > 0) {
        lockMutex();
        bool find = resources->find(name) != resources->end();
        unlockMutex();
        return find;
    }
    return false;
}

void ResourcesManager::checkForIdles() {
    // TODO: Implement this "garbage collector"
    // Ideally it should only iterate through so many resources on each frame, so it'll take like 10 frames to iterate
    // through it all, and then restart the check. This will (should) prevent lag.
}