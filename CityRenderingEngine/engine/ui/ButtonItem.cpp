#include "ButtonItem.h"

ButtonItem::ButtonItem(void) : InterfaceItem(ITEM_BUTTON) {
	state = BUTTON_NORMAL;
	selected = false;
}

ButtonItem::ButtonItem(Vector2 &position, float rotation, Vector2 &size) : InterfaceItem(ITEM_BUTTON, position, rotation, size) {
	state = BUTTON_NORMAL;
	selected = false;
}

ButtonItem::ButtonItem(Vector2 &position, float rotation, Vector2 &size, Texture *normal, Texture *hovered, Texture *pressed, Texture *selected) :
InterfaceItem(ITEM_BUTTON, position, rotation, size) {
	this->normalTex = normal;
	this->texture = this->normalTex;
	this->hoveredTex = hovered;
	this->pressedTex = pressed;
	this->selectedTex = selected;
	this->selected = false;
}

ButtonItem::ButtonItem(Vector2 &position, float rotation, Vector2 &size, const char *resourceName, const char *normal, const char *hovered, const char *pressed, const char *selected) :
	InterfaceItem(ITEM_BUTTON, position, rotation, size) {
		this->normalTex = Texture::getOrCreate((std::string(resourceName) + "_normal").c_str(), normal, true);
		this->texture = this->normalTex;
		this->hoveredTex = Texture::getOrCreate((std::string(resourceName) + "_hovered").c_str(), hovered, true);
		this->pressedTex = Texture::getOrCreate((std::string(resourceName) + "_pressed").c_str(), pressed, true);
		this->selectedTex = Texture::getOrCreate((std::string(resourceName) + "_selected").c_str(), selected, true);
		this->selected = false;
}

ButtonItem::~ButtonItem(void) {
	delete normalTex;
	delete hoveredTex;
	delete pressedTex;
	delete selectedTex;
}

void ButtonItem::onMouseMoved(Vector2 &position, Vector2 &amount) {
	if (state != BUTTON_PRESSED) {
		if (isMouseHovering(position)) {
			if (state != BUTTON_HOVERED) {
				// Play a little SFX
				//Sound::playSfx("ButtonHoverSfx", 16);
			}
			state = BUTTON_HOVERED;
		} else if (selected) {
			state = BUTTON_SELECTED;
		} else {
			state = BUTTON_NORMAL;
		}
	}
}

void ButtonItem::onMouseClick(Uint8 button, Vector2 &position) {
}

void ButtonItem::onMouseDoubleClick(Uint8 button, Vector2 &position) {
	// UNUSED
}

void ButtonItem::onMouseButtonDown(Uint8 button, Vector2 &position) {
	if (isMouseHovering(position) && button == SDL_BUTTON_LEFT) {
		if (state != BUTTON_PRESSED) {
			//Sound::playSfx("ButtonClickSfx", 64);
		}
		state = BUTTON_PRESSED;
	}
}

void ButtonItem::onMouseButtonUp(Uint8 button, Vector2 &position) {
	if (state == BUTTON_PRESSED && button == SDL_BUTTON_LEFT) {
		if (isMouseHovering(position)) {
			state = BUTTON_HOVERED;
			onMouseClick(button, position);
		} else if (selected) {
			state = BUTTON_SELECTED;
		} else {
			state = BUTTON_NORMAL;
		}
	}
}

/*
 * The parameter amount is positive when the wheel is scrolled away from the player,
 * and negative if scrolled toward the player.
 */
void ButtonItem::onMouseWheelScroll(int amount) {
}

void ButtonItem::onKeyPress(SDL_Keysym key) {
}

void ButtonItem::onKeyDown(SDL_Keysym key) {
}

void ButtonItem::onKeyUp(SDL_Keysym key) {
}

void ButtonItem::setTexture(Texture *texture) {
	this->texture = texture;
	this->normalTex = texture;
	this->hoveredTex = texture;
	this->pressedTex = texture;
	this->selectedTex = texture;
}

void ButtonItem::draw(unsigned millisElapsed, GLuint program) {
	if (!hidden) {
		Texture *chosenTex = normalTex;
		switch (state) {
		//case BUTTON_NORMAL:
			//break;
		case BUTTON_HOVERED:
			chosenTex = hoveredTex;
			break;
		case BUTTON_PRESSED:
			chosenTex = pressedTex;
			break;
		case BUTTON_SELECTED:
			chosenTex = selectedTex;
			break;
		}
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, false, (float*) &modelMatrix);
		if (&chosenTex) {
			chosenTex->bindTexture(program, TEXTURE0);
		} else if (normalTex) {
			// Fallback in case one of the textures wasn't specified
			normalTex->bindTexture(program, TEXTURE0);
		}
		Model::getQuadMesh()->draw();
	}
}