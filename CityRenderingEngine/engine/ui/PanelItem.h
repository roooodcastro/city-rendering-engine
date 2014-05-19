#pragma once

#include "InterfaceItem.h"

/*
 * A panel is an interface item that can have other interface items inside it, just like a panel should.
 * The child items here follow almost the same rules as the entities that are child to another entity,
 * except sor the scaling part. The size of the panel does not influentitate on the size of the child items,
 * because as we're calculating sizes in pixels, this wouldn't make sense. The positioning and rotation,
 * however, are relative to the panel.
 *
 * Basically, the panel is just a complete implementation of the InterfaceItem that supports nested items, as
 * the pure InterfaceItem class is abstract and does not implement the input events
 */

class PanelItem : public InterfaceItem {
public:

	PanelItem(void);
	PanelItem(Vector2 &position, float rotation, Vector2 &size);
	virtual ~PanelItem(void);
	
	/* Mouse events */
	void onMouseMoved(Vector2 &position, Vector2 &amount); // Will fire every time the mouse moves
	void onMouseClick(Uint8 button, Vector2 &position); // Will fire once a mouse button is released
	void onMouseDoubleClick(Uint8 button, Vector2 &position); // Will fire on a double click
	void onMouseButtonDown(Uint8 button, Vector2 &position); // Will fire in every tick that a button is down
	void onMouseButtonUp(Uint8 button, Vector2 &position); // Will fire every time a mouse button is released
	void onMouseWheelScroll(int amount); // Will fire every time the mouse wheel scrolls

	/* Keyboard events */
	void onKeyPress(SDL_Keysym key); // Will fire every time a key is released
	void onKeyDown(SDL_Keysym key); // Will fire in every tick that a key is down
	void onKeyUp(SDL_Keysym key); // Will fire every time a key is released
};

