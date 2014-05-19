/*
 * Author: Rodrigo Castro Azevedo
 * Date: 19/05
 *
 * Description: The Renderer class is mostly a collection of functionalities, resources and variables used together
 * with OpenGL to render the Scene. It contains the current OpenGL context and keeps track of every aspect of the
 * rendering pipeline. All OpenGL calls should be called from this class, using functions of the class, instead of
 * being directly called from outside.
 */

#pragma once

class Renderer {
public:

    Renderer(void);
    ~Renderer(void);
};