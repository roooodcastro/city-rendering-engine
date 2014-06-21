#include "Scene.h"

Scene::Scene() {
    entities = new std::map<std::string, Entity*>();
    opaqueEntities = new std::vector<Entity*>();
    opaqueEntities->reserve(25000);
    transparentEntities = new std::vector<Entity*>();
    transparentEntities->reserve(25000);
    //setProjectionMatrix(Matrix4::Perspective(1.0f, -100.0f, 1280/720, 45.0f));
    projectionMatrix = new Matrix4(Matrix4::Perspective(-1.0f, 100.0f, 1280.0f/720.0f, 45.0f));
    cameraMatrix = new Matrix4(Matrix4::Translation(Vector3(0, 0, -10.0f)));
    camera = new Camera();
    //lightSources = new std::vector<Light*>();
    lightSource = nullptr;
    dragging = false;
    draggingRight = false;
    mutex = SDL_CreateMutex();
    userInterface = nullptr;
    frustum = new Frustum(*projectionMatrix * *cameraMatrix);
}

Scene::Scene(const Scene &copy) {
    this->cameraMatrix = new Matrix4(*(copy.cameraMatrix));
    this->entities = new std::map<std::string, Entity*>(*(copy.entities));
    this->opaqueEntities = new std::vector<Entity*>(*(copy.opaqueEntities));
    this->transparentEntities = new std::vector<Entity*>(*(copy.transparentEntities));
    this->camera = new Camera(*(copy.camera));
    //this->lightSources = new std::vector<Light*>(*(copy.lightSources));
    this->lightSource = new Light(*(copy.lightSource));
    this->projectionMatrix = new Matrix4(*(copy.projectionMatrix));
    this->userInterface = new UserInterface(*(copy.userInterface));
    this->mutex = copy.mutex;
    this->frustum = new Frustum(*(copy.frustum));
}

Scene::Scene(UserInterface *userInterface) {
    entities = new std::map<std::string, Entity*>();
    projectionMatrix = new Matrix4();
    cameraMatrix = new Matrix4();
    userInterface = userInterface;
    camera = new Camera();
    //lightSources = new std::vector<Light*>();
    lightSource = nullptr;
    dragging = false;
    draggingRight = false;
    mutex = SDL_CreateMutex();
    frustum = new Frustum(*projectionMatrix * *cameraMatrix);
}

Scene::~Scene(void) {
    if (userInterface != nullptr) {
        delete userInterface;
        userInterface = nullptr;
    }
    if (entities != nullptr) {
        entities->clear();
        delete entities;
        entities = nullptr;
    }
    if (opaqueEntities != nullptr) {
        opaqueEntities->clear();
        delete opaqueEntities;
        opaqueEntities = nullptr;
    }
    if (transparentEntities != nullptr) {
        transparentEntities->clear();
        delete transparentEntities;
        transparentEntities = nullptr;
    }
    if (lightSource != nullptr) {
        delete lightSource;
        lightSource = nullptr;
    }
    if (camera != nullptr) {
        delete camera;
        camera = nullptr;
    }
    if (frustum != nullptr) {
        delete frustum;
        frustum = nullptr;
    }
    SDL_DestroyMutex(mutex);
}

void Scene::onMouseMoved(Vector2 &position, Vector2 &amount) {
    if (userInterface) {
        userInterface->onMouseMoved(position, amount);
    }
    /*if (!GameApp::getInstance()->isGamePaused()) {
        for (auto it = entities->begin(); it != entities->end(); ++it) {
            (*it).second->onMouseMoved(position, amount);
        }
    }*/

    if (dragging) {
        this->camera->rotateCamera(Vector3(amount.y / 4.0f, amount.x / 4.0f, 0));
    } else if (draggingRight) {
        this->camera->moveCamera(Vector3(amount.x / 5.0f, amount.y / 5.0f, 0));
    }
}

void Scene::onMouseClick(Uint8 button, Vector2 &position) {
    if (userInterface) {
        userInterface->onMouseClick(button, position);
    }
    /*if (!GameApp::getInstance()->isGamePaused()) {
        for (auto it = entities->begin(); it != entities->end(); ++it) {
            (*it).second->onMouseClick(button, position);
        }
    }*/
}

void Scene::onMouseDoubleClick(Uint8 button, Vector2 &position) {
    if (userInterface) {
        userInterface->onMouseDoubleClick(button, position);
    }
    /*if (!GameApp::getInstance()->isGamePaused()) {
        for (auto it = entities->begin(); it != entities->end(); ++it) {
            (*it).second->onMouseDoubleClick(button, position);
        }
    }*/
}

void Scene::onMouseButtonDown(Uint8 button, Vector2 &position) {
    if (userInterface) {
        userInterface->onMouseButtonDown(button, position);
    }
    /*if (!GameApp::getInstance()->isGamePaused()) {
        for (auto it = entities->begin(); it != entities->end(); ++it) {
            (*it).second->onMouseButtonDown(button, position);
        }
    }*/

    if (button == SDL_BUTTON_MIDDLE) {
        dragging = true;
    } else if (button == SDL_BUTTON_RIGHT) {
        draggingRight = true;
    }
}

void Scene::onMouseButtonUp(Uint8 button, Vector2 &position) {
    if (userInterface) {
        userInterface->onMouseButtonUp(button, position);
    }
    /*if (!GameApp::getInstance()->isGamePaused()) {
        for (auto it = entities->begin(); it != entities->end(); ++it) {
            (*it).second->onMouseButtonUp(button, position);
        }
    }*/

    if (button == SDL_BUTTON_MIDDLE) {
        dragging = false;
    } else if (button == SDL_BUTTON_RIGHT) {
        draggingRight = false;
    }
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void Scene::onMouseWheelScroll(int amount) {
    if (userInterface) {
        userInterface->onMouseWheelScroll(amount);
    }
    /*if (!GameApp::getInstance()->isGamePaused()) {
        for (auto it = entities->begin(); it != entities->end(); ++it) {
            (*it).second->onMouseWheelScroll(amount);
        }
    }*/
}

void Scene::onKeyPress(SDL_Keysym key) {
    if (userInterface) {
        userInterface->onKeyPress(key);
    }
    /*if (!GameApp::getInstance()->isGamePaused()) {
        for (auto it = entities->begin(); it != entities->end(); ++it) {
            (*it).second->onKeyPress(key);
        }
    }*/
}

void Scene::onKeyDown(SDL_Keysym key) {
    // Now we propagate this event to the interface items
    if (userInterface) {
        userInterface->onKeyDown(key);
    }
    /*if (!GameApp::getInstance()->isGamePaused()) {
        for (auto it = entities->begin(); it != entities->end(); ++it) {
            (*it).second->onKeyDown(key);
        }
    }*/
}

void Scene::onKeyUp(SDL_Keysym key) {
    // Now we propagate this event to the interface items
    if (userInterface) {
        userInterface->onKeyUp(key);
    }
    /*if (!GameApp::getInstance()->isGamePaused()) {
        for (auto it = entities->begin(); it != entities->end(); ++it) {
            (*it).second->onKeyUp(key);
        }
    }*/
}

bool Scene::isEntityInScene(std::string name) {
    return entities->find(name) != entities->end();
}

void Scene::addEntity(Entity *entity, std::string name) {
    if (entity && name != "") {
        if (entity->getParent() == nullptr) {
            // Only add to this map if it's a root entity 
            entities->insert(std::pair<std::string, Entity*>(name, entity));
        }
        // Add it to opaque or transparent lists and reorder them
        // Note that the children entities will also be added to the lists
        std::vector<Entity*> entityTree = entity->getAllChildren(entity);
        auto itEnd = entityTree.end();
        for (auto it = entityTree.begin(); it != itEnd; it++) {
            if ((*it)->getModel() != nullptr && (*it)->isTranslucent()) {
                transparentEntities->push_back((*it));
            } else {
                opaqueEntities->push_back((*it));
            }
        }
        std::sort_heap(transparentEntities->begin(), transparentEntities->end(), Entity::compareByCameraDistance);
        std::sort_heap(opaqueEntities->begin(), opaqueEntities->end(), Entity::compareByCameraDistance);
    }
}

bool Scene::removeEntity(std::string name) {
    if (isEntityInScene(name)) {
        return entities->erase(name) > 0;
    }
    return false;
}

void Scene::update(float millisElapsed) {
    Profiler::getTimer(2)->startMeasurement();
    lockMutex();
    if (userInterface != nullptr)
        userInterface->update(millisElapsed);

    // Calculate WASD movement (only debug purposes, NOT permanent)
    float speed = 10.0f;
    float sinPhi = sinf((float) toRadians(camera->getRotation().x));
    float cosPhi = cosf((float) toRadians(camera->getRotation().x));
    float sinTheta = sinf((float) toRadians(camera->getRotation().y));
    float cosTheta = cosf((float) toRadians(camera->getRotation().y));
    Vector3 movement = Vector3();
    if (Keyboard::isKeyPressed(SDLK_w)) {
        movement -= Vector3(-sinTheta * cosPhi, sinPhi, cosPhi * cosTheta);
    }
    if (Keyboard::isKeyPressed(SDLK_s)) {
        movement += Vector3(-sinTheta * cosPhi, sinPhi, cosPhi * cosTheta);
    }
    if (Keyboard::isKeyPressed(SDLK_a)) {
        movement += Vector3(-cosTheta, 0, -sinTheta);
    }
    if (Keyboard::isKeyPressed(SDLK_d)) {
        movement += Vector3(cosTheta, 0, sinTheta);
    }
    if (movement.getLength() > 0.00001f) {
        camera->moveCamera(movement * speed);
    }

    // Recalculate camera
    if (camera && camera->hasChanged()) {
        *cameraMatrix = camera->buildViewMatrix();
        frustum->updateMatrix(*projectionMatrix * *cameraMatrix);
    }

    unsigned numEntities = (unsigned) entities->size();
    // Doing this makes it unsafe for entities to add and remove other entities from the Scene, but speeds things up.
    auto it = entities->begin();
    auto itEnd = entities->end();
    for (; it != itEnd; it++) {
        it->second->update(millisElapsed);
    }

    // We only set this to false here because the Entities use it to recalculate their distanceToCamera
    camera->setChanged(false);

    unlockMutex();
    Profiler::getTimer(2)->finishMeasurement();
}

void Scene::render(Renderer *renderer, float millisElapsed) {
    bool updatedCameraMatrix = false;
    if (renderer->getCurrentShader() != nullptr && renderer->getCurrentShader()->isLoaded()) {
        renderer->updateShaderMatrix("viewMatrix", cameraMatrix);
    }
    // Draw Entities

    auto it = entities->begin();
    auto itEnd = entities->end();
    for (; it != itEnd; it++) {
        // We first get the right shader to use with this entity
        Entity *entity = it->second;
        if (frustum->isEntityInside(entity)) {
            entity->draw(millisElapsed);
        }
    }


    //auto it = opaqueEntities->rbegin();
    //auto itEnd = opaqueEntities->rend();
    //for (; it != itEnd; it++) {
    //    // We first get the right shader to use with this entity
    //    Entity *entity = (*it);
    //    if (frustum->isEntityInside(entity)) {
    //        if (entity->getShader() != nullptr && entity->getShader()->isLoaded()) {
    //            if (*(entity->getShader()) == *(renderer->getCurrentShader())) {
    //            
    //            } else {
    //                renderer->useShader(entity->getShader());
    //                renderer->updateShaderMatrix("projMatrix", projectionMatrix);
    //                renderer->updateShaderMatrix("viewMatrix", cameraMatrix);
    //                updatedCameraMatrix = true;
    //                if (lightSource != nullptr) {
    //                    lightSource->updateShaderParameters(entity->getShader());
    //                }
    //                entity->getShader()->updateShaderParameters(false);
    //            }
    //            if (!updatedCameraMatrix) {
    //                renderer->updateShaderMatrix("viewMatrix", cameraMatrix);
    //                updatedCameraMatrix = true;
    //            }
    //            renderer->updateShaderMatrix("modelMatrix", &(entity->getModelMatrix()));
    //            entity->draw(millisElapsed);
    //        }
    //    }
    //}
    //std::cout << sum << std::endl;
    // Draw Interface
    if (userInterface != nullptr) {
        //renderer->useShader(userInterface->getInterfaceShader());
        //glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, false, (float*) &(Matrix4::Translation(Vector3(0, 0, 1.0f))));
        //glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, false, (float*) &(Matrix4::Orthographic(-1, 1, 1920.0f, 0, 1080, 0)));
        //userInterface->draw(millisElapsed);
    }
}
//
//void Scene::addLightSource(Light &lightSource) {
//    lightSources->emplace_back(&lightSource);
//}
//
//void Scene::removeLightSource(Light &lightSource) {
//    lightSources->erase(std::remove(lightSources->begin(), lightSources->end(), &lightSource), lightSources->end());
//}

void Scene::applyShaderLight(GLuint program) {
    //unsigned lightCount = (unsigned) lightSources->size();
    //if (lightCount > 0) {
    //    // We first build the arrays to be passed to the shader
    //    Vector3 *positions = new Vector3[lightCount];
    //    Vector3 *colours = new Vector3[lightCount];
    //    float *radii = new float[lightCount];
    //    float *intensities = new float[lightCount];
    //    int *types = new int[lightCount];

    //    for (unsigned i = 0; i < lightCount; i++) {
    //        positions[i] = (*lightSources)[i]->position;
    //        colours[i] = (*lightSources)[i]->colour.getColourVec3();
    //        radii[i] = (*lightSources)[i]->radius;
    //        intensities[i] = (*lightSources)[i]->intensity;
    //        types[i] = (int) (*lightSources)[i]->type;
    //    }

    //    GLuint n1 = glGetUniformLocation(program, "lightColour[]");
    //    glUniform3fv(n1, (int) lightSources->size(), (float*) colours);
    //    GLuint n2 = glGetUniformLocation(program, "lightPos[]");
    //    glUniform3fv(glGetUniformLocation(program, "lightPos[]"), (int) lightSources->size(), (float*) positions);
    //    glUniform1fv(glGetUniformLocation(program, "lightRadius[]"), (int) lightSources->size(), (float*) radii);
    //    glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, (float*) cameraPos);

    //    delete[] positions;
    //    delete[] colours;
    //    delete[] radii;
    //    delete[] intensities;
    //    delete[] types;
    //}
    //glUniform1i(glGetUniformLocation(program, "lightCount"), (int) lightCount);
    ////GameApp::logOpenGLError("APPLY_SHADER_LIGHT");
}

void Scene::lockMutex() {
    SDL_mutexP(mutex);
}

void Scene::unlockMutex() {
    SDL_mutexV(mutex);
}

void Scene::useShader(Shader *shader) {
    if (shader != nullptr) {
        if (!shader->isLoaded()) shader->load(); // If it's not loaded yet, load it.
        if (shader->isLoaded()) { // Check this again just on case there's a problem loading the Shader
            Renderer *renderer = Naquadah::getRenderer();
            if (*shader != *(renderer->getCurrentShader())) {
                renderer->useShader(shader);
                renderer->updateShaderMatrix("projMatrix", projectionMatrix);
                renderer->updateShaderMatrix("viewMatrix", cameraMatrix);
                if (lightSource != nullptr) {
                    lightSource->updateShaderParameters(shader);
                }
                shader->updateShaderParameters(false);
            }
        }
    }
}