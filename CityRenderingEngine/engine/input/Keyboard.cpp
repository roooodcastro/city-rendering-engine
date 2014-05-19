#include "Keyboard.h"

Keyboard *Keyboard::instance = new Keyboard();

Keyboard::Keyboard(void) {
	pressedKeys = new std::vector<SDL_Keycode>();
	pressedKeys->reserve(10);
}


Keyboard::~Keyboard(void) {
	pressedKeys->clear();
	delete pressedKeys;
}

void Keyboard::keyDown(SDL_Keysym key) {
	auto it = find(pressedKeys->begin(), pressedKeys->end(), key.sym);
	if (it == pressedKeys->end()) {
		pressedKeys->emplace_back(key.sym);
	}
}

void Keyboard::keyUp(SDL_Keysym key) {
	pressedKeys->erase(std::remove(pressedKeys->begin(), pressedKeys->end(), key.sym), pressedKeys->end());
	// If vector becomes too big, shrink it
	if (pressedKeys->size() > 10) {
		pressedKeys->shrink_to_fit();
	}
}

bool Keyboard::isKeyPressed(SDL_Keycode keyCode) {
	for (std::vector<SDL_Keycode>::iterator it = pressedKeys->begin(); it != pressedKeys->end(); ++it) {
		if ((*it) == keyCode) {
			return true;
		}
	}
	return false;
}