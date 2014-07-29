/*
 * Author: Rodrigo Castro Azevedo
 * Date: 27/07/2014
 *
 * Description: This is the CityScene interface overlay. It renders as a 2D layer over the
 */

#pragma once

#include "../engine/ui/UserInterface.h"

class CitySceneInterface : public UserInterface {
public:

	CitySceneInterface(void);
	CitySceneInterface(const CitySceneInterface &copy);
	virtual ~CitySceneInterface(void);

	/* Mouse events */
	//void onMouseMoved(Vector2 &position, Vector2 &amount); // Will fire every time the mouse moves
	//void onMouseClick(Uint8 button, Vector2 &position); // Will fire once a mouse button is released
	//void onMouseDoubleClick(Uint8 button, Vector2 &position); // Will fire on a double click
	//void onMouseButtonDown(Uint8 button, Vector2 &position); // Will fire in every tick that a button is down
	//void onMouseButtonUp(Uint8 button, Vector2 &position); // Will fire every time a mouse button is released
	//void onMouseWheelScroll(int amount); // Will fire every time the mouse wheel scrolls
	/* Keyboard events */
	//void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
	//void onKeyDown(SDL_Keysym key); // Will fire in every tick that a key is down
	//void onKeyUp(SDL_Keysym key); // Will fire every time a key is released

	virtual void update(unsigned millisElapsed);
	virtual void draw(unsigned millisElapsed);

protected:

};