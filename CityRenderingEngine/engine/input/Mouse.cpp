#include "Mouse.h"

Mouse *Mouse::instance = new Mouse();

Mouse::Mouse(void) {
    mousePos = Vector2();
    pressedButtons[0] = pressedButtons[1] = pressedButtons[2] = false;
}

void Mouse::recordMouseButton(SDL_MouseButtonEvent buttonEvent) {
    instance->pressedButtons[buttonEvent.button - 1] = buttonEvent.type == SDL_MOUSEBUTTONDOWN;
}

void Mouse::recordMouseMove(SDL_MouseMotionEvent mouseMove) {
    instance->mousePos = Vector2((float) mouseMove.x, (float) mouseMove.y);
}

bool Mouse::isButtonPressed(int which) {
    return instance->pressedButtons[which];
}