/*
 * Author: Rodrigo Castro Azevedo
 * Date: 21/05/2014
 *
 * Description: The Keyboard class will keep track of all keyboard state changes. At any time during the execution of
 * the engine this class will know the exact state of all keys, if they are pressed or not. This can be used to detect
 * combinations of pressed keys, or simply test to see if a key is pressed at a specific frame. This class is a
 * Singleton, but all of its functions are static.
 */

#pragma once

#include <vector>
#include <algorithm>
#include <SDL.h>

class Keyboard {
public:

    ~Keyboard(void);

    /*
     * This function will be called by the engine when a key is either pressed or released, to tell Keyboard that its
     * state changed. This MUST NOT be called outside of the Naquadah::handleUserEvents() method, as doing so may cause
     * unknown behaviour.
     */
    static void recordKey(SDL_KeyboardEvent keyEvent);

    /* Returns the instance of this singleton class. */
    static Keyboard *getInstance() { return instance; }

    /* Checks if a specific key is pressed, returning true if it's pressed, or false otherwise. */
    static bool isKeyPressed(SDL_Keycode keyCode);

    /* These functions check if the modifier keys are pressed. They do not distinguish between left and right keys */
    static bool isCtrlPressed() { return isKeyPressed(SDLK_LCTRL) || isKeyPressed(SDLK_RCTRL); }
    static bool isShiftPressed() { return isKeyPressed(SDLK_LSHIFT) || isKeyPressed(SDLK_RSHIFT); }
    static bool isAltPressed() { return isKeyPressed(SDLK_LALT) || isKeyPressed(SDLK_RALT); }

protected:
    Keyboard(void);

    /* A vector containing all keys currently pressed. */
    std::vector<SDL_Keycode> *pressedKeys;

    /* The instance of this singleton class. */
    static Keyboard *instance;
};