#pragma once

#include "InterfaceItem.h"
#include "../Naquadah.h"
#include "../rendering/Model.h"
//#include "Sound.h"

enum ButtonState {
	BUTTON_NORMAL,
	BUTTON_HOVERED,
	BUTTON_PRESSED,
	BUTTON_SELECTED
};

class ButtonItem : public InterfaceItem {
public:

	ButtonItem(void);
	ButtonItem(Vector2 &position, float rotation, Vector2 &size);
	ButtonItem(Vector2 &position, float rotation, Vector2 &size, Texture *normal, Texture *hovered, Texture *pressed, Texture *selected);
	ButtonItem(Vector2 &position, float rotation, Vector2 &size, const char *resourceName, const char *normal, const char *hovered, const char *pressed, const char *selected);
	virtual ~ButtonItem(void);

	virtual void draw(unsigned millisElapsed, GLuint program);

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

	/* General getters and setters */
	void setSwitchButton(bool isSwitch) { this->switchButton = isSwitch; }
	bool isSwitch() { return switchButton; }
	void setSelected(bool selected) { this->selected = selected; }
	bool isSelected() { return selected; }
	bool isNormal() { return state == BUTTON_NORMAL; }
	bool isHovered() { return state == BUTTON_HOVERED; }
	bool isPressed() { return state == BUTTON_PRESSED; }
	void setPressed(bool pressed) { state = pressed ? BUTTON_PRESSED : BUTTON_NORMAL; }
	virtual void setTexture(Texture *texture);

protected:

	/*
	 * The current state of this button. A button can have four states:
	 *   Normal: The button is in its default state, nothing is happening
	 *   Hovered: The mouse cursor is currently hovering over the button
	 *   Pressed: The mouse is hovering the button and the left button is being pressed
	 *   Selected: The button is currently active, indicating either that the button
	 *   is on, in case it's a switch button, or that the button is triggering an action.
	 * This last state is more like an abstract state, it will not be set automatically by
	 * mouse events, and serves more to make clearer to the player what's happening.
	 */
	ButtonState state;

	/*
	 * Indicates if this button is a switch button. A switch button can be turned on and off,
	 * unlike the regular button that can only be pressed.
	 */
	bool switchButton;
	/*
	 * Indicates whether this button is in the selected state. This is necessary because the state can change on mouse events,
	 * so we must keep record of this information elsewhere.
	 */
	bool selected;

	/* Each texture represent the four different states a button can have */
	Texture *normalTex;
	Texture *hoveredTex;
	Texture *pressedTex;
	Texture *selectedTex;
};

