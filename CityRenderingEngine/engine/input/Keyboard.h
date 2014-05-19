#pragma once

#include <vector>
#include <algorithm>
#include <SDL.h>

class Keyboard {
public:

	~Keyboard(void);

	void keyDown(SDL_Keysym key);
	void keyUp(SDL_Keysym key);

	static Keyboard *getInstance() { return instance; }

	/*
	 * Checks if a key is pressed. This information is stored in the pressedKeys vector.
	 * This function will iterate over this vector checking every currently pressed key to
	 * see if it matches with the parameter
	 */
	bool isKeyPressed(SDL_Keycode keyCode);

	/* These functions check if the modifier keys are pressed. They do not distinguish between left and right keys */
	bool isCtrlPressed() { return isKeyPressed(SDLK_LCTRL) || isKeyPressed(SDLK_RCTRL); }
	bool isShiftPressed() { return isKeyPressed(SDLK_LSHIFT) || isKeyPressed(SDLK_RSHIFT); }
	bool isAltPressed() { return isKeyPressed(SDLK_LALT) || isKeyPressed(SDLK_RALT); }

protected:
	Keyboard(void);

	/* A vector containing all keys currently pressed */
	std::vector<SDL_Keycode> *pressedKeys;

	static Keyboard *instance;
};