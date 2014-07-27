/*
 * Author: Rodrigo Castro Azevedo
 * Date: 17/05/2014
 *
 * Description: 
 */

#pragma once

#include <map>
#include <vector>
#include <algorithm>
#include "Entity.h"
#include "Naquadah.h"
#include "math/Common.h"
#include "math/Matrix4.h"
#include "rendering/Light.h"
#include "ui/UserInterface.h"
#include "rendering/Camera.h"
#include "rendering/Skybox.h"
#include "rendering/Frustum.h"
#include "rendering/Renderer.h"

class UserInterface;
class Renderer;
class Frustum;
class Entity;
class Skybox;
class Light;

class Scene {
public:

    Scene(void);
    Scene(const Scene &copy);
    Scene(UserInterface *userInterface);
    virtual ~Scene(void);

    Scene &operator=(const Scene *other);

    /* ==========================================
     * ================= Events =================
     * ==========================================
     */

    /* Scene related events */
    virtual void onStart() {} // Will fire when the current level of the game is switched to this
    virtual void onPause() {} // Will fire when the gmae pauses
    virtual void onResume() {} // Will fire when the game restarts from a pause state
    virtual void onFinish() {} // Will fire when the current level of the game is switched to another one
    /* Mouse events */
    virtual void onMouseMoved(Vector2 &position, Vector2 &amount); // Will fire every time the mouse moves
    virtual void onMouseClick(Uint8 button, Vector2 &position); // Will fire once a mouse button is released
    virtual void onMouseDoubleClick(Uint8 button, Vector2 &position); // Will fire on a double click
    virtual void onMouseButtonDown(Uint8 button, Vector2 &position); // Will fire in every tick that a button is down
    virtual void onMouseButtonUp(Uint8 button, Vector2 &position); // Will fire every time a mouse button is released
    virtual void onMouseWheelScroll(int amount); // Will fire every time the mouse wheel scrolls
    /* Keyboard events */
    virtual void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
    virtual void onKeyDown(SDL_Keysym key); // Will fire in every tick that a key is down
    virtual void onKeyUp(SDL_Keysym key); // Will fire every time a key is released

    /* ==========================================
     * ============ Game loop control ===========
     * ==========================================
     */

    /* Updates the Scene logic. */
    virtual void update(float millisElapsed);

    /* Renders all the Scene objects and the interface. Renderer is the active renderer on the engine. */
    virtual void render(Renderer *renderer, float millisElapsed);

    /* ==========================================
     * =============== Other stuff ==============
     * ==========================================
     */

    // Adds a new entity to this level
    virtual void addEntity(Entity *entity, std::string name);
    // Removes a entity from this level
    bool removeEntity(std::string name);

    // General getters and setters
    void setCameraMatrix(Matrix4 &cameraMatrix) { (*this->cameraMatrix) = cameraMatrix; }
    Matrix4 getCameraMatrix() { return *cameraMatrix; }
    void setProjectionMatrix(Matrix4 &projectionMatrix) { (*this->projectionMatrix) = projectionMatrix; }
    Matrix4 getProjectionMatrix() { return *projectionMatrix; }
    void setInterface(UserInterface *userInterface) { this->userInterface = userInterface; }
    UserInterface *getUserInterface() { return userInterface; }
    void setLightSource(Light *lightSource) { this->lightSource = lightSource; }
    Light *getLightSource() { return lightSource; }
    //void addLightSource(Light &lightSource);
    //void removeLightSource(Light &lightSource);
    //std::vector<Light*> *getLightSources() { return lightSources; }
    void setCamera(Camera &camera) { *(this->camera) = camera; camera.setChanged(true); }
    Camera *getCamera() const { return camera; }
    Frustum *getFrustum() const { return frustum; }
    void setSkybox(Skybox *skybox) { this->skybox = skybox; }
    Skybox *getSkybox() const { return skybox; }


    /* Checks if the entity with the provided name has been added to this level */
    bool isEntityInScene(std::string name);

    Entity *getEntity(std::string name) {
        try {
            return entities->at(name);
        } catch (int &) {
            return NULL;
        }
    }

    std::map<std::string, Entity*> *getEntities() { return entities; }

    /*
     * Uses the Shader specified. If the Shader is already being used, it won't do anything. If the Shader is switched,
     * the projection and view matrices will be updates in the new Shader, which may cause lag. Careful ordering should
     * be used in order to minimize Shader changes.
     */
    void useShader(Shader *shader);

    void applyShaderLight(GLuint program);

    /* Locks and unlocks the mutex, to prevent errors while accessing and editting the entities map */
    void lockUpdateMutex();
    void unlockUpdateMutex();
    void lockRenderMutex();
    void unlockRenderMutex();

    bool dragging;
    bool draggingRight;

protected:

    /* A list with all the root (parent) entities contained in this level. No child entity should be added here. */
    std::map<std::string, Entity*> *entities;
    /* A list with all the opaque entities, including children, ordered from the closest to the farthest. */
    std::vector<Entity*> *opaqueEntities;
    /* A list with all the transparent entities, including children, ordered from the farthest fo the closest. */
    std::vector<Entity*> *transparentEntities;

    /* The Frustum used by this Scene to perform Frustum Culling. */
    Frustum *frustum;

    /* The LightSource for this Scene. Defaults to null. */
    Light *lightSource;

    /* An optional Skybox to render as background. */
    Skybox *skybox;

    /* The player interface, may be a menu, or a HUD. */
    UserInterface *userInterface;

    /* The current Camera being used by this Scene. */
    Camera *camera;
    Matrix4 *cameraMatrix; // The viewMatrix, or the camera
    Matrix4 *projectionMatrix; // The projectionMatrix. This will be switched all the time to render interface and game

    SDL_mutex *updateMutex;
    SDL_mutex *renderMutex;
};