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
#include "ui/UserInterface.h"
#include "Entity.h"
#include "Naquadah.h"
#include "math/Matrix4.h"
#include "rendering/Light.h"

/*
 * A enumerator with the possible level types. Each type will behave differently when it's being processed by the GameApp class.
 */
enum SceneType {
	SCENE_MENU, // A interactive menu screen level, this can be a menu or a end game screen, etc
	SCENE_STATIC_SCREEN, // A static screen level with no user input. Usually used as a loading screen
	SCENE_GAME, // A game level, to be used when the player is actually playing a level
};

class UserInterface;
class Entity;

class Scene {
public:

	Scene(SceneType type);
	Scene(const Scene &copy);
	Scene(SceneType type, UserInterface &userInterface);
	~Scene(void);

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

	virtual void processSceneTick(unsigned int millisElapsed);
	virtual void drawScene(unsigned int millisElapsed);

	/* ==========================================
	 * =============== Other stuff ==============
	 * ==========================================
	 */

	// Adds a new entity to this level
	void addEntity(Entity *entity, std::string name);
	// Removes a entity from this level
	bool removeEntity(std::string name);

	// General getters and setters
	void setCameraMatrix(Matrix4 &cameraMatrix) { (*this->cameraMatrix) = cameraMatrix; }
	Matrix4 getCameraMatrix() { return *cameraMatrix; }
	void setProjectionMatrix(Matrix4 &projectionMatrix) { (*this->projectionMatrix) = projectionMatrix; }
	Matrix4 getProjectionMatrix() { return *projectionMatrix; }
	void setInterface(UserInterface *userInterface) { this->userInterface = userInterface; }
	UserInterface *getUserInterface() { return userInterface; }
	void addLightSource(Light &lightSource);
	void removeLightSource(Light &lightSource);
	std::vector<Light*> *getLightSources() { return lightSources; }
	void setCameraPosition(Vector3 &position) {*(this->cameraPos) = position; }
	Vector3 *getCameraPosition() { return cameraPos; }
	void setCameraRotation(Vector3 &rotation) {*(this->cameraRotation) = rotation; }
	Vector3 *getCameraRotation() { return cameraRotation; }

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

	void applyShaderLight(GLuint program);

	/* Locks and unlocks the mutex, to prevent errors while accessing and editting the entities map */
	void lockMutex();
	void unlockMutex();

	bool dragging;
	bool draggingRight;

protected:

	Scene(void);
	virtual void calculateCameraMatrix() {
		Matrix4 rotationMatrix = Matrix4::Rotation(cameraRotation->x, Vector3(1, 0, 0)) * Matrix4::Rotation(cameraRotation->y, Vector3(0, 1, 0)) * Matrix4::Rotation(cameraRotation->z, Vector3(0, 0, 1));
		*cameraMatrix = Matrix4::Translation(*cameraPos) * rotationMatrix;
	}

	std::map<std::string, Entity*> *entities; // A list with all the entities contained in this level
	std::vector<Light*> *lightSources; // A list of all light sources contained in this level
	SceneType levelType; // The type of this level. MUST be set.

	UserInterface *userInterface; // The player interface, may be the manu, or a HUD
	Vector3 *cameraPos; // The position of the camera
	Vector3 *cameraRotation; // The direction the camera is facing
	Matrix4 *cameraMatrix; // The viewMatrix, or the camera
	Matrix4 *projectionMatrix; // The projectionMatrix. This will be switched all the time to render interface and game

	SDL_mutex *mutex;
};