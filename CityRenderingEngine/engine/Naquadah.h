/*
 * Author: Rodrigo Castro Azevedo
 * Date: 14/05/2014
 *
 * TODO: Write a small description of the engine itself
 *
 * Description: This is the main class for the (game) engine. The engine itself is called Naquadah,
 * and this class is used to access its most important and core funcionalities.
 *
 * It is intended to use as a Singleton, having methods to access the single instance and also a few
 * more common helper methods to avoid having to get the instance every time.
 */

#pragma once

#include <stdlib.h>
#include <iostream>
#include <GL/glew.h>
#include <functional>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "math/Vector2.h"
#include "math/Common.h"
#include "input/Mouse.h"
#include "input/Keyboard.h"
#include "input/ConfigurationManager.h"
#include "rendering/Renderer.h"
#include "physics/Simulation.h"
#include "Scene.h"

class Scene;
class Renderer;
class Simulation;

class Naquadah {
public:

    /* The target frame per second. The engine will calibrate the timer to maintain this FPS. Defaults to 60 FPS. */
    static int TARGET_FPS;

    /* Initialization codes to tell Naquadah which functionalities to initialize. */
    static const int NAQUADAH_INIT_EVERYTHING = 0x80000000;
    static const int NAQUADAH_INIT_CORE = 0x00000000;
    static const int NAQUADAH_INIT_GRAPHICS = 0x00000001;
    static const int NAQUADAH_INIT_PHYSICS = 0x00000002;

    Naquadah(void);
    ~Naquadah(void);

    /*
     * Returns the running instance of the engine. If there are no running instances, a new one will be created.
     */
    static Naquadah *getInstance();

    /*
     * This method will create a new instance and initialize only the selected modules on initModuless. To initialize
     * everything, use NAQUADAH_INIT_EVERYTHING. To initialize only the core functionality and the graphics module, use
     * NAQUADAH_INIT_GRAPHICS. The core module is always initialized, so calling 'initialize(0)' will only initialize
     * the core functionalities.
     *
     * To initialize all values correctly, a configuration file must be provided. The default fileName of this file is
     * 'settings.cfg', but it can be changed using the method 'ConfigurationManager::setConfigFileName()'. This change
     * must be done BEFORE calling this method to initialize using the correct configurations.
     */
    static void initialize(unsigned int initModules);

    /*
     * The actual game loop. Inside this loop the game will check for player input and other minor processing tasks.
     * This method will only return when the player quits the game or when SDL crashes.
     */
    void runGame();

    /*
     * This method will tell the game app to stop the game execution and quit. The game won't close instantly though,
     * it will allow time to finish the current frame and unload every resource from memory first.
     */
    void exitGame() { gameRunning = false; }

    /*
     * Function that will be called to handle special events input, like the player exiting the game, the timer firing
     * rocessing methods, and also some player input like keys and mouse movement. This wil be called in a while(true)
     * loop with 0 or 1ms delay between calls.
     */
    void handleUserEvents();

    /*
     * Function that will process all game logic, except the physics. millisElapsed represents the time passed since
     * the last call to updateLogic. It does not represent the time since the last render or physics update call.
     */
    void updateLogic(float millisElapsed);

    /*
     * Function that will process the game physics. millisElapsed represents the time passed since the last call to
     * updatePhysics. It does not represent the time since the last render or logic update call.
     */
    void updatePhysics(float millisElapsed);

    /*
     * Function that will render the game to the screen. millisElapsed represents the time passed since the last call
     * to render. It does not represent the time since the last logic or physics update call.
     */
    void render(float millisElapsed);

    /*
     * Sets the next Scene for the game. The engine will switch the scenes in the neginning of the next frame. It will
     * also destroy the previous scene, if there's any.
     */
    void setNextScene(Scene *nextScene) { this->nextScene = nextScene; }

    /*
     * Returns the size of the game window. If the instance hasn't been created yet, or if the Renderer is not present,
     * it will return (0, 0).
     */
    static Vector2 getWindowSize();

    /* Returns the Renderer for the running engine instance. Note that this can be null. */
    static Renderer *getRenderer() { return ((instance == nullptr) ? nullptr : instance->renderer); }

    /* Returns the Physics Simulation for the running engine instance. Note that this can be null. */
    static Simulation *getSimulation() { return ((instance == nullptr) ? nullptr : instance->simulation); }

protected:

    /* The single instance of this class. Defaults to null, is created and configured using the method initialize(). */
    static Naquadah *instance;

    /* The game will keep running until this flag is set to false. */
    bool gameRunning;

    /*
     * Set it to true to enable profiling, false to disable it. Enabling or disabling the profiling tool does not make
     * it be rendered, it just makes the game calculate profiling information. To render it, use the Renderer's method
     * setProfilingOn(true). Defaults to false.
     */
    bool profiling;

    /* The current scene being simulated. Defaults to null. */
    Scene *currentScene;

    /*
     * The next scene to be simulated. When this is set to a non-null value, the game will detect it and on the start
     * of the next frame it will call the onFinish() method of the currentScene, switch the scenes and start the next
     * scene, and this variable will be set back to null. Defaults to null.
     */
    Scene *nextScene;
    
    /*
     * The Renderer that will be used to render the game. It's an optional functionality, so it may not be
     * initialized. Defaults to null.
     */
    Renderer *renderer;

    /*
     * The Physics simulation that will be used to simulate the game's physics. It's an optional functionality, so it
     * may not be initialized. Defaults to null.
     */
    Simulation *simulation;
    
    /*
     * Initializes both update and render timers. Those timers are responsible for stabilizing the general FPS to the
     * FPS set in TARGET_FPS, or as high as possible. The timers are independent of one another, creating a smooth
     * running game that is more physically accurate and responds faster to player input while still getting good FPS.
     */
    void installTimers();

    /*
     * Callback functions for the timers. Each function basically creates a new event that will be processed by the 
     * handleUserEvents method to either render a frame or process a game update.
     */
    static Uint32 updateLoopTimer(Uint32 interval, void *param);
    static Uint32 renderLoopTimer(Uint32 interval, void *param);

    /* Timers ID for SDL. */
    SDL_TimerID updateTimer;
    SDL_TimerID renderTimer;
};