/*
 * Author: Rodrigo Castro Azevedo
 * Date: 17/05/2014
 *
 * Description: 
 */

#pragma once

#include "City.h"
#include "ChunkLoader.h"
#include "../engine/Scene.h"
#include "CitySceneInterface.h"

class CityScene : public Scene {
public:

    CityScene(void);
    CityScene(City *city);
    CityScene(const CityScene &copy);
    virtual ~CityScene(void);

    CityScene &operator=(const CityScene *other);

    /* Scene related events */
    virtual void onStart() {} // Will fire when the current level of the game is switched to this
    virtual void onPause() {} // Will fire when the gmae pauses
    virtual void onResume() {} // Will fire when the game restarts from a pause state
    virtual void onFinish() {} // Will fire when the current level of the game is switched to another one
    /* Mouse events */
    //virtual void onMouseMoved(Vector2 &position, Vector2 &amount); // Will fire every time the mouse moves
    //virtual void onMouseClick(Uint8 button, Vector2 &position); // Will fire once a mouse button is released
    //virtual void onMouseDoubleClick(Uint8 button, Vector2 &position); // Will fire on a double click
    //virtual void onMouseButtonDown(Uint8 button, Vector2 &position); // Will fire in every tick that a button is down
    //virtual void onMouseButtonUp(Uint8 button, Vector2 &position); // Will fire every time a mouse button is released
    //virtual void onMouseWheelScroll(int amount); // Will fire every time the mouse wheel scrolls
    /* Keyboard events */
    virtual void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
    //virtual void onKeyDown(SDL_Keysym key); // Will fire in every tick that a key is down
    //virtual void onKeyUp(SDL_Keysym key); // Will fire every time a key is released

    virtual void addEntity(Entity *entity, std::string name);

    /* Updates the Scene logic. */
    virtual void update(float millisElapsed);

    /* Renders all the Scene objects and the interface. Renderer is the active renderer on the engine. */
    virtual void render(Renderer *renderer, float millisElapsed);

    /* Renders all the Scene objects and the interface. Renderer is the active renderer on the engine. */
    //virtual void render(Renderer *renderer, float millisElapsed);

    void setCity(City *city) { this->city = city; }
    City *getCity() { return city; }

protected:

    /* Control flag to determine if the shaders need to be reloaded. Debug tool. Defaults to false. */
    bool reloadShaders;
    bool reloadTextures;

    /* The City that will be simulated in this CityScene. */
    City *city;
};