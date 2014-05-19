/*
 * Author: Rodrigo Castro Azevedo
 * Date: 14/05/2014
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
#include "math/Vector2.h"
#include "math/Common.h"
#include <SDL.h>
#include <GL/glew.h>

class Naquadah {
public:

	Naquadah(void);
	~Naquadah(void);

	/*
	 * Returns the running instance of the engine. If there are no running instances, a new one will be created.
	 */
	static Naquadah *getInstance();

	/*
	 * This method will create a new instance and configure it according to the configurations on
	 * the configuration file specified on the parameter.
	 */
	static void initialize(std::string configFilePath);

	/*
	 * Returns the size of the game window. If the game is in fullscreen, it will simply return the screen resolution.
	 * If the instance hasn't been created yet, it will return (0, 0).
	 */
	static inline Vector2 getWindowSize() { return (instance == nullptr ? Vector2() : Vector2(instance->windowSize)); }

	/*
	 * Returns true if the game is in fullscreen mode, false otherwise. If the instance hasn't been created yet, it
	 * will return false.
	 */
	static inline bool isFullscreen() { return (instance == nullptr ? false : instance->fullscreen); }

protected:

	/* The single instance of this class. Defaults to null, is created and configured using the method initialize(). */
	static Naquadah *instance;

	/* The size of the window. If the game's in fullscreen, it'll return the screen resolution. Defaults to (0, 0). */
	Vector2 windowSize;
	/* True if the game is in fullscreen mode. Defaults to false. */
	bool fullscreen;

	/* The current scene being simulated. Defaults to null. */
	//Scene *currentScene;
	/* The next scene to be simulated. This should not hold a value for more than 1 frame. Defaults to null. */
	//Scene *nextScene;
};
