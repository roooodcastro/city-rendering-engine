/*
 * Author: Rodrigo Castro Azevedo
 * Date: 19/05
 *
 * Description: The Renderer class is mostly a collection of functionalities, resources and variables used together
 * with OpenGL to render the Scene. It contains the current OpenGL context and keeps track of every aspect of the
 * rendering pipeline. All OpenGL calls should be called from this class, using functions of the class, instead of
 * being directly called from outside.
 *
 * When the constructor of this class is called, all rendering functionalities of OpenGL and SDL will be turned on,
 * and a new desktop window will be created to render to. The constructor will load all setings, like window size,
 * fullscreen mode, and so on, from the config file of the ConfigurationManager, that is set on the initialization of
 * the engine itself. If the configuration file cannot be opened or the required values are not found, the default
 * values will be used.
 */

#pragma once

#include "../Naquadah.h"

class Renderer {
public:

    Renderer(void);
    ~Renderer(void);

    /*
     * Function that will be called to render the next frame into the back buffer, and flip the buffer afterwards.
     * The millisElapsed param represents the time since the last call to render. It does not represent the time since
     * the last update or physics update call.
     */
    void render(float millisElapsed);

    /* Returns true if the game is in fullscreen mode, false otherwise */
    bool isFullscreen() { return fullscreen; }

    /* Returns the current window size. */
    Vector2 getWindowSize() { return windowSize; }

    /*
     * Fetches all errors on the OpenGL error buffer and prints each one to the console window. Note that more than one
     * error may be printed on a single call, and errors that occurred a few frames ago but weren't printed will also
     * be printed before the new errors.
     */
    static void logOpenGLError(const std::string &msg) {
        GLenum errCode = glGetError();
        while (GL_NO_ERROR != errCode) {
            std::cout << "OpenGL Error " << errCode << ": " << msg << std::endl;
            errCode = glGetError();
        }
    }

protected:

    /* The size of the window. Defaults to (1280, 720). */
    Vector2 windowSize;

    /* True if the game is in fullscreen mode. Defaults to false. */
    bool fullscreen;

    /* The Window in which OpenGL will render to. */
    SDL_Window *window;

    /* The SDL Renderer, which will be used to aid creating textures for OpenGL. */
    SDL_Renderer *SDLRenderer;

    /* The OpenGL context that will be used to basically render the game. */
    SDL_GLContext glContext;
};