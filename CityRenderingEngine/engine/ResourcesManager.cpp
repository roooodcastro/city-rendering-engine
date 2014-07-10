#include "ResourcesManager.h"

std::map<int, Resource*> *ResourcesManager::resources = nullptr;
int ResourcesManager::nameSequence = 1000000;

void ResourcesManager::initialize() {
    ResourcesManager::resources = new std::map<int, Resource*>();
}

void ResourcesManager::terminate() {
    // First unload all loaded resources, then delete the map
    for (auto it = resources->begin(); it != resources->end(); it++) {
        (*it).second->unload();
        delete (*it).second;
    }
    delete resources;
    resources = nullptr;
}

bool ResourcesManager::addResource(Resource *resource, bool load) {
    if (resource != nullptr && !resourceExists(resource->getName())) {
        resources->insert(std::pair<int, Resource*>(resource->getName(), resource));
        if (load) resource->load();
        return true;
    }
    return false;
}

bool ResourcesManager::removeResource(int name) {
    if (resourceExists(name)) {
        Resource *resource = getResource(name);
        resource->unload();
        resources->erase(name);
        delete resource;
        return true;
    }
    return false;
}

bool ResourcesManager::releaseResource(int name) {
    if (resourceExists(name)) {
        Resource *resource = getResource(name);
        resource->removeUser();
        if (resource->getNumUsers() <= 0) {
            resource->unload();
            resources->erase(name);
            delete resource;
            return true;
        }
    }
    return false;
}

Resource* ResourcesManager::getResource(int name) {
    if (ResourcesManager::resourceExists(name)) {
        return resources->at(name);
    }
    return nullptr;
}

bool ResourcesManager::resourceExists(int name) {
    if (resources != nullptr && name > 0) {
        return resources->find(name) != resources->end();
    }
    return false;
}

void ResourcesManager::checkForIdles() {
    // TODO: Implement this "garbage collector"
    // Ideally it should only iterate through so many resources on each frame, so it'll take like 10 frames to iterate
    // through it all, and then restart the check. This will (should) prevent lag.
}