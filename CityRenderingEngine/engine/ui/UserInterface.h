#pragma once

#include <stdlib.h>
#include <map>
#include <vector>
#include "InterfaceItem.h"

class InterfaceItem;
class ImageItem;
class Shader;

class UserInterface {
public:

	UserInterface(void);
	UserInterface(const UserInterface &copy);
	virtual ~UserInterface(void);

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

	virtual void update(unsigned millisElapsed);
	virtual void draw(unsigned millisElapsed);

	/* General getters and setters */
	void addItem(InterfaceItem *item, std::string name);
	bool removeItem(InterfaceItem *item, std::string name);
	InterfaceItem *getItem(std::string name);
	std::map<std::string, InterfaceItem*> *getItems() { return items; }
	Shader *getInterfaceShader() { return interfaceShader; }
	/* Returns true if a named item has been added to this interface */
	bool isItemInInterface(std::string name);

	/* Start a fadeIn effect from black, for the specified duration */
	void startFadeIn(int millis);
	/* Start a fadeOut effect to black, for the specified duration */
	void startFadeOut(int millis);

	/* Checks if the interface is fading, no matter in which direction. */
	bool isFading() { return fadeControl > 0; }

	UserInterface &operator=(const UserInterface &other);

protected:

	/*
	 * The list of interface items included in this interface
	 */
	std::map<std::string, InterfaceItem*> *items;

	/* Debug and profiling tools */
	//InterfaceItem *fpsCounter; // Average FPS
	//InterfaceItem *resourcesCounter; // Number of resources allocated
	//InterfaceItem *entitiesCounter; // Number of entities in the current level
	//InterfaceItem *mousePosDisplay; // Current mouse position
	//Vector2 mousePos; // Stores the mouse pos for debugging

	Shader *interfaceShader; // The default shader to be used on the interface. Plain simple shader.
	/* Describes if the interface is fading in (1), fading out (2), or not fading (0). Defaults to 0. */
	int fadeControl;
	/* The black overlay used to fade the screen */
	InterfaceItem *fadeOverlay;
	/* Internal variable used to control the fading effect */
	int fadeStep;
	/* Internal variable used to control the fading effect */
	int fadeDuration;
};