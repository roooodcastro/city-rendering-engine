/*
 * Author: Rodrigo Castro Azevedo
 * Date: 21/05/2014
 *
 * Description: The Mouse class will keep track of the state of the mouse buttons and its position on screen. At any
 * time during the execution of the engine this class will know the exact state of all buttons and the cursor position,
 * This class is a Singleton, but all of its functions are static.
 */

#pragma once

#include <vector>
#include <algorithm>
#include <SDL.h>
#include "../math/Vector2.h"

class Mouse {
public:

    ~Mouse(void) {}

    /*
     * This function will be called by the engine when a mouse button is either pressed or released, to tell Mouse that
     * its state has changed. This MUST NOT be called outside of the Naquadah::handleUserEvents() method, as doing so
     * may cause unknown behaviour.
     */
    static void recordMouseButton(SDL_MouseButtonEvent buttonEvent);

    /*
     * This function will be called by the engine when the mouse cursor has moved, to tell Mouse its new position. This
     * MUST NOT be called outside of the Naquadah::handleUserEvents() method, as doing so may cause unknown behaviour.
     */
    static void recordMouseMove(SDL_MouseMotionEvent mouseMove);

    /* Returns the instance of this singleton class. */
    static Mouse *getInstance() { return instance; }

    /*
     * Checks if a specific button is pressed, returning true if it's pressed, or false otherwise. 'which' is an
     * integer to specify the button, with 0 being the left button, 1 the middle button and 2 the right button.
     */
    static bool isButtonPressed(int which);

    /* Returns the current position of the mouse on screen, relative to the top left corner of the game window. */
    static Vector2 getMousePos() { return instance->mousePos; }

protected:
    Mouse(void);

    /* An array that holds the state of the buttons. */
    bool pressedButtons[3];

    /* The position os the mouse cursor. */
    Vector2 mousePos;

    /* The instance of this singleton class. */
    static Mouse *instance;
};