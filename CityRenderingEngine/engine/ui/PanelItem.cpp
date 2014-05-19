#include "PanelItem.h"

PanelItem::PanelItem(void) : InterfaceItem(ITEM_PANEL) {
}

PanelItem::PanelItem(Vector2 &position, float rotation, Vector2 &size) : InterfaceItem(ITEM_PANEL, position, rotation, size) {
}

PanelItem::~PanelItem(void) {
}

void PanelItem::onMouseMoved(Vector2 &position, Vector2 &amount) {
	position.x -= this->position->x;
	position.y -= this->position->y;
	for (std::vector<InterfaceItem*>::iterator it = innerItems->begin(); it != innerItems->end(); ++it) {
		(*it)->onMouseMoved(position, amount);
	}
}

void PanelItem::onMouseClick(Uint8 button, Vector2 &position) {
	position.x -= this->position->x;
	position.y -= this->position->y;
	for (std::vector<InterfaceItem*>::iterator it = innerItems->begin(); it != innerItems->end(); ++it) {
		(*it)->onMouseClick(button, position);
	}
}

void PanelItem::onMouseDoubleClick(Uint8 button, Vector2 &position) {
	position.x -= this->position->x;
	position.y -= this->position->y;
	for (std::vector<InterfaceItem*>::iterator it = innerItems->begin(); it != innerItems->end(); ++it) {
		(*it)->onMouseDoubleClick(button, position);
	}
}

void PanelItem::onMouseButtonDown(Uint8 button, Vector2 &position) {
	position.x -= this->position->x;
	position.y -= this->position->y;
	for (std::vector<InterfaceItem*>::iterator it = innerItems->begin(); it != innerItems->end(); ++it) {
		(*it)->onMouseButtonDown(button, position);
	}
}

void PanelItem::onMouseButtonUp(Uint8 button, Vector2 &position) {
	position.x -= this->position->x;
	position.y -= this->position->y;
	for (std::vector<InterfaceItem*>::iterator it = innerItems->begin(); it != innerItems->end(); ++it) {
		(*it)->onMouseButtonUp(button, position);
	}
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void PanelItem::onMouseWheelScroll(int amount) {
	for (std::vector<InterfaceItem*>::iterator it = innerItems->begin(); it != innerItems->end(); ++it) {
		(*it)->onMouseWheelScroll(amount);
	}
}

void PanelItem::onKeyPress(SDL_Keysym key) {
	for (std::vector<InterfaceItem*>::iterator it = innerItems->begin(); it != innerItems->end(); ++it) {
		(*it)->onKeyPress(key);
	}
}

void PanelItem::onKeyDown(SDL_Keysym key) {
	for (std::vector<InterfaceItem*>::iterator it = innerItems->begin(); it != innerItems->end(); ++it) {
		(*it)->onKeyDown(key);
	}
}

void PanelItem::onKeyUp(SDL_Keysym key) {
	for (std::vector<InterfaceItem*>::iterator it = innerItems->begin(); it != innerItems->end(); ++it) {
		(*it)->onKeyUp(key);
	}
}