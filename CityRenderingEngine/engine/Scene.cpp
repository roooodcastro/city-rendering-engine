#include "Scene.h"

Scene::Scene() {
    entities = new std::map<std::string, Entity*>();
    //setProjectionMatrix(Matrix4::Perspective(1.0f, -100.0f, 1280/720, 45.0f));
    projectionMatrix = new Matrix4(Matrix4::Perspective(1.0f, -100.0f, 1280/720, 45.0f));
    //setCameraMatrix(Matrix4::Translation(Vector3(0, 0, -10.0f)));
    cameraMatrix = new Matrix4(Matrix4::Translation(Vector3(0, 0, -10.0f)));
    //lightSources = new std::vector<Light*>();
    lightSource = nullptr;
    cameraPos = new Vector3(0, 0, 0);
    cameraRotation = new Vector3(0, 0, 0);
    dragging = false;
    draggingRight = false;
    mutex = SDL_CreateMutex();
    userInterface = nullptr;
    cameraChanged = true;
}

Scene::Scene(const Scene &copy) {
    this->cameraPos = new Vector3(*(copy.cameraPos));
    this->cameraRotation = new Vector3(*(copy.cameraRotation));
    this->cameraMatrix = new Matrix4(*(copy.cameraMatrix));
    this->entities = new std::map<std::string, Entity*>(*(copy.entities));
    //this->lightSources = new std::vector<Light*>(*(copy.lightSources));
    this->lightSource = new Light(*(copy.lightSource));
    this->projectionMatrix = new Matrix4(*(copy.projectionMatrix));
    this->userInterface = new UserInterface(*(copy.userInterface));
    this->mutex = copy.mutex;
    this->cameraChanged = copy.cameraChanged;
}

Scene::Scene(UserInterface *userInterface) {
    entities = new std::map<std::string, Entity*>();
    projectionMatrix = new Matrix4();
    cameraMatrix = new Matrix4();
    this->userInterface = userInterface;
    //lightSources = new std::vector<Light*>();
    lightSource = nullptr;
    cameraPos = new Vector3(0, 0, 0);
    cameraRotation = new Vector3(0, 0, 0);
    dragging = false;
    draggingRight = false;
    cameraChanged = true;
    mutex = SDL_CreateMutex();
}

Scene::~Scene(void) {
    if (userInterface) {
        delete userInterface;
    }
    entities->clear();
    delete entities;
    delete lightSource;
    delete cameraPos;
    delete cameraRotation;
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
        this->cameraRotation->y += (amount.x / 4.0f);
        this->cameraRotation->x += (amount.y / 4.0f);
    } else if (draggingRight) {
        this->cameraPos->x += (amount.x / 20.0f);
        this->cameraPos->y -= (amount.y / 20.0f);
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
        entities->insert(std::pair<std::string, Entity*>(name, entity));
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
    unsigned numEntities = (unsigned) entities->size();
    // Doing this makes it unsafe for entities to add and remove other entities from the Scene, but speeds things up.
    auto it = entities->begin();
    auto itEnd = entities->end();
    for (; it != itEnd; it++) {
        it->second->update(millisElapsed);
    }

    // Calculate WASD movement
    float speed = 10.0f;
    float sinPhi = sinf(toRadians(cameraRotation->x));
    float cosPhi = cosf(toRadians(cameraRotation->x));
    float sinTheta = sinf(toRadians(cameraRotation->y));
    float cosTheta = cosf(toRadians(cameraRotation->y));
    Vector3 movement = Vector3();
    if (Keyboard::isKeyPressed(SDLK_w)) {
        movement += Vector3(-sinTheta * cosPhi, sinPhi, cosPhi * cosTheta);
    }
    if (Keyboard::isKeyPressed(SDLK_s)) {
        movement += -Vector3(-sinTheta * cosPhi, sinPhi, cosPhi * cosTheta);
    }
    if (Keyboard::isKeyPressed(SDLK_a)) {
        movement += Vector3(cosTheta, 0, sinTheta);
    }
    if (Keyboard::isKeyPressed(SDLK_d)) {
        movement += Vector3(-cosTheta, 0, -sinTheta);
    }
    *cameraPos += (movement * speed);
    calculateCameraMatrix();
    unlockMutex();
    Profiler::getTimer(2)->finishMeasurement();
}

void Scene::render(Renderer *renderer, float millisElapsed) {
    bool updatedCameraMatrix = false;
    // Draw Entities
    auto it = entities->begin();
    auto itEnd = entities->end();
    for (; it != itEnd; it++) {
        // We first get the right shader to use with this entity
        Entity *entity = it->second;
        if (entity->getShader() != nullptr && entity->getShader()->isLoaded()) {
            if (*(entity->getShader()) == *(renderer->getCurrentShader())) {
                
            } else {
                renderer->useShader(entity->getShader());
                renderer->updateShaderMatrix("projMatrix", projectionMatrix);
                renderer->updateShaderMatrix("viewMatrix", cameraMatrix);
                cameraChanged = false;
                updatedCameraMatrix = true;
                if (lightSource != nullptr) {
                    lightSource->updateShaderParameters(entity->getShader());
                }
                entity->getShader()->updateShaderParameters(false);
            }
            if (cameraChanged && !updatedCameraMatrix) {
                renderer->updateShaderMatrix("viewMatrix", cameraMatrix);
                cameraChanged = false;
                updatedCameraMatrix = true;
            }
            renderer->updateShaderMatrix("modelMatrix", &(entity->getModelMatrix()));
            entity->draw(millisElapsed);
        }
    }
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