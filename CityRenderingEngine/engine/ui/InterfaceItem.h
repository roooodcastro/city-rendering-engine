#pragma once

#include <vector>
#include <algorithm>
#include "../math/Matrix4.h"
#include "../Naquadah.h"
#include "../math/Vector2.h"
#include "../rendering/Model.h"

enum InterfaceItemType {
	ITEM_BUTTON, // A clickable button
	ITEM_IMAGE, // Just a image
	ITEM_3D_FRAME, // A frame that can render a perspective 3D model into it
	ITEM_PANEL, // A panel that can hold child interface items
	ITEM_TEXT, // Just a text on the screen
};

class Naquadah;
class Texture;

class InterfaceItem {
public:
	InterfaceItem(void);
	InterfaceItem(const InterfaceItem &copy);
	InterfaceItem(InterfaceItemType type);
	InterfaceItem(InterfaceItemType type, Vector2 &position, float rotation, Vector2 &size);
	virtual ~InterfaceItem(void);

	/* Gameloop funcions */
	virtual void update(unsigned millisElapsed);
	virtual void draw(float millisElapsed, GLuint program);
	
	/* Mouse events */
	virtual void onMouseMoved(Vector2 &position, Vector2 &amount) = 0; // Will fire every time the mouse moves
	virtual void onMouseClick(Uint8 button, Vector2 &position) = 0; // Will fire once a mouse button is released
	virtual void onMouseDoubleClick(Uint8 button, Vector2 &position) = 0; // Will fire on a double click
	virtual void onMouseButtonDown(Uint8 button, Vector2 &position) = 0; // Will fire in every tick that a button is down
	virtual void onMouseButtonUp(Uint8 button, Vector2 &position) = 0; // Will fire every time a mouse button is released
	virtual void onMouseWheelScroll(int amount) = 0; // Will fire every time the mouse wheel scrolls

	/* Keyboard events */
	virtual void onKeyPress(SDL_Keysym key) = 0; // Will fire every time a key is released
	virtual void onKeyDown(SDL_Keysym key) = 0; // Will fire in every tick that a key is down
	virtual void onKeyUp(SDL_Keysym key) = 0; // Will fire every time a key is released

	/* Calculates is the mouse position provided is inside this interface item */
	bool isMouseHovering(Vector2 &mousePos);

	/* General getters and setters */
	InterfaceItemType getType() { return type; }
	virtual void setTexture(Texture *texture) { this->texture = texture; }
	virtual Texture *getTexture() { return texture; }

	void setPosition(Vector2 &position) { this->position->x = position.x; this->position->y = position.y; }
	Vector2 getPosition() { return *position; }
	void setRotation(float rotation) { this->rotation = rotation; }
	float getRotation() { return rotation; }
	void setSize(Vector2 &size) { this->size = &size; }
	Vector2 getSize() { return *size; }
	void setModelMatrix(Matrix4 &modelMatrix) { this->modelMatrix = modelMatrix; }
	Matrix4 getModelMatrix() { return modelMatrix; }
	void setHidden(bool hidden) { this->hidden = hidden; }
	bool isHidden() { return hidden; }

	/* Returns the actual size, calculating the size in case it is set to some constant, like SIZE_NO_RESIZE */
	Vector2 getRealSize();
	/* Returns the actual screen coordinates, calculating for constrains and different screen resolutions. */
	Vector2 getRealPosition();

	void addInnerItem(InterfaceItem *item);
	void removeInnerItem(InterfaceItem *item);
	std::vector<InterfaceItem*> *getInnerItems() { return innerItems; }

	virtual InterfaceItem &operator=(const InterfaceItem &other);

	const static float SIZE_NO_RESIZE; // Don't scale the item, make it the same size as its texture
	const static float POSITION_CENTERED; // Centers the item relative to the window

protected:

	/* The screen position of this item */
	Vector2 *position;
	/* The screen rotation of this item */
	float rotation;
	/* The size in pixels of this item */
	Vector2 *size;
	/* The type of this item. Different item types will have different behaviour */
	InterfaceItemType type;
	/* The texture to be used to render this item */
	Texture *texture;
	/* The model matrix to be applied to the texture in order to render it. Internal use only */
	Matrix4 modelMatrix;
	/* A vector containing items that may be added to this item. Works in a hierarchy similar to the child/parent entities */
	std::vector<InterfaceItem*> *innerItems;
	/* Indicates if this item is active or not. When hidden, it won't be rendered nor updated. Defaults to false */
	bool hidden;
};