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

void Keyboard::recordKey(SDL_KeyboardEvent keyEvent) {
    SDL_Keycode key = keyEvent.keysym.sym;
    if (keyEvent.type == SDL_KEYDOWN) {
        auto it = find(instance->pressedKeys->begin(), instance->pressedKeys->end(), key);
        if (it == instance->pressedKeys->end()) {
            instance->pressedKeys->emplace_back(key);
        }
    } else {
        instance->pressedKeys->erase(std::remove(instance->pressedKeys->begin(), instance->pressedKeys->end(), key), instance->pressedKeys->end());
    }
}

bool Keyboard::isKeyPressed(SDL_Keycode keyCode) {
    for (std::vector<SDL_Keycode>::iterator it = instance->pressedKeys->begin(); it != instance->pressedKeys->end(); ++it) {
        if ((*it) == keyCode) {
            return true;
        }
    }
    return false;
}