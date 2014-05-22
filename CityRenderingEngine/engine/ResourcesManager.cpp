#include "ResourcesManager.h"

std::map<std::string, Resource*> *ResourcesManager::resources = nullptr;

void ResourcesManager::initialize() {
    ResourcesManager::resources = new std::map<std::string, Resource*>();
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

bool ResourcesManager::addResource(Resource *resource) {
    if (resource != nullptr && !resourceExists(resource->getName())) {
        resources->insert(std::pair<std::string, Resource*>(resource->getName(), resource));
        resource->load();
        return true;
    }
    return false;
}

bool ResourcesManager::removeResource(std::string name) {
    if (resourceExists(name)) {
        Resource *resource = getResource(name);
        resource->unload();
        resources->erase(name);
        return true;
    }
    return false;
}

Resource* ResourcesManager::getResource(std::string name) {
    if (ResourcesManager::resourceExists(name)) {
        return resources->at(name);
    }
    return nullptr;
}

bool ResourcesManager::resourceExists(std::string name) {
    if (resources != nullptr && name.size() > 0) {
        return resources->find(name) != resources->end();
    }
    return false;
}